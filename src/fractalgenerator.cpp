/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Michał Męciński
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include "fractalgenerator.h"

#include <math.h>

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

#include "fraqtiveapplication.h"
#include "fractaldata.h"
#include "jobscheduler.h"
#include "datafunctions.h"

FractalGenerator::FractalGenerator( QObject* parent ) : QObject( parent ),
    m_preview( false ),
    m_priority( 0 ),
    m_receiver( NULL ),
    m_enabled( false ),
    m_functor( NULL ),
#if defined( HAVE_SSE2 )
    m_functorSSE2( NULL ),
#endif
    m_buffer( NULL ),
    m_activeJobs( 0 ),
    m_pending( false ),
    m_update( NoUpdate ),
    m_previewBuffer( NULL )
{
}

FractalGenerator::~FractalGenerator()
{
    QMutexLocker locker( &m_mutex );

    cancelJobs();
    m_enabled = false;

    while ( m_activeJobs > 0 )
        m_allJobsDone.wait( &m_mutex );

    delete m_functor;
#if defined( HAVE_SSE2 )
    delete m_functorSSE2;
#endif

    delete[] m_buffer;
    delete[] m_previewBuffer;
}

void FractalGenerator::setPreviewMode( bool preview )
{
    m_preview = preview;
}

void FractalGenerator::setPriority( int priority )
{
    m_priority = priority;
}

void FractalGenerator::setReceiver( QObject* receiver )
{
    m_receiver = receiver;
}

void FractalGenerator::setEnabled( bool enabled )
{
    QMutexLocker locker( &m_mutex );

    if ( enabled != m_enabled ) {
        m_enabled = enabled;

        if ( enabled )
            handleState();
        else
            cancelJobs();
    }
}

void FractalGenerator::setParameters( const FractalType& type, const Position& position )
{
    QMutexLocker locker( &m_mutex );

    m_pendingType = type;
    m_pendingPosition = position;
    m_pending = true;

    if ( !m_preview )
        reset();

    handleState();
}

void FractalGenerator::setFractalType( const FractalType& type )
{
    QMutexLocker locker( &m_mutex );

    m_pendingType = type;
    m_pending = true;

    if ( !m_preview )
        reset();

    handleState();
}

void FractalGenerator::setPosition( const Position& position )
{
    QMutexLocker locker( &m_mutex );

    m_pendingPosition = position;
    m_pending = true;

    if ( !m_preview )
        reset();

    handleState();
}

void FractalGenerator::setGeneratorSettings( const GeneratorSettings& settings )
{
    QMutexLocker locker( &m_mutex );

    m_pendingSettings = settings;
    m_pending = true;

    if ( !m_preview )
        reset();

    handleState();
}

static const int CellsPerRegion = 8;
static const int RegionSize = CellsPerRegion * GeneratorCore::CellSize + 1;

static int roundToCellSize( int size )
{
    // round up to nearest N * CellSize + 1
    return ( ( size - 1 + GeneratorCore::CellSize - 1 ) / GeneratorCore::CellSize ) * GeneratorCore::CellSize + 1;
}

void FractalGenerator::setResolution( const QSize& resolution )
{
    QMutexLocker locker( &m_mutex );

    int fullRegions = resolution.height() / RegionSize;
    int remainder = resolution.height() - fullRegions * RegionSize;

    int width = roundToCellSize( resolution.width() );
    int height = fullRegions * RegionSize;
    if ( remainder > 0 )
        height += roundToCellSize( remainder );

    m_pendingResolution = resolution;
    m_pendingBufferSize = QSize( width, height );
    m_pending = true;

    reset();

    handleState();
}

int FractalGenerator::maximumIterations() const
{
    return (int)( pow( 10.0, m_settings.calculationDepth() ) * qMax( 1.0, 1.45 + m_position.zoomFactor() ) );
}

FractalGenerator::UpdateStatus FractalGenerator::updateData( FractalData* data )
{
    QMutexLocker locker( &m_mutex );

    UpdateStatus update = m_update;
    m_update = NoUpdate;

    switch ( update ) {
        case InitialUpdate:
            data->setBuffer( m_buffer, m_bufferSize.width(), m_resolution );
            data->setValidRegions( m_validRegions );
            break;

        case PartialUpdate:
            if ( data->isEmpty() )
                return NoUpdate;
            data->setValidRegions( m_validRegions );
            break;

        case FullUpdate:
            data->transferBuffer( m_previewBuffer, m_bufferSize.width(), m_resolution );
            m_previewBuffer = NULL;
            data->setValidRegion( QRect( QPoint( 0, 0 ), m_resolution ) );
            break;

        case ClearUpdate:
            data->clear();
            break;

        default:
            break;
    }

    return update;
}

int FractalGenerator::priority() const
{
    return m_priority;
}

void FractalGenerator::executeJob()
{
    QMutexLocker locker( &m_mutex );

    if ( m_enabled && m_regions.count() > 0 )
        calculateRegion( m_regions.takeFirst() );

    finishJob();
    handleState();
}

void FractalGenerator::calculateRegion( const QRect& region )
{
    GeneratorCore::Input input;
    calculateInput( &input, region );

    GeneratorCore::Output output;
    calculateOutput( &output, region );

    int maxIterations = maximumIterations();
    double threshold = m_settings.detailThreshold();

    m_mutex.unlock();

#if defined( HAVE_SSE2 )
    if ( m_functorSSE2 ) {
        GeneratorCore::generatePreviewSSE2( input, output, m_functorSSE2, maxIterations );
        GeneratorCore::interpolate( output );
        GeneratorCore::generateDetailsSSE2( input, output, m_functorSSE2, maxIterations, threshold );
    } else
#endif
    if ( m_functor ) {
        GeneratorCore::generatePreview( input, output, m_functor, maxIterations );
        GeneratorCore::interpolate( output );
        GeneratorCore::generateDetails( input, output, m_functor, maxIterations, threshold );
    }

    m_mutex.lock();

    appendValidRegion( region );

    if ( !m_preview && m_update == NoUpdate )
        postUpdate( PartialUpdate );
}

void FractalGenerator::reset()
{
    m_update = ClearUpdate;
    cancelJobs();
}

void FractalGenerator::handleState()
{
    if ( m_activeJobs > 0 || !m_enabled || m_pendingResolution.isEmpty() )
        return;

    if ( !m_regions.isEmpty() && !m_pending ) {
        addJobs();
        return;
    }

    if ( m_preview && m_buffer && m_regions.isEmpty() && m_resolution == m_pendingResolution ) {
        delete[] m_previewBuffer;
        m_previewBuffer = m_buffer;
        m_buffer = NULL;

        postUpdate( FullUpdate );
    }

    if ( m_pending ) {
        if ( m_buffer && m_bufferSize != m_pendingBufferSize ) {
            delete[] m_buffer;
            m_buffer = NULL;
        }

        m_type = m_pendingType;
        m_position = m_pendingPosition;
        m_settings = m_pendingSettings;
        m_resolution = m_pendingResolution;
        m_bufferSize = m_pendingBufferSize;
        m_pending = false;

        if ( !m_buffer )
            m_buffer = new double[ m_bufferSize.width() * m_bufferSize.height() ];

        createFunctor();

        if ( !m_preview )
            postUpdate( InitialUpdate );

        m_validRegions.clear();

        splitRegions();
        addJobs();
    }
}

void FractalGenerator::createFunctor()
{
    delete m_functor;
    m_functor = NULL;

#if defined( HAVE_SSE2 )
    delete m_functorSSE2;
    m_functorSSE2 = DataFunctions::createFunctorSSE2( m_type );
    if ( m_functorSSE2 != NULL )
        return;
#endif

    m_functor = DataFunctions::createFunctor( m_type );
}

void FractalGenerator::splitRegions()
{
    m_regions.clear();

    int fullRegions = m_bufferSize.height() / RegionSize;
    int remainder = m_bufferSize.height() - fullRegions * RegionSize;

    for ( int i = 0; i < fullRegions; i++ ) {
        QRect region( 0, i * RegionSize, m_bufferSize.width(), RegionSize );
        m_regions.append( region );
    }

    if ( remainder > 0 ) {
        QRect region( 0, fullRegions * RegionSize, m_bufferSize.width(), remainder );
        m_regions.append( region );
    }
}

void FractalGenerator::calculateInput( GeneratorCore::Input* input, const QRect& region )
{
    double scale = pow( 10.0, -m_position.zoomFactor() ) / (double)m_resolution.height();

    double sa = scale * sin( m_position.angle() * M_PI / 180.0 );
    double ca = scale * cos( m_position.angle() * M_PI / 180.0 );

    double offsetX = (double)region.left() - (double)m_resolution.width() / 2.0 - 0.5;
    double offsetY = (double)region.top() - (double)m_resolution.height() / 2.0 - 0.5;

    input->m_sa = sa;
    input->m_ca = ca;
    input->m_x = m_position.center().x() + ca * offsetX + sa * offsetY;
    input->m_y = m_position.center().y() - sa * offsetX + ca * offsetY;
}

void FractalGenerator::calculateOutput( GeneratorCore::Output* output, const QRect& region )
{
    output->m_buffer = m_buffer + region.top() * m_bufferSize.width() + region.left();
    output->m_stride = m_bufferSize.width();
    output->m_width = region.width();
    output->m_height = region.height();
}

void FractalGenerator::addJobs()
{
    int count = m_regions.count();
    if ( count > 0 ) {
        fraqtive()->jobScheduler()->addJobs( this, count );
        m_activeJobs += count;
    }
}

void FractalGenerator::cancelJobs()
{
    int count = fraqtive()->jobScheduler()->cancelAllJobs( this );
    m_activeJobs -= count;

    if ( m_activeJobs == 0 )
        m_allJobsDone.wakeAll();
}

void FractalGenerator::finishJob()
{
    m_activeJobs--;

    if ( m_activeJobs == 0 )
        m_allJobsDone.wakeAll();
}

void FractalGenerator::appendValidRegion( const QRect& region )
{
    QRect clipped = region.intersected( QRect( QPoint( 0, 0 ), m_resolution ) );
    if ( clipped.isEmpty() )
        return;

    for ( int i = 0; i < m_validRegions.count(); i++ ) {
        if ( m_validRegions[ i ].bottom() + 1 == clipped.top() ) {
            if ( i + 1 < m_validRegions.count() && m_validRegions[ i + 1 ].top() == clipped.bottom() + 1 )
                m_validRegions[ i ].setBottom( m_validRegions.takeAt( i + 1 ).bottom() );
            else
                m_validRegions[ i ].setBottom( clipped.bottom() );
            return;
        }
        if ( m_validRegions[ i ].top() == clipped.bottom() + 1 ) {
            m_validRegions[ i ].setTop( clipped.top() );
            return;
        }
        if ( m_validRegions[ i ].top() > clipped.top() ) {
            m_validRegions.insert( i, clipped );
            return;
        }
    }
    m_validRegions.append( clipped );
}

void FractalGenerator::postUpdate( UpdateStatus update )
{
    m_update = update;

    if ( m_receiver )
        QApplication::postEvent( m_receiver, new QEvent( UpdateEvent ) );
}

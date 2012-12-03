/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Micha³ Mêciñski
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

#include "imagegenerator.h"

#include <math.h>

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

#include <QPalette>
#include <QPainter>
#include <QIcon>

#include "fraqtiveapplication.h"
#include "fractaldata.h"
#include "jobscheduler.h"
#include "datafunctions.h"

ImageGenerator::ImageGenerator( QObject* parent ) : QObject( parent ),
    m_gradientCache( NULL ),
    m_maximumProgress( 0 ),
    m_activeJobs( 0 ),
    m_imageCount( 1 ),
    m_currentImage( 0 )
{
}

ImageGenerator::~ImageGenerator()
{
    QMutexLocker locker( &m_mutex );

    cancelJobs();

    while ( m_activeJobs > 0 )
        m_allJobsDone.wait( &m_mutex );

    delete[] m_gradientCache;
}

static const int CellsPerRegion = 20;
static const int RegionSize = CellsPerRegion * GeneratorCore::CellSize + 1;

static int roundToCellSize( int size )
{
    // round up to nearest N * CellSize + 1
    return ( ( size - 1 + GeneratorCore::CellSize - 1 ) / GeneratorCore::CellSize ) * GeneratorCore::CellSize + 1;
}

void ImageGenerator::setResolution( const QSize& resolution )
{
    m_resolution = resolution;

    int height = resolution.height() + 2;

    int fullRegions = height / ( RegionSize - 2 );
    int remainder = height - fullRegions * ( RegionSize - 2 );

    m_maximumProgress = fullRegions;

    if ( remainder > 0 )
        m_maximumProgress++;
}

void ImageGenerator::setParameters( const FractalType& type, const Position& position )
{
    m_type = type;
    m_position = position;
}

static const int GradientSize = 16384;

void ImageGenerator::setColorSettings( const Gradient& gradient, const QColor& backgroundColor, const ColorMapping& mapping )
{
    if ( !m_gradientCache )
        m_gradientCache = new QRgb[ GradientSize ];

    DataFunctions::fillGradientCache( gradient, m_gradientCache, GradientSize );

    m_backgroundColor = backgroundColor;
    m_colorMapping = mapping;
}

void ImageGenerator::setGeneratorSettings( const GeneratorSettings& settings )
{
    m_generatorSettings = settings;
}

void ImageGenerator::setViewSettings( const ViewSettings& settings )
{
    m_viewSettings = settings;
}

void ImageGenerator::setImageCount( int count )
{
    m_imageCount = count;
}

void ImageGenerator::setCurrentImage( int image )
{
    m_currentImage = image;
}

QImage ImageGenerator::takeImage()
{
    QImage result = m_image;
    m_image = QImage();
    return result;
}

bool ImageGenerator::start()
{
    m_image = QImage( m_resolution, QImage::Format_RGB32 );

    if ( m_image.isNull() )
        return false;

    m_regions.clear();

    int width = roundToCellSize( m_image.width() + 2 );
    int height = m_image.height() + 2;

    int fullRegions = height / ( RegionSize - 2 );
    int remainder = height - fullRegions * ( RegionSize - 2 );

    for ( int i = 0; i < fullRegions; i++ ) {
        QRect region( 0, i * ( RegionSize - 2 ), width, RegionSize );
        m_regions.append( region );
    }

    if ( remainder > 0 ) {
        QRect region( 0, fullRegions * ( RegionSize - 2 ), width, roundToCellSize( remainder ) );
        m_regions.append( region );
    }

    addJobs();

    return true;
}

int ImageGenerator::priority() const
{
    return 1;
}

void ImageGenerator::executeJob()
{
    QMutexLocker locker( &m_mutex );

    if ( m_regions.count() > 0 )
        calculateRegion( m_regions.takeFirst() );

    finishJob();
}

void ImageGenerator::calculateRegion( const QRect& region )
{
    GeneratorCore::Input input;
    calculateInput( &input, region );

    GeneratorCore::Output output;
    calculateOutput( &output, region );

    int maxIterations = maximumIterations();
    double threshold = m_generatorSettings.detailThreshold();

    m_mutex.unlock();

#if defined( HAVE_SSE2 )
    GeneratorCore::FunctorSSE2* functorSSE2 = DataFunctions::createFunctorSSE2( m_type );
    if ( functorSSE2 ) {
        GeneratorCore::generatePreviewSSE2( input, output, functorSSE2, maxIterations );
        GeneratorCore::interpolate( output );
        GeneratorCore::generateDetailsSSE2( input, output, functorSSE2, maxIterations, threshold );
        delete functorSSE2;
    } else {
#endif
        GeneratorCore::Functor* functor = DataFunctions::createFunctor( m_type );
        if ( functor ) {
            GeneratorCore::generatePreview( input, output, functor, maxIterations );
            GeneratorCore::interpolate( output );
            GeneratorCore::generateDetails( input, output, functor, maxIterations, threshold );
            delete functor;
        }
#if defined( HAVE_SSE2 )
    }
#endif

    m_mutex.lock();

    FractalData data;
    data.transferBuffer( output.m_buffer, output.m_stride, region.size() );

    DataFunctions::ColorMapper mapper( m_gradientCache, GradientSize, m_backgroundColor.rgb(), m_colorMapping );

    DataFunctions::drawImage( m_image, region.topLeft(), &data, QRect( 0, 0, m_image.width(), qMin( region.height() - 2, m_image.height() - region.top() ) ),
        mapper, m_viewSettings.antiAliasing() );
}

void ImageGenerator::calculateInput( GeneratorCore::Input* input, const QRect& region )
{
    double scale = pow( 10.0, -m_position.zoomFactor() ) / (double)m_image.height();

    double sa = scale * sin( m_position.angle() * M_PI / 180.0 );
    double ca = scale * cos( m_position.angle() * M_PI / 180.0 );

    double offsetX = (double)region.left() - (double)m_image.width() / 2.0 - 0.5;
    double offsetY = (double)region.top() - (double)m_image.height() / 2.0 - 0.5;

    input->m_sa = sa;
    input->m_ca = ca;
    input->m_x = m_position.center().x() + ca * offsetX + sa * offsetY;
    input->m_y = m_position.center().y() - sa * offsetX + ca * offsetY;
}

void ImageGenerator::calculateOutput( GeneratorCore::Output* output, const QRect& region )
{
    output->m_buffer = new double[ region.width() * region.height() ];
    output->m_stride = region.width();
    output->m_width = region.width();
    output->m_height = region.height();
}

int ImageGenerator::maximumIterations() const
{
    return (int)( pow( 10.0, m_generatorSettings.calculationDepth() ) * qMax( 1.0, 1.45 + m_position.zoomFactor() ) );
}

void ImageGenerator::addJobs()
{
    int count = m_regions.count();
    if ( count > 0 ) {
        fraqtive()->jobScheduler()->addJobs( this, count );
        m_activeJobs += count;
    }
}

void ImageGenerator::cancelJobs()
{
    int count = fraqtive()->jobScheduler()->cancelAllJobs( this );
    m_activeJobs -= count;

    emit progressChanged( m_maximumProgress * m_currentImage + m_maximumProgress - m_activeJobs );

    if ( m_activeJobs == 0 )
        m_allJobsDone.wakeAll();
}

void ImageGenerator::finishJob()
{
    m_activeJobs--;

    emit progressChanged( m_maximumProgress * m_currentImage + m_maximumProgress - m_activeJobs );

    if ( m_activeJobs == 0 ) {
        m_allJobsDone.wakeAll();
        emit completed();
    }
}

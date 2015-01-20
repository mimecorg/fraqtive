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

#include "bookmarkmodel.h"

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

Q_DECLARE_METATYPE( QModelIndex )

BookmarkModel::BookmarkModel( QObject* parent ) : QAbstractListModel( parent ),
    m_gradientCache( NULL ),
    m_enabled( false ),
    m_activeJobs( 0 )
{
    qRegisterMetaType<QModelIndex>();
}

BookmarkModel::~BookmarkModel()
{
    QMutexLocker locker( &m_mutex );

    cancelJobs();

    while ( m_activeJobs > 0 )
        m_allJobsDone.wait( &m_mutex );

    delete[] m_gradientCache;
}

void BookmarkModel::setMap( BookmarkMap* map )
{
    m_map = map;

    update();

    m_enabled = true;
    m_queue = m_keys;
}

static const int GradientSize = 16384;

void BookmarkModel::setColorSettings( const Gradient& gradient, const QColor& backgroundColor, const ColorMapping& mapping )
{
    QMutexLocker locker( &m_mutex );

    cancelJobs();

    if ( !m_gradientCache )
        m_gradientCache = new QRgb[ GradientSize ];

    DataFunctions::fillGradientCache( gradient, m_gradientCache, GradientSize );

    m_backgroundColor = backgroundColor;
    m_colorMapping = mapping;
}

void BookmarkModel::abortGeneration()
{
    QMutexLocker locker( &m_mutex );

    m_enabled = false;

    cancelJobs();
}

void BookmarkModel::continueGeneration()
{
    QMutexLocker locker( &m_mutex );

    m_enabled = true;

    addJobs();
}

void BookmarkModel::invalidateBookmark( const QString& name )
{
    QMutexLocker locker( &m_mutex );

    m_images.remove( name );

    if ( !m_queue.contains( name ) ) {
        m_queue.append( name );
        addJobs( 1 );
    }
}

static bool localeAwareLessThan( const QString& s1, const QString& s2 )
{
    return QString::localeAwareCompare( s1, s2 ) < 0;
}

void BookmarkModel::update()
{
    QMutexLocker locker( &m_mutex );

    cancelJobs();

    beginResetModel();

    m_keys = m_map->keys();
    qSort( m_keys.begin(), m_keys.end(), localeAwareLessThan );

    endResetModel();
}

int BookmarkModel::rowCount( const QModelIndex& parent ) const
{
    if ( !parent.isValid() )
        return m_keys.count();
    return 0;
}

QVariant BookmarkModel::data( const QModelIndex& index, int role ) const
{
    if ( role == Qt::DisplayRole )
        return m_keys.at( index.row() );

    if ( role == Qt::DecorationRole ) {
        QMutexLocker locker( &m_mutex );

        QString name = m_keys.at( index.row() );

        QPixmap pixmap;
        if ( m_images.contains( name ) ) {
            pixmap = QPixmap::fromImage( m_images.value( name ) );
        } else {
            pixmap = QPixmap( 48, 48 );
            pixmap.fill( QApplication::palette().color( QPalette::Disabled, QPalette::Window ) );
        }

        locker.unlock();

        QPainter painter( &pixmap );

        QPixmap pixmap2 = pixmap;
        QPainter painter2( &pixmap2 );

        painter.setPen( QApplication::palette().color( QPalette::Dark ) );
        painter.drawRect( pixmap.rect().adjusted( 0, 0, -1, -1 ) );

        painter2.setPen( QApplication::palette().color( QPalette::Highlight ) );
        painter2.drawRect( pixmap.rect().adjusted( 0, 0, -1, -1 ) );
        painter2.drawRect( pixmap.rect().adjusted( 1, 1, -2, -2 ) );

        QIcon icon;
        icon.addPixmap( pixmap, QIcon::Normal );
        icon.addPixmap( pixmap2, QIcon::Selected );

        return icon;
    }

    return QVariant();
}

int BookmarkModel::priority() const
{
    return 1;
}

static int roundToCellSize( int size )
{
    // round up to nearest N * CellSize + 1
    return ( ( size - 1 + GeneratorCore::CellSize - 1 ) / GeneratorCore::CellSize ) * GeneratorCore::CellSize + 1;
}

void BookmarkModel::executeJob()
{
    QMutexLocker locker( &m_mutex );

    if ( !m_enabled || m_queue.isEmpty() ) {
        finishJob();
        return;
    }

    QString name = m_queue.takeFirst();

    if ( !m_map->contains( name ) ) {
        finishJob();
        return;
    }

    Bookmark bookmark = m_map->value( name );

    locker.unlock();

    const int imageSize = 48;
    const int bufferSize = roundToCellSize( imageSize + 2 ); // 2 pixel margin for anti-aliasing

    double* buffer = new double[ bufferSize * bufferSize ];

    calculate( bookmark, buffer, QSize( bufferSize, bufferSize ), QSize( imageSize, imageSize ) );

    FractalData data;
    data.transferBuffer( buffer, bufferSize, QSize( imageSize, imageSize ) );

    QImage image( imageSize, imageSize, QImage::Format_RGB32 );

    ViewSettings settings = DataFunctions::defaultViewSettings();

    DataFunctions::ColorMapper mapper( m_gradientCache, GradientSize, m_backgroundColor.rgb(), m_colorMapping );
    DataFunctions::drawImage( image, &data, image.rect(), mapper, settings.antiAliasing() );

    locker.relock();

    if ( m_queue.contains( name ) ) {
        finishJob();
        return;
    }

    m_images.insert( name, image );

    int row = m_keys.indexOf( name );
    if ( row >= 0 )
        emit dataChanged( index( row ), index( row ) );

    finishJob();
}

void BookmarkModel::calculate( const Bookmark& bookmark, double* buffer, const QSize& size, const QSize& resolution )
{
    GeneratorCore::Input input;

    Position position = bookmark.position();

    double scale = pow( 10.0, -position.zoomFactor() ) / (double)resolution.height();

    double sa = scale * sin( position.angle() * M_PI / 180.0 );
    double ca = scale * cos( position.angle() * M_PI / 180.0 );

    double offsetX = -(double)resolution.width() / 2.0 - 0.5;
    double offsetY = -(double)resolution.height() / 2.0 - 0.5;

    input.m_sa = sa;
    input.m_ca = ca;
    input.m_x = position.center().x() + ca * offsetX + sa * offsetY;
    input.m_y = position.center().y() - sa * offsetX + ca * offsetY;

    GeneratorCore::Output output;

    output.m_buffer = buffer;
    output.m_width = size.width();
    output.m_height = size.height();
    output.m_stride = size.width();

    GeneratorSettings settings = DataFunctions::defaultGeneratorSettings();

    int maxIterations = (int)( pow( 10.0, settings.calculationDepth() ) * qMax( 1.0, 1.45 + position.zoomFactor() ) );
    double threshold = settings.detailThreshold();

#if defined( HAVE_SSE2 )
    GeneratorCore::FunctorSSE2* functorSSE2 = DataFunctions::createFunctorSSE2( bookmark.fractalType() );
    if ( functorSSE2 ) {
        GeneratorCore::generatePreviewSSE2( input, output, functorSSE2, maxIterations );
        GeneratorCore::interpolate( output );
        GeneratorCore::generateDetailsSSE2( input, output, functorSSE2, maxIterations, threshold );
        delete functorSSE2;
        return;
    }
#endif

    GeneratorCore::Functor* functor = DataFunctions::createFunctor( bookmark.fractalType() );
    if ( functor ) {
        GeneratorCore::generatePreview( input, output, functor, maxIterations );
        GeneratorCore::interpolate( output );
        GeneratorCore::generateDetails( input, output, functor, maxIterations, threshold );
        delete functor;
    }
}

void BookmarkModel::addJobs( int count /*= -1*/ )
{
    if ( count < 0 )
        count = m_queue.count();
    if ( count > 0 ) {
        fraqtive()->jobScheduler()->addJobs( this, count );
        m_activeJobs += count;
    }
}

void BookmarkModel::cancelJobs()
{
    int count = fraqtive()->jobScheduler()->cancelAllJobs( this );
    m_activeJobs -= count;

    if ( m_activeJobs == 0 )
        m_allJobsDone.wakeAll();
}

void BookmarkModel::finishJob()
{
    m_activeJobs--;

    if ( m_activeJobs == 0 )
        m_allJobsDone.wakeAll();
}

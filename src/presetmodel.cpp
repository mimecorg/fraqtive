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

#include "presetmodel.h"

#include <QPainter>
#include <QApplication>
#include <QPalette>
#include <QIcon>

#include "datafunctions.h"

PresetModel::PresetModel( QObject* parent ) : QAbstractListModel( parent ),
    m_map( NULL )
{
}

PresetModel::~PresetModel()
{
}

void PresetModel::setMap( PresetMap* map )
{
    m_map = map;

    update();
}

static bool localeAwareLessThan( const QString& s1, const QString& s2 )
{
    return QString::localeAwareCompare( s1, s2 ) < 0;
}
 
void PresetModel::update()
{
    beginResetModel();

    m_keys = m_map->keys();
    qSort( m_keys.begin(), m_keys.end(), localeAwareLessThan );

    endResetModel();
}

int PresetModel::rowCount( const QModelIndex& parent ) const
{
    if ( !parent.isValid() )
        return m_keys.count();
    return 0;
}

QVariant PresetModel::data( const QModelIndex& index, int role ) const
{
    if ( role == Qt::DisplayRole )
        return m_keys.at( index.row() );

    if ( role == Qt::DecorationRole ) {
        QString name = m_keys.at( index.row() );

        Preset preset = m_map->value( name );

        QPixmap pixmap( 48, 48 );
        QPainter painter( &pixmap );

        QGradientStops stops = DataFunctions::calculateGradientStops( preset.gradient() );

        bool reversed = preset.colorMapping().isReversed();
        bool mirrored = preset.colorMapping().isMirrored();

        if ( ( !mirrored && !reversed ) || ( mirrored && reversed ) ) {
            for ( int i = 0; i < stops.count() / 2; i++ ) {
                QGradientStop stop1 = stops.at( i );
                QGradientStop stop2 = stops.at( stops.count() - i - 1 );
                stops[ i ] = QGradientStop( 1.0 - stop2.first, stop2.second );
                stops[ stops.count() - i - 1 ] = QGradientStop( 1.0 - stop1.first, stop1.second );
            }
            if ( stops.count() % 2 == 1 ) {
                QGradientStop stop = stops.at( stops.count() / 2 );
                stops[ stops.count() / 2 ] = QGradientStop( 1.0 - stop.first, stop.second );
            }
        }

        if ( mirrored ) {
            for ( int i = stops.count() - 1; i >= 0; i-- ) {
                QGradientStop stop = stops.at( i );
                stops[ i ] = QGradientStop( 0.5 * stop.first, stop.second );
                stops.append( QGradientStop( 1.0 - 0.5 * stop.first, stop.second ) );
            }
        }

        double angle = 90.0 + 360.0 * preset.colorMapping().offset();

        QConicalGradient gradient( pixmap.width() / 2, pixmap.height() / 2, angle );
        gradient.setStops( stops );

        painter.fillRect( pixmap.rect(), gradient );

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

/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "bookmarkmodel.h"

BookmarkModel::BookmarkModel( QObject* parent ) : QAbstractListModel( parent )
{
}

BookmarkModel::~BookmarkModel()
{
}


void BookmarkModel::setMap( BookmarkMap* map )
{
    m_map = map;

    update();
}

static bool localeAwareLessThan( const QString& s1, const QString& s2 )
{
    return QString::localeAwareCompare( s1, s2 ) < 0;
}
 
void BookmarkModel::update()
{
    m_keys = m_map->keys();

    qSort( m_keys.begin(), m_keys.end(), localeAwareLessThan );

    reset();
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
        QPixmap pixmap( ":/icons/fraqtive-48.png", NULL );
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

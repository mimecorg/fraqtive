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

#ifndef BOOKMARKLISTVIEW_H
#define BOOKMARKLISTVIEW_H

#include <QListView>

#include "datastructures.h"

class BookmarkListView : public QListView
{
    Q_OBJECT
public:
    BookmarkListView( QWidget* parent );
    ~BookmarkListView();

public:
    void setMap( BookmarkMap* map );

    void setColorSettings( const Gradient& gradient, const QColor& backgroundColor, const ColorMapping& mapping );

protected: // overrides
    void contextMenuEvent( QContextMenuEvent* e );

private slots:
    void renameItem();
    void deleteItem();
};

#endif

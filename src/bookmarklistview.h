/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
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

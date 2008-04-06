/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H

#include <QAbstractListModel>

#include "datastructures.h"

class BookmarkModel : public QAbstractListModel
{
    Q_OBJECT
public:
    BookmarkModel( QObject* parent );
    ~BookmarkModel();

public:
    void setMap( BookmarkMap* map );
    BookmarkMap* map() const { return m_map; }

    void update();

public: // overrides
    int rowCount( const QModelIndex& parent ) const;

    QVariant data( const QModelIndex& index, int role ) const;

private:
    BookmarkMap* m_map;

    QStringList m_keys;
};

#endif

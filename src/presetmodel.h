/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef PRESETMODEL_H
#define PRESETMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

#include "datastructures.h"

class PresetModel : public QAbstractListModel
{
    Q_OBJECT
public:
    PresetModel( QObject* parent );
    ~PresetModel();

public:
    void setMap( PresetMap* map );
    PresetMap* map() const { return m_map; }

    void update();

public: // overrides
    int rowCount( const QModelIndex& parent ) const;

    QVariant data( const QModelIndex& index, int role ) const;

private:
    PresetMap* m_map;

    QStringList m_keys;
};

#endif

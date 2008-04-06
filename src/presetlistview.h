/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef PRESETLISTVIEW_H
#define PRESETLISTVIEW_H

#include <QListView>

#include "datastructures.h"

class PresetListView : public QListView
{
    Q_OBJECT
public:
    PresetListView( QWidget* parent );
    ~PresetListView();

public:
    void setMap( PresetMap* map );

    void setEditable( bool editable );

protected: // overrides
    void contextMenuEvent( QContextMenuEvent* e );

private slots:
    void renameItem();
    void deleteItem();

private:
    bool m_editable;
};

#endif

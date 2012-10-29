/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "presetlistview.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QMessageBox>

#include "presetmodel.h"
#include "renamedialog.h"

PresetListView::PresetListView( QWidget* parent ) : QListView( parent ),
    m_editable( false )
{
    setViewMode( QListView::IconMode );
    setFlow( QListView::LeftToRight );
    setMovement( QListView::Static );
    setResizeMode( QListView::Adjust );
    setWrapping( true );
    setIconSize( QSize( 48, 48 ) );
    setSpacing( 6 );
    setWordWrap( true );

    setModel( new PresetModel( this ) );
}

PresetListView::~PresetListView()
{
}

void PresetListView::setMap( PresetMap* map )
{
    ( (PresetModel*)model() )->setMap( map );
}

void PresetListView::setEditable( bool editable )
{
    m_editable = editable;
}

void PresetListView::contextMenuEvent( QContextMenuEvent* e )
{
    if ( !m_editable )
        return;

    QModelIndexList selection = selectionModel()->selectedIndexes();

    if ( selection.count() == 1 ) {
        QMenu menu( this );

        menu.addAction( QIcon( ":/icons/rename-16.png" ), tr( "&Rename" ), this, SLOT( renameItem() ) );
        menu.addAction( QIcon( ":/icons/delete-16.png" ), tr( "&Delete" ), this, SLOT( deleteItem() ) );

        menu.exec( e->globalPos() );
    }
}

void PresetListView::renameItem()
{
    QModelIndexList selection = selectionModel()->selectedIndexes();

    if ( selection.count() == 1 ) {
        QString name = model()->data( selection[ 0 ] ).toString();
        PresetMap* map = ( (PresetModel*)model() )->map();

        RenameDialog dialog( RenameDialog::PresetMode, name, this );

        dialog.setExistingNames( map->keys() );

        if ( dialog.exec() == QDialog::Accepted ) {
            Preset preset = map->take( name );
            map->insert( dialog.name(), preset );

            ( (PresetModel*)model() )->update();
        }
    }
}

void PresetListView::deleteItem()
{
    QModelIndexList selection = selectionModel()->selectedIndexes();

    if ( selection.count() == 1 ) {
        QString name = model()->data( selection[ 0 ] ).toString();

        if ( QMessageBox::warning( this, tr( "Warning" ), tr( "<qt>Are you sure you want to delete preset<br>named <b>%1</b>?</qt>" ).arg( name ),
            QMessageBox::Ok | QMessageBox::Cancel ) == QMessageBox::Ok ) {
            PresetMap* map = ( (PresetModel*)model() )->map();
            map->remove( name );

            ( (PresetModel*)model() )->update();
        }
    }
}

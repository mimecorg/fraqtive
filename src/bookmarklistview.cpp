/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "bookmarklistview.h"

#include "bookmarkmodel.h"
#include "renamedialog.h"

BookmarkListView::BookmarkListView( QWidget* parent ) : QListView( parent )
{
    setViewMode( QListView::IconMode );
    setFlow( QListView::LeftToRight );
    setMovement( QListView::Static );
    setResizeMode( QListView::Adjust );
    setWrapping( true );
    setIconSize( QSize( 48, 48 ) );
    setSpacing( 6 );
    setWordWrap( true );

    setModel( new BookmarkModel( this ) );
}

BookmarkListView::~BookmarkListView()
{
}


void BookmarkListView::setMap( BookmarkMap* map )
{
    ( (BookmarkModel*)model() )->setMap( map );
}

void BookmarkListView::setColorSettings( const Gradient& gradient, const QColor& backgroundColor, const ColorMapping& mapping )
{
    BookmarkModel* bookmarkModel = (BookmarkModel*)model();
    bookmarkModel->setColorSettings( gradient, backgroundColor, mapping );
    bookmarkModel->continueGeneration();
}

void BookmarkListView::contextMenuEvent( QContextMenuEvent* e )
{
    QModelIndexList selection = selectionModel()->selectedIndexes();

    if ( selection.count() == 1 ) {
        QMenu menu( this );

        menu.addAction( QIcon( ":/icons/rename-16.png" ), tr( "&Rename" ), this, SLOT( renameItem() ) );
        menu.addAction( QIcon( ":/icons/delete-16.png" ), tr( "&Delete" ), this, SLOT( deleteItem() ) );

        menu.exec( e->globalPos() );
    }
}

void BookmarkListView::renameItem()
{
    QModelIndexList selection = selectionModel()->selectedIndexes();

    if ( selection.count() == 1 ) {
        BookmarkModel* bookmarkModel = (BookmarkModel*)model();
        BookmarkMap* map = bookmarkModel->map();

        QString name = model()->data( selection[ 0 ] ).toString();

        RenameDialog dialog( this );

        dialog.setMode( RenameDialog::BookmarkMode );
        dialog.setName( name );
        dialog.setExistingNames( map->keys() );

        if ( dialog.exec() == QDialog::Accepted ) {
            bookmarkModel->abortGeneration();

            Bookmark bookmark = map->take( name );
            map->insert( dialog.name(), bookmark );

            bookmarkModel->invalidateBookmark( name );
            bookmarkModel->invalidateBookmark( dialog.name() );
            bookmarkModel->update();

            bookmarkModel->continueGeneration();
        }
    }
}

void BookmarkListView::deleteItem()
{
    QModelIndexList selection = selectionModel()->selectedIndexes();

    if ( selection.count() == 1 ) {
        QString name = model()->data( selection[ 0 ] ).toString();

        if ( QMessageBox::warning( this, tr( "Warning" ), tr( "<qt>Are you sure you want to delete bookmark<br>named <b>%1</b>?</qt>" ).arg( name ),
            QMessageBox::Ok | QMessageBox::Cancel ) == QMessageBox::Ok ) {
            BookmarkModel* bookmarkModel = (BookmarkModel*)model();

            bookmarkModel->abortGeneration();

            BookmarkMap* map = bookmarkModel->map();
            map->remove( name );

            bookmarkModel->invalidateBookmark( name );
            bookmarkModel->update();

            bookmarkModel->continueGeneration();
        }
    }
}

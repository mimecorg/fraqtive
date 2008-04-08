/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "savebookmarkdialog.h"

#include "fraqtiveapplication.h"
#include "configurationdata.h"
#include "fractalmodel.h"

SaveBookmarkDialog::SaveBookmarkDialog( QWidget* parent ) : QDialog( parent )
{
    m_ui.setupUi( this );

    connect( m_ui.listView->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ),
        this, SLOT( selectionChanged() ) );

    m_ui.listView->setMap( fraqtive()->configuration()->bookmarks() );

    m_ui.editName->setText( tr( "New Bookmark" ) );
    m_ui.editName->setFocus();
}

SaveBookmarkDialog::~SaveBookmarkDialog()
{
}

void SaveBookmarkDialog::setModel( FractalModel* model )
{
    m_model = model;

    m_ui.listView->setColorSettings( model->gradient(), model->backgroundColor(), model->colorMapping() );
}

void SaveBookmarkDialog::on_listView_doubleClicked()
{
    accept();
}

void SaveBookmarkDialog::on_editName_textChanged( const QString& text )
{
    m_ui.buttonBox->button( QDialogButtonBox::Ok )->setEnabled( !text.isEmpty() );

    if ( !m_keepSelection )
        m_ui.listView->clearSelection();
}

void SaveBookmarkDialog::selectionChanged()
{
    QModelIndexList selection = m_ui.listView->selectionModel()->selectedIndexes();
    if ( selection.count() == 1 ) {
        QString name = m_ui.listView->model()->data( selection[ 0 ] ).toString();

        m_keepSelection = true;
        m_ui.editName->setText( name );
        m_keepSelection = false;
    }
}

void SaveBookmarkDialog::accept()
{
    QString name = m_ui.editName->text();

    BookmarkMap* map = fraqtive()->configuration()->bookmarks();

    if ( map->contains( name ) ) {
        if ( QMessageBox::warning( this, tr( "Warning" ), tr( "<qt>Are you sure you want to overwrite the existing bookmark<br>named <b>%1</b>?</qt>" ).arg( name ),
            QMessageBox::Ok | QMessageBox::Cancel ) != QMessageBox::Ok )
            return;
    }

    Bookmark bookmark;
    bookmark.setFractalType( m_model->fractalType() );
    bookmark.setPosition( m_model->position() );

    map->insert( name, bookmark );

    QDialog::accept();
}

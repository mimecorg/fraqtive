/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "savepresetdialog.h"

#include "fraqtiveapplication.h"
#include "configurationdata.h"
#include "fractalmodel.h"

SavePresetDialog::SavePresetDialog( QWidget* parent ) : QDialog( parent ),
    m_model( NULL ),
    m_keepSelection( false )
{
    m_ui.setupUi( this );

    connect( m_ui.listView->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ),
        this, SLOT( selectionChanged() ) );

    m_ui.listView->setMap( fraqtive()->configuration()->userPresets() );
    m_ui.listView->setEditable( true );

    m_ui.editName->setText( tr( "New Preset" ) );
    m_ui.editName->setFocus();
}

SavePresetDialog::~SavePresetDialog()
{
}

void SavePresetDialog::setModel( FractalModel* model )
{
    m_model = model;
}

void SavePresetDialog::on_listView_doubleClicked()
{
    accept();
}

void SavePresetDialog::on_editName_textChanged( const QString& text )
{
    m_ui.buttonBox->button( QDialogButtonBox::Ok )->setEnabled( !text.isEmpty() );

    if ( !m_keepSelection )
        m_ui.listView->clearSelection();
}

void SavePresetDialog::selectionChanged()
{
    QModelIndexList selection = m_ui.listView->selectionModel()->selectedIndexes();
    if ( selection.count() == 1 ) {
        QString name = m_ui.listView->model()->data( selection[ 0 ] ).toString();

        m_keepSelection = true;
        m_ui.editName->setText( name );
        m_keepSelection = false;
    }
}

void SavePresetDialog::accept()
{
    QString name = m_ui.editName->text();

    PresetMap* map = fraqtive()->configuration()->userPresets();

    if ( map->contains( name ) ) {
        if ( QMessageBox::warning( this, tr( "Warning" ), tr( "<qt>Are you sure you want to overwrite the existing preset<br>named <b>%1</b>?</qt>" ).arg( name ),
            QMessageBox::Ok | QMessageBox::Cancel ) != QMessageBox::Ok )
            return;
    }

    Preset preset;
    preset.setGradient( m_model->gradient() );
    preset.setBackgroundColor( m_model->backgroundColor() );
    preset.setColorMapping( m_model->colorMapping() );

    map->insert( name, preset );

    QDialog::accept();
}

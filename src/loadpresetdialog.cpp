/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "loadpresetdialog.h"

#include <QPushButton>

#include "fraqtiveapplication.h"
#include "configurationdata.h"
#include "fractalmodel.h"

LoadPresetDialog::LoadPresetDialog( QWidget* parent ) : QDialog( parent ),
    m_model( NULL )
{
    m_ui.setupUi( this );

    m_ui.listDefault->setMap( fraqtive()->configuration()->defaultPresets() );
    m_ui.listUser->setMap( fraqtive()->configuration()->userPresets() );
    m_ui.listUser->setEditable( true );

    connect( m_ui.listDefault->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ),
        this, SLOT( defaultSelectionChanged() ) );
    connect( m_ui.listDefault->model(), SIGNAL( modelReset() ), this, SLOT( defaultSelectionChanged() ) );
    connect( m_ui.listUser->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ),
        this, SLOT( userSelectionChanged() ) );
    connect( m_ui.listUser->model(), SIGNAL( modelReset() ), this, SLOT( userSelectionChanged() ) );

    defaultSelectionChanged();
}

LoadPresetDialog::~LoadPresetDialog()
{
}

void LoadPresetDialog::setModel( FractalModel* model )
{
    m_model = model;
}

void LoadPresetDialog::on_tabWidget_currentChanged()
{
    if ( m_ui.tabWidget->currentIndex() == 0 )
        defaultSelectionChanged();
    else
        userSelectionChanged();
}

void LoadPresetDialog::on_listDefault_doubleClicked()
{
    accept();
}

void LoadPresetDialog::on_listUser_doubleClicked()
{
    accept();
}

void LoadPresetDialog::on_buttonBox_clicked( QAbstractButton* button )
{
    if ( button == m_ui.buttonBox->button( QDialogButtonBox::Apply ) ) {
        apply();
        m_ui.buttonBox->button( QDialogButtonBox::Apply )->setEnabled( false );
    }
}

void LoadPresetDialog::defaultSelectionChanged()
{
    QModelIndexList selection = m_ui.listDefault->selectionModel()->selectedIndexes();
    m_ui.buttonBox->button( QDialogButtonBox::Ok )->setEnabled( selection.count() == 1 );
    m_ui.buttonBox->button( QDialogButtonBox::Apply )->setEnabled( selection.count() == 1 );
}

void LoadPresetDialog::userSelectionChanged()
{
    QModelIndexList selection = m_ui.listUser->selectionModel()->selectedIndexes();
    m_ui.buttonBox->button( QDialogButtonBox::Ok )->setEnabled( selection.count() == 1 );
    m_ui.buttonBox->button( QDialogButtonBox::Apply )->setEnabled( selection.count() == 1 );
}

void LoadPresetDialog::accept()
{
    apply();

    QDialog::accept();
}

void LoadPresetDialog::apply()
{
    PresetListView* list;
    PresetMap* map;
    if ( m_ui.tabWidget->currentIndex() == 0 ) {
        list = m_ui.listDefault;
        map = fraqtive()->configuration()->defaultPresets();
    } else {
        list = m_ui.listUser;
        map = fraqtive()->configuration()->userPresets();
    }

    QModelIndexList selection = list->selectionModel()->selectedIndexes();

    if ( selection.count() == 1 ) {
        QString name = list->model()->data( selection[ 0 ] ).toString();
        Preset preset = map->value( name );
        m_model->setColorSettings( preset.gradient(), preset.backgroundColor(), preset.colorMapping() );
    }
}

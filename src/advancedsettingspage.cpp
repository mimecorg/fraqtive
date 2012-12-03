/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Michał Męciński
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

#include "advancedsettingspage.h"

#include <QMessageBox>

#include "fractalmodel.h"

AdvancedSettingsPage::AdvancedSettingsPage( QWidget* parent ) : QWidget( parent ),
    m_model( NULL ),
    m_loading( false )
{
    m_ui.setupUi( this );

    m_ui.sliderDepth->setScaledRange( 1.5, 4.0 );
    m_ui.sliderDetail->setScaledRange( 3.0, 0.0 );
    m_ui.sliderHeight->setScaledRange( 0.02, 0.42 );
    m_ui.sliderZoom->setScaledRange( 45.0, 10.0 );
}

AdvancedSettingsPage::~AdvancedSettingsPage()
{
}

void AdvancedSettingsPage::setModel( FractalModel* model )
{
    m_model = model;

    connect( m_model, SIGNAL( generatorSettingsChanged() ), this, SLOT( generatorSettingsChanged() ) );
    connect( m_model, SIGNAL( viewSettingsChanged() ), this, SLOT( viewSettingsChanged() ) );
    connect( m_model, SIGNAL( viewModeChanged() ), this, SLOT( viewModeChanged() ) );
}

void AdvancedSettingsPage::on_sliderDepth_valueChanged()
{
    saveGenerator();
}

void AdvancedSettingsPage::on_sliderDetail_valueChanged()
{
    saveGenerator();
}

void AdvancedSettingsPage::on_radioAANone_clicked()
{
    saveView();
}

void AdvancedSettingsPage::on_radioAALow_clicked()
{
    saveView();
}

void AdvancedSettingsPage::on_radioAAMedium_clicked()
{
    saveView();
}

void AdvancedSettingsPage::on_radioAAHigh_clicked()
{
    saveView();
}

void AdvancedSettingsPage::on_radioResLow_clicked()
{
    saveView();
}

void AdvancedSettingsPage::on_radioResMedium_clicked()
{
    saveView();
}

void AdvancedSettingsPage::on_radioResHigh_clicked()
{
    saveView();
}

void AdvancedSettingsPage::on_radioResVHigh_clicked()
{
    saveView();
}

void AdvancedSettingsPage::on_sliderHeight_valueChanged()
{
    saveView();
}

void AdvancedSettingsPage::on_sliderZoom_valueChanged()
{
    saveView();
}

void AdvancedSettingsPage::on_buttonRestore_clicked()
{
    m_model->loadDefaultGeneratorSettings();
    m_model->loadDefaultViewSettings();
}

void AdvancedSettingsPage::on_buttonStore_clicked()
{
    if ( QMessageBox::warning( this, tr( "Warning" ), tr( "Are you sure you want to store current advanced settings\nas the default settings?" ),
        QMessageBox::Ok | QMessageBox::Cancel ) == QMessageBox::Ok ) {
        m_model->saveDefaultGeneratorSettings();
        m_model->saveDefaultViewSettings();
        updateButtons();
    }
}

void AdvancedSettingsPage::generatorSettingsChanged()
{
    loadGenerator();
    updateButtons();
}

void AdvancedSettingsPage::viewSettingsChanged()
{
    loadView();
    updateButtons();
}

void AdvancedSettingsPage::viewModeChanged()
{
    bool isImage = ( m_model->viewMode() == ImageViewMode );
    bool isMesh = ( m_model->viewMode() == MeshViewMode );

    m_ui.radioAANone->setEnabled( isImage );
    m_ui.radioAALow->setEnabled( isImage );
    m_ui.radioAAMedium->setEnabled( isImage );
    m_ui.radioAAHigh->setEnabled( isImage );

    m_ui.sliderHeight->setEnabled( isMesh );
    m_ui.sliderZoom->setEnabled( isMesh );
    m_ui.radioResLow->setEnabled( isMesh );
    m_ui.radioResMedium->setEnabled( isMesh );
    m_ui.radioResHigh->setEnabled( isMesh );
    m_ui.radioResVHigh->setEnabled( isMesh );
}

void AdvancedSettingsPage::loadGenerator()
{
    m_loading = true;

    GeneratorSettings settings = m_model->generatorSettings();
    m_ui.sliderDepth->setScaledValue( settings.calculationDepth() );
    m_ui.sliderDetail->setScaledValue( settings.detailThreshold() );

    m_loading = false;
}

void AdvancedSettingsPage::saveGenerator()
{
    if ( m_loading )
        return;

    GeneratorSettings settings;
    settings.setCalculationDepth( m_ui.sliderDepth->scaledValue() );
    settings.setDetailThreshold( m_ui.sliderDetail->scaledValue() );
    m_model->setGeneratorSettings( settings );
}

void AdvancedSettingsPage::loadView()
{
    m_loading = true;

    ViewSettings settings = m_model->viewSettings();

    switch ( settings.antiAliasing() ) {
        case NoAntiAliasing:
            m_ui.radioAANone->setChecked( true );
            break;
        case LowAntiAliasing:
            m_ui.radioAALow->setChecked( true );
            break;
        case MediumAntiAliasing:
            m_ui.radioAAMedium->setChecked( true );
            break;
        case HighAntiAliasing:
            m_ui.radioAAHigh->setChecked( true );
            break;
    }

    switch ( settings.meshResolution() ) {
        case LowResolution:
            m_ui.radioResLow->setChecked( true );
            break;
        case MediumResolution:
            m_ui.radioResMedium->setChecked( true );
            break;
        case HighResolution:
            m_ui.radioResHigh->setChecked( true );
            break;
        case VeryHighResolution:
            m_ui.radioResVHigh->setChecked( true );
            break;
    }

    m_ui.sliderHeight->setScaledValue( settings.heightScale() );
    m_ui.sliderZoom->setScaledValue( settings.cameraZoom() );

    m_loading = false;
}

void AdvancedSettingsPage::saveView()
{
    if ( m_loading )
        return;

    ViewSettings settings;

    if ( m_ui.radioAANone->isChecked() )
        settings.setAntiAliasing( NoAntiAliasing );
    if ( m_ui.radioAALow->isChecked() )
        settings.setAntiAliasing( LowAntiAliasing );
    if ( m_ui.radioAAMedium->isChecked() )
        settings.setAntiAliasing( MediumAntiAliasing );
    if ( m_ui.radioAAHigh->isChecked() )
        settings.setAntiAliasing( HighAntiAliasing );

    if ( m_ui.radioResLow->isChecked() )
        settings.setMeshResolution( LowResolution );
    if ( m_ui.radioResMedium->isChecked() )
        settings.setMeshResolution( MediumResolution );
    if ( m_ui.radioResHigh->isChecked() )
        settings.setMeshResolution( HighResolution );
    if ( m_ui.radioResVHigh->isChecked() )
        settings.setMeshResolution( VeryHighResolution );

    settings.setHeightScale( m_ui.sliderHeight->scaledValue() );
    settings.setCameraZoom( m_ui.sliderZoom->scaledValue() );

    m_model->setViewSettings( settings );
}

void AdvancedSettingsPage::updateButtons()
{
    bool enabled = !m_model->hasDefaultGeneratorSettings() || !m_model->hasDefaultViewSettings();
    m_ui.buttonRestore->setEnabled( enabled );
    m_ui.buttonStore->setEnabled( enabled );
}

/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "advancedsettingspage.h"

#include <QMessageBox>

#include "fractalmodel.h"

AdvancedSettingsPage::AdvancedSettingsPage( QWidget* parent ) : QWidget( parent )
{
    m_ui.setupUi( this );

    m_ui.sliderDepth->setScaledRange( 1.5, 3.5 );
    m_ui.sliderDetail->setScaledRange( 3.0, 0.0 );
}

AdvancedSettingsPage::~AdvancedSettingsPage()
{
}

void AdvancedSettingsPage::setModel( FractalModel* model )
{
    m_model = model;

    connect( m_model, SIGNAL( generatorSettingsChanged() ), this, SLOT( generatorSettingsChanged() ) );
    connect( m_model, SIGNAL( viewSettingsChanged() ), this, SLOT( viewSettingsChanged() ) );
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

void AdvancedSettingsPage::loadGenerator()
{
    GeneratorSettings settings = m_model->generatorSettings();
    m_ui.sliderDepth->setScaledValue( settings.calculationDepth() );
    m_ui.sliderDetail->setScaledValue( settings.detailThreshold() );
}

void AdvancedSettingsPage::saveGenerator()
{
    GeneratorSettings settings;
    settings.setCalculationDepth( m_ui.sliderDepth->scaledValue() );
    settings.setDetailThreshold( m_ui.sliderDetail->scaledValue() );
    m_model->setGeneratorSettings( settings );
}

void AdvancedSettingsPage::loadView()
{
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
}

void AdvancedSettingsPage::saveView()
{
    ViewSettings settings;

    if ( m_ui.radioAANone->isChecked() )
        settings.setAntiAliasing( NoAntiAliasing );
    if ( m_ui.radioAALow->isChecked() )
        settings.setAntiAliasing( LowAntiAliasing );
    if ( m_ui.radioAAMedium->isChecked() )
        settings.setAntiAliasing( MediumAntiAliasing );
    if ( m_ui.radioAAHigh->isChecked() )
        settings.setAntiAliasing( HighAntiAliasing );

    m_model->setViewSettings( settings );
}

void AdvancedSettingsPage::updateButtons()
{
    bool enabled = !m_model->hasDefaultGeneratorSettings() || !m_model->hasDefaultViewSettings();
    m_ui.buttonRestore->setEnabled( enabled );
    m_ui.buttonStore->setEnabled( enabled );
}

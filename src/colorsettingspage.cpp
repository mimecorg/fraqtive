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

#include "colorsettingspage.h"

#include <QColorDialog>
#include <QMessageBox>

#include "fractalmodel.h"
#include "gradientdialog.h"
#include "loadpresetdialog.h"
#include "savepresetdialog.h"
#include "datafunctions.h"

ColorSettingsPage::ColorSettingsPage( QWidget* parent ) : QWidget( parent ),
    m_model( NULL )
{
    m_ui.setupUi( this );

    m_ui.sliderScale->setScaledRange( 0.02, 0.2 );
    m_ui.sliderOffset->setScaledRange( 0.0, 1.0 );
}

ColorSettingsPage::~ColorSettingsPage()
{
}

void ColorSettingsPage::setModel( FractalModel* model )
{
    m_model = model;

    connect( model, SIGNAL( gradientChanged() ), this, SLOT( gradientChanged() ) );
    connect( model, SIGNAL( backgroundColorChanged() ), this, SLOT( backgroundColorChanged() ) );
    connect( model, SIGNAL( colorMappingChanged() ), this, SLOT( colorMappingChanged() ) );
}

void ColorSettingsPage::on_buttonBackground_clicked()
{
    QColor color = QColorDialog::getColor( m_model->backgroundColor(), this );
    if ( color.isValid() )
        m_model->setBackgroundColor( color );
}

void ColorSettingsPage::on_buttonGradient_clicked()
{
    GradientDialog dialog( this );
    dialog.setGradient( m_model->gradient() );

    connect( &dialog, SIGNAL( applyGradient( const Gradient& ) ), this, SLOT( applyGradient( const Gradient& ) ) );

    dialog.exec();
}

void ColorSettingsPage::applyGradient( const Gradient& gradient )
{
    m_model->setGradient( gradient );
}

void ColorSettingsPage::on_sliderScale_valueChanged()
{
    saveMapping();
}

void ColorSettingsPage::on_sliderOffset_valueChanged()
{
    saveMapping();
}

void ColorSettingsPage::on_checkMirrored_toggled()
{
    saveMapping();
}

void ColorSettingsPage::on_checkReversed_toggled()
{
    saveMapping();
}

void ColorSettingsPage::on_buttonRestore_clicked()
{
    m_model->loadDefaultColorSettings();
}

void ColorSettingsPage::on_buttonStore_clicked()
{
    if ( QMessageBox::warning( this, tr( "Warning" ), tr( "Are you sure you want to store current color settings\nas the default settings?" ),
        QMessageBox::Ok | QMessageBox::Cancel ) == QMessageBox::Ok ) {
        m_model->saveDefaultColorSettings();
        updateButtons();
    }
}

void ColorSettingsPage::on_buttonLoad_clicked()
{
    LoadPresetDialog dialog( this );
    dialog.setModel( m_model );
    dialog.exec();
}

void ColorSettingsPage::on_buttonSave_clicked()
{
    SavePresetDialog dialog( this );
    dialog.setModel( m_model );
    dialog.exec();
}

void ColorSettingsPage::gradientChanged()
{
    loadGradient();
    updateButtons();
}

void ColorSettingsPage::backgroundColorChanged()
{
    loadBackground();
    updateButtons();
}

void ColorSettingsPage::colorMappingChanged()
{
    loadMapping();
    updateButtons();
}

void ColorSettingsPage::loadBackground()
{
    m_ui.widgetBackground->setBackgroundColor( m_model->backgroundColor() );
}

void ColorSettingsPage::loadGradient()
{
    QGradientStops stops = DataFunctions::calculateGradientStops( m_model->gradient() );
    m_ui.widgetGradient->setGradientStops( stops );
}

void ColorSettingsPage::loadMapping()
{
    ColorMapping mapping = m_model->colorMapping();
    m_ui.sliderScale->setScaledValue( mapping.scale() );
    m_ui.sliderOffset->setScaledValue( mapping.offset() );
    m_ui.checkMirrored->setChecked( mapping.isMirrored() );
    m_ui.checkReversed->setChecked( mapping.isReversed() );
}

void ColorSettingsPage::saveMapping()
{
    ColorMapping mapping;
    mapping.setScale( m_ui.sliderScale->scaledValue() );
    mapping.setOffset( m_ui.sliderOffset->scaledValue() );
    mapping.setMirrored( m_ui.checkMirrored->isChecked() );
    mapping.setReversed( m_ui.checkReversed->isChecked() );
    m_model->setColorMapping( mapping );
}

void ColorSettingsPage::updateButtons()
{
    bool enabled = !m_model->hasDefaultColorSettings();
    m_ui.buttonRestore->setEnabled( enabled );
    m_ui.buttonStore->setEnabled( enabled );
}

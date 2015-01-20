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

#include "generateimagedialog.h"

#include <QDesktopWidget>

#include "fraqtiveapplication.h"
#include "configurationdata.h"
#include "datafunctions.h"
#include "iconloader.h"

static void initializeDefaultSettings()
{
    static bool initialized = false;
    if ( !initialized ) {
        ConfigurationData* config = fraqtive()->configuration();

        if ( !config->contains( "ImageResolution" ) )
            config->setValue( "ImageResolution", QVariant::fromValue( QApplication::desktop()->screenGeometry().size() ) );
        if ( !config->contains( "ImageGeneratorSettings" ) )
            config->setValue( "ImageGeneratorSettings", QVariant::fromValue( DataFunctions::defaultGeneratorSettings() ) );
        if ( !config->contains( "ImageViewSettings" ) )
            config->setValue( "ImageViewSettings", QVariant::fromValue( DataFunctions::defaultViewSettings() ) );

        initialized = true;
    }
}

GenerateImageDialog::GenerateImageDialog( QWidget* parent ) : QDialog( parent )
{
    m_ui.setupUi( this );

    m_ui.promptPixmap->setPixmap( IconLoader::pixmap( "generate-image", 22 ) );
    m_ui.promptLabel->setText( tr( "Generate an image of the fractal:" ) );

    m_ui.promptLabel->setMinimumWidth( 350 );
    m_ui.promptLabel->setFixedHeight( m_ui.promptLabel->heightForWidth( 350 ) );

    setFixedHeight( sizeHint().height() );

    m_ui.sliderDepth->setScaledRange( 1.5, 4.0 );
    m_ui.sliderDetail->setScaledRange( 3.0, 0.0 );

    if ( QSysInfo::WordSize == 64 ) {
        m_ui.spinWidth->setMaximum( 30720 );
        m_ui.spinHeight->setMaximum( 17280 );
    }

    initializeDefaultSettings();

    ConfigurationData* config = fraqtive()->configuration();

    m_resolution = config->value( "ImageResolution" ).value<QSize>();

    m_ui.spinWidth->setValue( m_resolution.width() );
    m_ui.spinHeight->setValue( m_resolution.height() );

    m_generatorSettings = config->value( "ImageGeneratorSettings" ).value<GeneratorSettings>();

    m_ui.sliderDepth->setScaledValue( m_generatorSettings.calculationDepth() );
    m_ui.sliderDetail->setScaledValue( m_generatorSettings.detailThreshold() );

    m_viewSettings = config->value( "ImageViewSettings" ).value<ViewSettings>();

    switch ( m_viewSettings.antiAliasing() ) {
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

GenerateImageDialog::~GenerateImageDialog()
{
}

void GenerateImageDialog::accept()
{
    ConfigurationData* config = fraqtive()->configuration();

    m_resolution = QSize( m_ui.spinWidth->value(), m_ui.spinHeight->value() );

    config->setValue( "ImageResolution", QVariant::fromValue( m_resolution ) );

    m_generatorSettings.setCalculationDepth( m_ui.sliderDepth->scaledValue() );
    m_generatorSettings.setDetailThreshold( m_ui.sliderDetail->scaledValue() );

    config->setValue( "ImageGeneratorSettings", QVariant::fromValue( m_generatorSettings ) );

    if ( m_ui.radioAANone->isChecked() )
        m_viewSettings.setAntiAliasing( NoAntiAliasing );
    if ( m_ui.radioAALow->isChecked() )
        m_viewSettings.setAntiAliasing( LowAntiAliasing );
    if ( m_ui.radioAAMedium->isChecked() )
        m_viewSettings.setAntiAliasing( MediumAntiAliasing );
    if ( m_ui.radioAAHigh->isChecked() )
        m_viewSettings.setAntiAliasing( HighAntiAliasing );

    config->setValue( "ImageViewSettings", QVariant::fromValue( m_viewSettings ) );

    QDialog::accept();
}

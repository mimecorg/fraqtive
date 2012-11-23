/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "generateseriesdialog.h"

#include "fraqtiveapplication.h"
#include "configurationdata.h"
#include "datafunctions.h"
#include "iconloader.h"

static void initializeSeriesSettings()
{
    static bool initialized = false;
    if ( !initialized ) {
        ConfigurationData* config = fraqtive()->configuration();

        if ( !config->contains( "SeriesResolution" ) )
            config->setValue( "SeriesResolution", QVariant::fromValue( QSize( 640, 480 ) ) );
        if ( !config->contains( "SeriesGeneratorSettings" ) )
            config->setValue( "SeriesGeneratorSettings", QVariant::fromValue( DataFunctions::defaultGeneratorSettings() ) );
        if ( !config->contains( "SeriesViewSettings" ) )
            config->setValue( "SeriesViewSettings", QVariant::fromValue( DataFunctions::defaultViewSettings() ) );
        if ( !config->contains( "SeriesImages" ) )
            config->setValue( "SeriesImages", 100 );
        if ( !config->contains( "SeriesBlending" ) )
            config->setValue( "SeriesBlending", 0.0 );

        initialized = true;
    }
}

GenerateSeriesDialog::GenerateSeriesDialog( float zoomFactor, QWidget* parent ) : QDialog( parent )
{
    m_ui.setupUi( this );

    m_ui.promptPixmap->setPixmap( IconLoader::pixmap( "generate-series", 22 ) );
    m_ui.promptLabel->setText( tr( "Generate a series of images:" ) );

    m_ui.promptLabel->setMinimumWidth( 350 );
    m_ui.promptLabel->setFixedHeight( m_ui.promptLabel->heightForWidth( 350 ) );

    setFixedHeight( sizeHint().height() );

    m_ui.sliderDepth->setScaledRange( 1.5, 3.5 );
    m_ui.sliderDetail->setScaledRange( 3.0, 0.0 );

    int width = 0;
    QLabel* labels[ 3 ] = { m_ui.labelZoom, m_ui.labelAngle, m_ui.labelBlending };

    for ( int i = 0; i < 3; i++ )
        width = qMax( width, labels[ i ]->sizeHint().width() );

    m_ui.imageSpacer->changeSize( width + m_ui.animationLayout->horizontalSpacing(), 20, QSizePolicy::Fixed, QSizePolicy::Expanding );

    for ( int i = 0; i < 3; i++ )
        labels[ i ]->setFixedWidth( width );

    initializeSeriesSettings();

    ConfigurationData* config = fraqtive()->configuration();

    m_resolution = config->value( "SeriesResolution" ).value<QSize>();

    m_ui.spinWidth->setValue( m_resolution.width() );
    m_ui.spinHeight->setValue( m_resolution.height() );

    m_generatorSettings = config->value( "SeriesGeneratorSettings" ).value<GeneratorSettings>();

    m_ui.sliderDepth->setScaledValue( m_generatorSettings.calculationDepth() );
    m_ui.sliderDetail->setScaledValue( m_generatorSettings.detailThreshold() );

    m_viewSettings = config->value( "SeriesViewSettings" ).value<ViewSettings>();

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

    m_images = config->value( "SeriesImages" ).toInt();

    m_ui.spinImages->setValue( m_images );

    m_zoomFactor = zoomFactor + 0.45;
    m_angle = 0.0;

    m_ui.spinZoom->setValue( m_zoomFactor );
    m_ui.spinAngle->setValue( m_angle );

    m_blending = config->value( "SeriesBlending" ).toDouble();

    m_ui.spinBlending->setValue( m_blending );
}

GenerateSeriesDialog::~GenerateSeriesDialog()
{
}

void GenerateSeriesDialog::accept()
{
    ConfigurationData* config = fraqtive()->configuration();

    m_resolution = QSize( m_ui.spinWidth->value(), m_ui.spinHeight->value() );

    config->setValue( "SeriesResolution", QVariant::fromValue( m_resolution ) );

    m_generatorSettings.setCalculationDepth( m_ui.sliderDepth->scaledValue() );
    m_generatorSettings.setDetailThreshold( m_ui.sliderDetail->scaledValue() );

    config->setValue( "SeriesGeneratorSettings", QVariant::fromValue( m_generatorSettings ) );

    if ( m_ui.radioAANone->isChecked() )
        m_viewSettings.setAntiAliasing( NoAntiAliasing );
    if ( m_ui.radioAALow->isChecked() )
        m_viewSettings.setAntiAliasing( LowAntiAliasing );
    if ( m_ui.radioAAMedium->isChecked() )
        m_viewSettings.setAntiAliasing( MediumAntiAliasing );
    if ( m_ui.radioAAHigh->isChecked() )
        m_viewSettings.setAntiAliasing( HighAntiAliasing );

    config->setValue( "SeriesViewSettings", QVariant::fromValue( m_viewSettings ) );

    m_images = m_ui.spinImages->value();

    config->setValue( "SeriesImages", m_images );

    m_zoomFactor = m_ui.spinZoom->value();
    m_angle = m_ui.spinAngle->value();

    m_blending = m_ui.spinBlending->value();

    config->setValue( "SeriesBlending", m_blending );

    QDialog::accept();
}

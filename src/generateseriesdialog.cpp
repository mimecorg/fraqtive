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

#include "generateseriesdialog.h"

#include "fraqtiveapplication.h"
#include "fractalpresenter.h"
#include "fractalmodel.h"
#include "imageview.h"
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
        if ( !config->contains( "SeriesMultiSampling" ) )
            config->setValue( "SeriesMultiSampling", QVariant::fromValue( 0 ) );
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

GenerateSeriesDialog::GenerateSeriesDialog( QWidget* parent, const FractalModel* model ) : QDialog( parent ),
    m_presenter( NULL )
{
    m_ui.setupUi( this );

    m_ui.promptPixmap->setPixmap( IconLoader::pixmap( "generate-series", 22 ) );
    m_ui.promptLabel->setText( tr( "Generate a series of images:" ) );

    m_ui.promptLabel->setMinimumWidth( 350 );
    m_ui.promptLabel->setFixedHeight( m_ui.promptLabel->heightForWidth( 350 ) );

    setFixedHeight( sizeHint().height() );

    m_ui.sliderDepth->setScaledRange( 1.5, 4.0 );
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

    m_multiSampling = config->value( "SeriesMultiSampling" ).value<int>();

    switch ( m_multiSampling ) {
        case 0:
            m_ui.radioMSNone->setChecked( true );
            break;
        case 1:
            m_ui.radioMS2x2->setChecked( true );
            break;
        case 2:
            m_ui.radioMS4x4->setChecked( true );
            break;
        case 3:
            m_ui.radioMS8x8->setChecked( true );
            break;
    }

    updateMaximumSize();

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

    m_fractalType = model->fractalType();
    m_endPosition = model->position();

    m_zoomFactor = m_endPosition.zoomFactor() + 0.45;
    m_angle = 0.0;

    m_ui.spinZoom->setValue( m_zoomFactor );
    m_ui.spinAngle->setValue( m_angle );

    m_blending = config->value( "SeriesBlending" ).toDouble();

    m_ui.spinBlending->setValue( m_blending );

    m_presenter = new FractalPresenter( this );

    ImageView* view = new ImageView( m_ui.viewContainer, m_presenter );
    m_ui.viewContainer->setView( view );

    m_presenter->setView( view );
    m_presenter->setPriority( 1 );

    m_presenter->setColorSettings( model->gradient(), model->backgroundColor(), model->colorMapping() );

    updatePosition();
    updateSettings();

    m_presenter->setEnabled( true );
}

GenerateSeriesDialog::~GenerateSeriesDialog()
{
}

void GenerateSeriesDialog::accept()
{
    ConfigurationData* config = fraqtive()->configuration();

    m_resolution = QSize( m_ui.spinWidth->value(), m_ui.spinHeight->value() );

    config->setValue( "SeriesResolution", QVariant::fromValue( m_resolution ) );

    if ( m_ui.radioMSNone->isChecked() )
        m_multiSampling = 0;
    else if ( m_ui.radioMS2x2->isChecked() )
        m_multiSampling = 1;
    else if ( m_ui.radioMS4x4->isChecked() )
        m_multiSampling = 2;
    else if ( m_ui.radioMS8x8->isChecked() )
        m_multiSampling = 3;

    config->setValue( "SeriesMultiSampling", QVariant::fromValue( m_multiSampling ) );

    config->setValue( "SeriesGeneratorSettings", QVariant::fromValue( m_generatorSettings ) );
    config->setValue( "SeriesViewSettings", QVariant::fromValue( m_viewSettings ) );

    m_images = m_ui.spinImages->value();

    config->setValue( "SeriesImages", m_images );

    m_zoomFactor = m_ui.spinZoom->value();
    m_angle = m_ui.spinAngle->value();

    m_blending = m_ui.spinBlending->value();

    config->setValue( "SeriesBlending", m_blending );

    QDialog::accept();
}

void GenerateSeriesDialog::on_sliderDepth_valueChanged()
{
    updateSettings();
}

void GenerateSeriesDialog::on_sliderDetail_valueChanged()
{
    updateSettings();
}

void GenerateSeriesDialog::on_radioAANone_clicked()
{
    updateSettings();
}

void GenerateSeriesDialog::on_radioAALow_clicked()
{
    updateSettings();
}

void GenerateSeriesDialog::on_radioAAMedium_clicked()
{
    updateSettings();
}

void GenerateSeriesDialog::on_radioAAHigh_clicked()
{
    updateSettings();
}

void GenerateSeriesDialog::on_radioMSNone_clicked()
{
    updateMaximumSize();
}

void GenerateSeriesDialog::on_radioMS2x2_clicked()
{
    updateMaximumSize();
}

void GenerateSeriesDialog::on_radioMS4x4_clicked()
{
    updateMaximumSize();
}

void GenerateSeriesDialog::on_radioMS8x8_clicked()
{
    updateMaximumSize();
}

void GenerateSeriesDialog::on_spinZoom_valueChanged()
{
    updatePosition();
}

void GenerateSeriesDialog::on_spinAngle_valueChanged()
{
    updatePosition();
}

void GenerateSeriesDialog::on_animSlider_valueChanged()
{
    updatePosition();
}

void GenerateSeriesDialog::updateMaximumSize()
{
    int width, height;
    if ( QSysInfo::WordSize == 64 ) {
        width = 30720;
        height = 17280;
    } else {
        width = 8000;
        height = 8000;
    }

    int multiSampling = 0;
    if ( m_ui.radioMS2x2->isChecked() )
        multiSampling = 1;
    else if ( m_ui.radioMS4x4->isChecked() )
        multiSampling = 2;
    else if ( m_ui.radioMS8x8->isChecked() )
        multiSampling = 3;

    m_ui.spinWidth->setMaximum( width >> multiSampling );
    m_ui.spinHeight->setMaximum( height >> multiSampling );
}

void GenerateSeriesDialog::updatePosition()
{
    if ( !m_presenter )
        return;

    Position position = m_endPosition;

    double zoomTo = position.zoomFactor();
    double zoomFrom = zoomTo - m_ui.spinZoom->value();
    double angleTo = position.angle();
    double angleFrom = angleTo - m_ui.spinAngle->value();

    double a = (double)m_ui.animSlider->value() / 100.0;

    position.setZoomFactor( zoomFrom + a * ( zoomTo - zoomFrom ) );
    position.setAngle( angleFrom + a * ( angleTo - angleFrom ) );

    m_presenter->setParameters( m_fractalType, position );
}

void GenerateSeriesDialog::updateSettings()
{
    if ( !m_presenter )
        return;

    m_generatorSettings.setCalculationDepth( m_ui.sliderDepth->scaledValue() );
    m_generatorSettings.setDetailThreshold( m_ui.sliderDetail->scaledValue() );

    if ( m_ui.radioAANone->isChecked() )
        m_viewSettings.setAntiAliasing( NoAntiAliasing );
    if ( m_ui.radioAALow->isChecked() )
        m_viewSettings.setAntiAliasing( LowAntiAliasing );
    if ( m_ui.radioAAMedium->isChecked() )
        m_viewSettings.setAntiAliasing( MediumAntiAliasing );
    if ( m_ui.radioAAHigh->isChecked() )
        m_viewSettings.setAntiAliasing( HighAntiAliasing );

    m_presenter->setGeneratorSettings( m_generatorSettings );
    m_presenter->setViewSettings( m_viewSettings );
}

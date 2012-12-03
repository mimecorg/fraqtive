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

#include "parameterspage.h"

#include "fractalmodel.h"
#include "fractaltypedialog.h"
#include "loadbookmarkdialog.h"
#include "savebookmarkdialog.h"

ParametersPage::ParametersPage( QWidget* parent ) : QWidget( parent ),
    m_model( NULL ),
    m_loading( false )
{
    m_ui.setupUi( this );
}

ParametersPage::~ParametersPage()
{
}

void ParametersPage::setModel( FractalModel* model )
{
    m_model = model;

    connect( model, SIGNAL( fractalTypeChanged() ), this, SLOT( fractalTypeChanged() ) );
    connect( model, SIGNAL( positionChanged() ), this, SLOT( positionChanged() ) );
    connect( model, SIGNAL( trackingChanged() ), this, SLOT( trackingChanged() ) );
    connect( model, SIGNAL( hoveringChanged() ), this, SLOT( hoveringChanged() ) );
}

void ParametersPage::on_buttonType_clicked()
{
    FractalTypeDialog dialog( this, m_model );

    if ( dialog.exec() == QDialog::Accepted )
        m_model->setParameters( dialog.fractalType(), dialog.position() );
}

void ParametersPage::on_buttonRestore_clicked()
{
    m_model->setDefaultPosition();
}

void ParametersPage::on_buttonLoad_clicked()
{
    LoadBookmarkDialog dialog( this );
    dialog.setModel( m_model );
    dialog.exec();
}

void ParametersPage::on_buttonSave_clicked()
{
    SaveBookmarkDialog dialog( this );
    dialog.setModel( m_model );
    dialog.exec();
}

void ParametersPage::on_editJuliaX_valueChanged()
{
    saveParameters();
}

void ParametersPage::on_editJuliaY_valueChanged()
{
    saveParameters();
}

void ParametersPage::on_editPositionX_valueChanged()
{
    savePosition();
}

void ParametersPage::on_editPositionY_valueChanged()
{
    savePosition();
}

void ParametersPage::on_spinZoom_valueChanged()
{
    savePosition();
}

void ParametersPage::on_spinAngle_valueChanged()
{
    savePosition();
}

void ParametersPage::fractalTypeChanged()
{
    loadFractalType();
    loadParameters();
}

void ParametersPage::positionChanged()
{
    loadPosition();
}

void ParametersPage::trackingChanged()
{
    loadPosition();
}

void ParametersPage::hoveringChanged()
{
    loadParameters();
}

void ParametersPage::loadFractalType()
{
    FractalType type = m_model->fractalType();

    QString fractal;
    if ( type.fractal() == MandelbrotFractal )
        fractal = tr( "Mandelbrot" );
    else if ( type.fractal() == JuliaFractal )
        fractal = tr( "Julia" );

    QString variant;
    switch ( type.variant() ) {
        case GeneratorCore::NormalVariant:
            variant = tr( "Normal" );
            break;
        case GeneratorCore::ConjugateVariant:
            variant = tr( "Conjugate" );
            break;
        case GeneratorCore::AbsoluteVariant:
            variant = tr( "Absolute" );
            break;
        case GeneratorCore::AbsoluteImVariant:
            variant = tr( "Absolute Im." );
            break;
    }

    QString exponent;
    if ( type.exponentType() == IntegralExponent )
        exponent = QString::number( type.integralExponent() );
    else if ( type.exponentType() == RealExponent )
        exponent = QString::number( type.realExponent(), 'f', 2 );

    m_ui.editType->setText( QString( "%1 (%2), N=%3" ).arg( fractal, variant, exponent ) );
}

void ParametersPage::loadParameters()
{
    m_loading = true;

    if ( m_model->isHovering() ) {
        FractalType type = m_model->hoveringFractalType();
        m_ui.editJuliaX->setReadOnly( true );
        m_ui.editJuliaY->setReadOnly( true );
        m_ui.editJuliaX->setValue( type.parameter().x() );
        m_ui.editJuliaY->setValue( type.parameter().y() );
    } else {
        FractalType type = m_model->fractalType();
        if ( type.fractal() == JuliaFractal ) {
            m_ui.editJuliaX->setReadOnly( false );
            m_ui.editJuliaY->setReadOnly( false );
            m_ui.editJuliaX->setValue( type.parameter().x() );
            m_ui.editJuliaY->setValue( type.parameter().y() );
        } else {
            m_ui.editJuliaX->setReadOnly( true );
            m_ui.editJuliaY->setReadOnly( true );
            m_ui.editJuliaX->clear();
            m_ui.editJuliaY->clear();
        }
    }

    m_loading = false;
}

void ParametersPage::saveParameters()
{
    if ( !m_loading ) {
        FractalType type = m_model->fractalType();
        if ( type.fractal() == JuliaFractal ) {
            type.setParameter( QPointF( m_ui.editJuliaX->value(), m_ui.editJuliaY->value() ) );
            m_model->setFractalType( type );
        }
    }
}

void ParametersPage::loadPosition()
{
    m_loading = true;

    Position position = m_model->isTracking() ? m_model->trackingPosition() : m_model->position();

    m_ui.editPositionX->setValue( position.center().x() );
    m_ui.editPositionY->setValue( position.center().y() );
    m_ui.spinZoom->setValue( position.zoomFactor() );
    m_ui.spinAngle->setValue( position.angle() );

    m_ui.buttonRestore->setEnabled( !m_model->hasDefaultPosition() );

    m_loading = false;
}

void ParametersPage::savePosition()
{
    if ( !m_loading && !m_model->isTracking() ) {
        Position position;
        position.setCenter( QPointF( m_ui.editPositionX->value(), m_ui.editPositionY->value() ) );
        position.setZoomFactor( m_ui.spinZoom->value() );
        position.setAngle( m_ui.spinAngle->value() );
        m_model->setPosition( position );
    }
}

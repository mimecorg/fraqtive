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

#include "fractaltypewidget.h"

FractalTypeWidget::FractalTypeWidget( QWidget* parent ) : QWidget( parent ),
    m_loading( false )
{
    m_ui.setupUi( this );

    saveData();
}

FractalTypeWidget::~FractalTypeWidget()
{
}

void FractalTypeWidget::setFractalType( const FractalType& type )
{
    if ( m_type != type ) {
        m_type = type;
        loadData();
        emit fractalTypeChanged();
    }
}

void FractalTypeWidget::on_radioMandelbrot_clicked()
{
    saveData();
}

void FractalTypeWidget::on_radioJulia_clicked()
{
    saveData();
}

void FractalTypeWidget::on_editJuliaX_valueChanged()
{
    saveData();
}

void FractalTypeWidget::on_editJuliaY_valueChanged()
{
    saveData();
}

void FractalTypeWidget::on_radioNormal_clicked()
{
    saveData();
}

void FractalTypeWidget::on_radioConjugate_clicked()
{
    saveData();
}

void FractalTypeWidget::on_radioAbsolute_clicked()
{
    saveData();
}

void FractalTypeWidget::on_radioAbsoluteIm_clicked()
{
    saveData();
}

void FractalTypeWidget::on_radioIntegral_clicked()
{
    saveData();
}

void FractalTypeWidget::on_radioReal_clicked()
{
    saveData();
}

void FractalTypeWidget::on_spinIntegral_valueChanged()
{
    saveData();
}

void FractalTypeWidget::on_spinReal_valueChanged()
{
    saveData();
}

void FractalTypeWidget::loadData()
{
    m_loading = true;

    switch ( m_type.fractal() ) {
        case MandelbrotFractal:
            m_ui.radioMandelbrot->setChecked( true );
            break;
        case JuliaFractal:
            m_ui.radioJulia->setChecked( true );
            m_ui.editJuliaX->setValue( m_type.parameter().x() );
            m_ui.editJuliaY->setValue( m_type.parameter().y() );
            break;
    }

    switch ( m_type.variant() ) {
        case GeneratorCore::NormalVariant:
            m_ui.radioNormal->setChecked( true );
            break;
        case GeneratorCore::ConjugateVariant:
            m_ui.radioConjugate->setChecked( true );
            break;
        case GeneratorCore::AbsoluteVariant:
            m_ui.radioAbsolute->setChecked( true );
            break;
        case GeneratorCore::AbsoluteImVariant:
            m_ui.radioAbsoluteIm->setChecked( true );
            break;
    }

    switch ( m_type.exponentType() ) {
        case IntegralExponent:
            m_ui.radioIntegral->setChecked( true );
            m_ui.spinIntegral->setValue( m_type.integralExponent() );
            break;
        case RealExponent:
            m_ui.radioReal->setChecked( true );
            m_ui.spinReal->setValue( m_type.realExponent() );
            break;
    }

    m_loading = false;
}

void FractalTypeWidget::saveData()
{
    if ( m_loading )
        return;

    if ( m_ui.radioMandelbrot->isChecked() ) {
        m_type.setFractal( MandelbrotFractal );
    } else if ( m_ui.radioJulia->isChecked() ) {
        m_type.setFractal( JuliaFractal );
        m_type.setParameter( QPointF( m_ui.editJuliaX->value(), m_ui.editJuliaY->value() ) );
    }

    if ( m_ui.radioNormal->isChecked() )
        m_type.setVariant( GeneratorCore::NormalVariant );
    else if ( m_ui.radioConjugate->isChecked() )
        m_type.setVariant( GeneratorCore::ConjugateVariant );
    else if ( m_ui.radioAbsolute->isChecked() )
        m_type.setVariant( GeneratorCore::AbsoluteVariant );
    else if ( m_ui.radioAbsoluteIm->isChecked() )
        m_type.setVariant( GeneratorCore::AbsoluteImVariant );

    if ( m_ui.radioIntegral->isChecked() ) {
        m_type.setExponentType( IntegralExponent );
        m_type.setIntegralExponent( m_ui.spinIntegral->value() );
    } else if ( m_ui.radioReal->isChecked() ) {
        m_type.setExponentType( RealExponent );
        m_type.setRealExponent( m_ui.spinReal->value() );
    }

    QString variable;
    switch ( m_type.variant() ) {
        case GeneratorCore::NormalVariant:
            variable = "Z<sub>n</sub>";
            break;
        case GeneratorCore::ConjugateVariant:
            variable = "<span style=\"text-decoration:overline\">Z</span><sub>n</sub>";
            break;
        case GeneratorCore::AbsoluteVariant:
            variable = "(|Re(Z<sub>n</sub>)|+i|Im(Z<sub>n</sub>)|)";
            break;
        case GeneratorCore::AbsoluteImVariant:
            variable = "(Re(Z<sub>n</sub>)+i|Im(Z<sub>n</sub>)|)";
            break;
    }

    QString exponent;
    switch ( m_type.exponentType() ) {
        case IntegralExponent:
            exponent = QString::number( m_type.integralExponent() );
            break;
        case RealExponent:
            exponent = QString::number( m_type.realExponent(), 'g', 4 );
            break;
    }

    m_ui.valueFormula->setText( QString( "Z<sub>n+1</sub> = %1<sup>%2</sup> + C" ).arg( variable, exponent ) );

    emit fractalTypeChanged();
}

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

#include "gradienteditor.h"

#include "datafunctions.h"

GradientEditor::GradientEditor( QWidget* parent ) : QWidget( parent )
{
    m_ui.setupUi( this );

    m_ui.shadeRed->setShadeColor( QColor( 255, 0, 0 ) );
    m_ui.shadeGreen->setShadeColor( QColor( 0, 255, 0 ) );
    m_ui.shadeBlue->setShadeColor( QColor( 0, 0, 255 ) );
}

GradientEditor::~GradientEditor()
{
}

void GradientEditor::setGradient( const Gradient& gradient )
{
    m_ui.shadeRed->setPoints( gradient.red() );
    m_ui.shadeGreen->setPoints( gradient.green() );
    m_ui.shadeBlue->setPoints( gradient.blue() );
}

Gradient GradientEditor::gradient() const
{
    Gradient gradient;
    gradient.setRed( m_ui.shadeRed->points() );
    gradient.setGreen( m_ui.shadeGreen->points() );
    gradient.setBlue( m_ui.shadeBlue->points() );
    return gradient;
}

void GradientEditor::on_shadeRed_pointsChanged()
{
    updateGradient();
}

void GradientEditor::on_shadeGreen_pointsChanged()
{
    updateGradient();
}

void GradientEditor::on_shadeBlue_pointsChanged()
{
    updateGradient();
}

void GradientEditor::updateGradient()
{
    QGradientStops stops = DataFunctions::calculateGradientStops( gradient() );
    m_ui.gradient->setGradientStops( stops );

    emit gradientChanged();
}

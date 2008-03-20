/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "gradientwidget.h"

#include "datafunctions.h"

GradientWidget::GradientWidget( QWidget* parent ) : QWidget( parent )
{
    m_ui.setupUi( this );

    m_ui.shadeRed->setShadeColor( QColor( 255, 0, 0 ) );
    m_ui.shadeGreen->setShadeColor( QColor( 0, 255, 0 ) );
    m_ui.shadeBlue->setShadeColor( QColor( 0, 0, 255 ) );
}

GradientWidget::~GradientWidget()
{
}

void GradientWidget::setGradient( const Gradient& gradient )
{
    m_ui.shadeRed->setPoints( gradient.red() );
    m_ui.shadeGreen->setPoints( gradient.green() );
    m_ui.shadeBlue->setPoints( gradient.blue() );
}

Gradient GradientWidget::gradient() const
{
    Gradient gradient;
    gradient.setRed( m_ui.shadeRed->points() );
    gradient.setGreen( m_ui.shadeGreen->points() );
    gradient.setBlue( m_ui.shadeBlue->points() );
    return gradient;
}

void GradientWidget::on_shadeRed_pointsChanged()
{
    updateGradient();
}

void GradientWidget::on_shadeGreen_pointsChanged()
{
    updateGradient();
}

void GradientWidget::on_shadeBlue_pointsChanged()
{
    updateGradient();
}

void GradientWidget::updateGradient()
{
    QGradientStops stops = DataFunctions::calculateGradientStops( gradient() );
    m_ui.gradient->setGradientStops( stops );

    emit gradientChanged();
}

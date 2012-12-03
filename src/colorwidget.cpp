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

#include "colorwidget.h"

#include <QPainter>
#include <QPaintEvent>

ColorWidget::ColorWidget( QWidget* parent ) : QWidget( parent )
{
}

ColorWidget::~ColorWidget()
{
}

void ColorWidget::setBackgroundColor( const QColor& color )
{
    m_color = color;
    m_stops.clear();
    update();
}

void ColorWidget::setGradientStops( const QGradientStops& stops )
{
    m_color = QColor();
    m_stops = stops;
    update();
}

void ColorWidget::paintEvent( QPaintEvent* e )
{
    QPainter painter( this );

    QRect rect = e->rect().adjusted( 0, 2, 0, -2 );

    if ( m_color.isValid() ) {
        painter.setBrush( m_color );
    } else if ( !m_stops.isEmpty() ) {
        QLinearGradient gradient( rect.topLeft(), rect.topRight() );
        gradient.setStops( m_stops );
        painter.setBrush( gradient );
    }

    painter.setPen( Qt::NoPen );

    painter.drawRect( rect );
}

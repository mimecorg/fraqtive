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

#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>

class ColorWidget : public QWidget
{
    Q_OBJECT
public:
    ColorWidget( QWidget* parent );
    ~ColorWidget();

public:
    void setBackgroundColor( const QColor& color );
    void setGradientStops( const QGradientStops& stops );

protected: // overrides
    void paintEvent( QPaintEvent* e );

private:
    QColor m_color;
    QGradientStops m_stops;
};

#endif

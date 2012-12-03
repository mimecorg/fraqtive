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

#ifndef DOUBLESLIDER_H
#define DOUBLESLIDER_H

#include <QSlider>

class DoubleSlider : public QSlider
{
    Q_OBJECT
public:
    DoubleSlider( QWidget* parent );
    ~DoubleSlider();

public:
    void setScaledRange( double min, double max );

    void setScaledValue( double value );
    double scaledValue() const;

private:
    double m_minimum;
    double m_maximum;
};

#endif

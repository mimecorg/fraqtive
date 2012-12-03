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

#include "doubleslider.h"

DoubleSlider::DoubleSlider( QWidget* parent ) : QSlider( parent ),
    m_minimum( 0.0 ),
    m_maximum( 1.0 )
{
}

DoubleSlider::~DoubleSlider()
{
}

void DoubleSlider::setScaledRange( double min, double max )
{
    m_minimum = min;
    m_maximum = max;
}

void DoubleSlider::setScaledValue( double value )
{
    setValue( minimum() + (int)( ( value - m_minimum ) / ( m_maximum - m_minimum ) * ( maximum() - minimum() ) + 0.5 ) );
}

double DoubleSlider::scaledValue() const
{
    return m_minimum + ( m_maximum - m_minimum ) * (double)( value() - minimum() ) / (double)( maximum() - minimum() );
}

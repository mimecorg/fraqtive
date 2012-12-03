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

#include "fractaldata.h"

FractalData::FractalData() :
    m_buffer( NULL ),
    m_owner( false ),
    m_stride( 0 )
{
}

FractalData::~FractalData()
{
    if ( m_owner )
        delete[] m_buffer;
}

void FractalData::clear()
{
    if ( m_owner )
        delete[] m_buffer;

    m_buffer = NULL;
    m_owner = false;
    m_stride = 0;
    m_size = QSize();
    m_validRegions.clear();
}

void FractalData::setBuffer( double* buffer, int stride, const QSize& size )
{
    if ( m_owner )
        delete[] m_buffer;

    m_buffer = buffer;
    m_owner = false;
    m_stride = stride;
    m_size = size;
    m_validRegions.clear();
}

void FractalData::transferBuffer( double* buffer, int stride, const QSize& size )
{
    if ( m_owner )
        delete[] m_buffer;

    m_buffer = buffer;
    m_owner = true;
    m_stride = stride;
    m_size = size;
    m_validRegions.clear();
}

void FractalData::setValidRegion( const QRect& region )
{
    m_validRegions.clear();
    m_validRegions.append( region );
}

void FractalData::setValidRegions( const QList<QRect>& regions )
{
    m_validRegions = regions;
}

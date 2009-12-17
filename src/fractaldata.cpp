/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
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

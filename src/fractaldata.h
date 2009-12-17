/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef FRACTALDATA_H
#define FRACTALDATA_H

#include "datastructures.h"

class FractalData
{
public:
    FractalData();
    ~FractalData();

public:
    void clear();
    void setBuffer( double* buffer, int stride, const QSize& size );
    void transferBuffer( double* buffer, int stride, const QSize& size );

    bool isEmpty() const { return !m_buffer; }
    const double* buffer() const { return m_buffer; }
    int stride() const { return m_stride; }
    QSize size() const { return m_size; }

    void setValidRegion( const QRect& region );
    void setValidRegions( const QList<QRect>& regions );

    QList<QRect> validRegions() const { return m_validRegions; }

private:
    double* m_buffer;
    bool m_owner;

    int m_stride;
    QSize m_size;

    QList<QRect> m_validRegions;
};

#endif

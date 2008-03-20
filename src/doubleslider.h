/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
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

/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef GRADIENTWIDGET_H
#define GRADIENTWIDGET_H

#include <QWidget>

#include "ui_gradientwidget.h"

class Gradient;

class GradientWidget : public QWidget
{
    Q_OBJECT
public:
    GradientWidget( QWidget* parent );
    ~GradientWidget();

public:
    void setGradient( const Gradient& gradient );
    Gradient gradient() const;

signals:
    void gradientChanged();

private slots:
    void on_shadeRed_pointsChanged();
    void on_shadeGreen_pointsChanged();
    void on_shadeBlue_pointsChanged();

private:
    void updateGradient();

private:
    Ui::GradientWidget m_ui;
};

#endif

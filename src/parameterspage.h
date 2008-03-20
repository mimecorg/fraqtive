/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef PARAMETERSPAGE_H
#define PARAMETERSPAGE_H

#include <QWidget>

#include "ui_parameterspage.h"

class FractalModel;

class ParametersPage : public QWidget
{
    Q_OBJECT
public:
    ParametersPage( QWidget* parent );
    ~ParametersPage();

public:
    void setModel( FractalModel* model );

private slots:
    void on_buttonType_clicked();
    void on_buttonRestore_clicked();

    void on_editJuliaX_valueChanged();
    void on_editJuliaY_valueChanged();
    void on_editPositionX_valueChanged();
    void on_editPositionY_valueChanged();
    void on_spinZoom_valueChanged();
    void on_spinAngle_valueChanged();

    void fractalTypeChanged();
    void positionChanged();
    void trackingChanged();
    void hoveringChanged();

private:
    void loadFractalType();

    void loadParameters();
    void saveParameters();

    void loadPosition();
    void savePosition();

private:
    Ui::ParametersPage m_ui;

    FractalModel* m_model;

    bool m_loading;
};

#endif

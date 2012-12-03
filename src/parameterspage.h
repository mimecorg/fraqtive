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
    void on_buttonLoad_clicked();
    void on_buttonSave_clicked();

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

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

#ifndef ADVANCEDSETTINGSPAGE_H
#define ADVANCEDSETTINGSPAGE_H

#include <QWidget>

#include "ui_advancedsettingspage.h"

class FractalModel;

class AdvancedSettingsPage : public QWidget
{
    Q_OBJECT
public:
    AdvancedSettingsPage( QWidget* parent );
    ~AdvancedSettingsPage();

public:
    void setModel( FractalModel* model );

private slots:
    void on_sliderDepth_valueChanged();
    void on_sliderDetail_valueChanged();
    void on_radioAANone_clicked();
    void on_radioAALow_clicked();
    void on_radioAAMedium_clicked();
    void on_radioAAHigh_clicked();
    void on_radioResLow_clicked();
    void on_radioResMedium_clicked();
    void on_radioResHigh_clicked();
    void on_radioResVHigh_clicked();
    void on_sliderHeight_valueChanged();
    void on_sliderZoom_valueChanged();

    void on_buttonRestore_clicked();
    void on_buttonStore_clicked();

    void generatorSettingsChanged();
    void viewSettingsChanged();
    void viewModeChanged();

private:
    void loadGenerator();
    void saveGenerator();

    void loadView();
    void saveView();

    void updateButtons();

private:
    Ui::AdvancedSettingsPage m_ui;

    FractalModel* m_model;

    bool m_loading;
};

#endif

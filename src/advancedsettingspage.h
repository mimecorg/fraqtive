/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
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

    void on_buttonRestore_clicked();
    void on_buttonStore_clicked();

    void generatorSettingsChanged();
    void viewSettingsChanged();

private:
    void loadGenerator();
    void saveGenerator();

    void loadView();
    void saveView();

    void updateButtons();

private:
    Ui::AdvancedSettingsPage m_ui;

    FractalModel* m_model;
};

#endif

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

#ifndef COLORSETTINGSPAGE_H
#define COLORSETTINGSPAGE_H

#include <QWidget>

#include "ui_colorsettingspage.h"

class FractalModel;
class Gradient;

class ColorSettingsPage : public QWidget
{
    Q_OBJECT
public:
    ColorSettingsPage( QWidget* parent );
    ~ColorSettingsPage();

public:
    void setModel( FractalModel* model );

private slots:
    void on_buttonBackground_clicked();
    void on_buttonGradient_clicked();

    void on_sliderScale_valueChanged();
    void on_sliderOffset_valueChanged();
    void on_checkMirrored_toggled();
    void on_checkReversed_toggled();

    void on_buttonRestore_clicked();
    void on_buttonStore_clicked();
    void on_buttonLoad_clicked();
    void on_buttonSave_clicked();

    void gradientChanged();
    void backgroundColorChanged();
    void colorMappingChanged();

    void applyGradient( const Gradient& gradient );

private:
    void loadBackground();
    void loadGradient();

    void loadMapping();
    void saveMapping();

    void updateButtons();

private:
    Ui::ColorSettingsPage m_ui;

    FractalModel* m_model;
};

#endif

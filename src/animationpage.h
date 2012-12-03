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

#ifndef ANIMATIONPAGE_H
#define ANIMATIONPAGE_H

#include <QWidget>

#include "ui_animationpage.h"

class FractalModel;

class AnimationPage : public QWidget
{
    Q_OBJECT
public:
    AnimationPage( QWidget* parent );
    ~AnimationPage();

public:
    void setModel( FractalModel* model );

private slots:
    void on_sliderScrolling_valueChanged();
    void on_sliderRotation_valueChanged();

    void on_stopScrolling_clicked();
    void on_stopRotation_clicked();

    void animationSettingsChanged();
    void viewModeChanged();

private:
    void loadSettings();
    void saveSettings();

    void updateButtons();

private:
    Ui::AnimationPage m_ui;

    FractalModel* m_model;

    bool m_loading;
};

#endif

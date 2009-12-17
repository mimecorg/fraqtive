/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
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

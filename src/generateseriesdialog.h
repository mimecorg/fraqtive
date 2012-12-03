/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef GENERATESERIESDIALOG_H
#define GENERATESERIESDIALOG_H

#include <QDialog>

#include "ui_generateseriesdialog.h"
#include "datastructures.h"

class FractalModel;
class FractalPresenter;

class GenerateSeriesDialog : public QDialog
{
    Q_OBJECT
public:
    GenerateSeriesDialog( QWidget* parent, const FractalModel* model );
    ~GenerateSeriesDialog();

public:
    QSize resolution() const { return m_resolution; }
    GeneratorSettings generatorSettings() const { return m_generatorSettings; }
    ViewSettings viewSettings() const { return m_viewSettings; }

    int images() const { return m_images; }

    double zoomFactor() const { return m_zoomFactor; }
    double angle() const { return m_angle; }

    double blending() const { return m_blending; }

public: // overrides
    void accept();

private slots:
    void on_sliderDepth_valueChanged();
    void on_sliderDetail_valueChanged();
    void on_radioAANone_clicked();
    void on_radioAALow_clicked();
    void on_radioAAMedium_clicked();
    void on_radioAAHigh_clicked();
    void on_spinZoom_valueChanged();
    void on_spinAngle_valueChanged();
    void on_animSlider_valueChanged();

private:
    void updatePosition();
    void updateSettings();

private:
    Ui::GenerateSeriesDialog m_ui;

    FractalPresenter* m_presenter;

    FractalType m_fractalType;
    Position m_endPosition;

    QSize m_resolution;
    GeneratorSettings m_generatorSettings;
    ViewSettings m_viewSettings;

    int m_images;
    double m_zoomFactor;
    double m_angle;
    double m_blending;
};

#endif

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

#ifndef FRACTALTYPEWIDGET_H
#define FRACTALTYPEWIDGET_H

#include <QWidget>

#include "ui_fractaltypewidget.h"
#include "datastructures.h"

class FractalTypeWidget : public QWidget
{
    Q_OBJECT
public:
    FractalTypeWidget( QWidget* parent );
    ~FractalTypeWidget();

public:
    void setFractalType( const FractalType& type );
    FractalType fractalType() const { return m_type; }

signals:
    void fractalTypeChanged();

private slots:
    void on_radioMandelbrot_clicked();
    void on_radioJulia_clicked();
    void on_editJuliaX_valueChanged();
    void on_editJuliaY_valueChanged();
    void on_radioNormal_clicked();
    void on_radioConjugate_clicked();
    void on_radioAbsolute_clicked();
    void on_radioAbsoluteIm_clicked();
    void on_radioIntegral_clicked();
    void on_radioReal_clicked();
    void on_spinIntegral_valueChanged();
    void on_spinReal_valueChanged();

private:
    void loadData();
    void saveData();

private:
    Ui::FractalTypeWidget m_ui;

    FractalType m_type;

    bool m_loading;
};

#endif

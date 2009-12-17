/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef FRACTALTYPEDIALOG_H
#define FRACTALTYPEDIALOG_H

#include <QDialog>

#include "ui_fractaltypedialog.h"
#include "fractalpresenter.h"

class FractalTypeDialog : public QDialog
{
    Q_OBJECT
public:
    FractalTypeDialog( QWidget* parent, const FractalModel* model );
    ~FractalTypeDialog();

public:
    void setFractalType( const FractalType& type );
    FractalType fractalType() const;

    Position position() const;

private slots:
    void on_fractalTypeWidget_fractalTypeChanged();

private:
    Ui::FractalTypeDialog m_ui;

    FractalPresenter* m_presenter;
};

#endif

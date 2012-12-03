/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef LOADPRESETDIALOG_H
#define LOADPRESETDIALOG_H

#include <QDialog>

#include "ui_loadpresetdialog.h"

class FractalModel;

class LoadPresetDialog : public QDialog
{
    Q_OBJECT
public:
    LoadPresetDialog( QWidget* parent );
    ~LoadPresetDialog();

public:
    void setModel( FractalModel* model );

public: // overrides
    void accept();

private slots:
    void on_tabWidget_currentChanged();

    void on_listDefault_doubleClicked();
    void on_listUser_doubleClicked();

    void on_buttonBox_clicked( QAbstractButton* button );

    void defaultSelectionChanged();
    void userSelectionChanged();

private:
    void apply();

private:
    Ui::LoadPresetDialog m_ui;

    FractalModel* m_model;
};

#endif

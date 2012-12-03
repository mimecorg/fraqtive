/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef SAVEPRESETDIALOG_H
#define SAVEPRESETDIALOG_H

#include <QDialog>

#include "ui_savepresetdialog.h"

class FractalModel;

class SavePresetDialog : public QDialog
{
    Q_OBJECT
public:
    SavePresetDialog( QWidget* parent );
    ~SavePresetDialog();

public:
    void setModel( FractalModel* model );

public: // overrides
    void accept();

private slots:
    void on_listView_doubleClicked();
    void on_editName_textChanged( const QString& text );

    void selectionChanged();

private:
    Ui::SavePresetDialog m_ui;

    FractalModel* m_model;

    bool m_keepSelection;
};

#endif

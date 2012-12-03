/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Micha³ Mêciñski
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

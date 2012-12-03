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

#ifndef LOADBOOKMARKDIALOG_H
#define LOADBOOKMARKDIALOG_H

#include <QDialog>

#include "ui_loadbookmarkdialog.h"

class FractalPresenter;
class FractalModel;

class LoadBookmarkDialog : public QDialog
{
    Q_OBJECT
public:
    LoadBookmarkDialog( QWidget* parent );
    ~LoadBookmarkDialog();

public:
    void setModel( FractalModel* model );

public: // overrides
    void accept();

private slots:
    void on_listView_doubleClicked();

    void selectionChanged();

private:
    Ui::LoadBookmarkDialog m_ui;

    FractalPresenter* m_presenter;

    FractalModel* m_model;
};

#endif

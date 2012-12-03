/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef SAVEBOOKMARKDIALOG_H
#define SAVEBOOKMARKDIALOG_H

#include <QDialog>

#include "ui_savebookmarkdialog.h"

class FractalModel;

class SaveBookmarkDialog : public QDialog
{
    Q_OBJECT
public:
    SaveBookmarkDialog( QWidget* parent );
    ~SaveBookmarkDialog();

public:
    void setModel( FractalModel* model );

public: // overrides
    void accept();

private slots:
    void on_listView_doubleClicked();
    void on_editName_textChanged( const QString& text );

    void selectionChanged();

private:
    Ui::SaveBookmarkDialog m_ui;

    FractalModel* m_model;

    bool m_keepSelection;
};

#endif

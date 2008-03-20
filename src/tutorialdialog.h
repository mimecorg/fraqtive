/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef TUTORIALDIALOG_H
#define TUTORIALDIALOG_H

#include <QDialog>

#include "ui_tutorialdialog.h"

class TutorialDialog : public QDialog
{
    Q_OBJECT
public:
    TutorialDialog( QWidget* parent );
    ~TutorialDialog();

protected: // overrides
    void closeEvent( QCloseEvent* e );

private slots:
    void on_buttonBack_clicked();
    void on_buttonNext_clicked();

private:
    void loadPage( int page );

private:
    Ui::TutorialDialog m_ui;

    int m_page;
};

#endif

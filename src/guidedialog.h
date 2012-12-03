/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef GUIDEDIALOG_H
#define GUIDEDIALOG_H

#include "xmlui/client.h"

#include <QDialog>

class QTextBrowser;

class GuideDialog : public QDialog, public XmlUi::Client
{
    Q_OBJECT
public:
    GuideDialog( QWidget* parent );
    ~GuideDialog();

private slots:
    void goBack();
    void goForward();
    void goHome();

    void updateActions();

private:
    QTextBrowser* m_browser;
};

#endif

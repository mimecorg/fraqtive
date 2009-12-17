/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>

#include "ui_renamedialog.h"

class RenameDialog : public QDialog
{
    Q_OBJECT
public:
    enum Mode
    {
        PresetMode,
        BookmarkMode
    };

public:
    RenameDialog( QWidget* parent );
    ~RenameDialog();

public:
    void setMode( Mode mode );

    void setName( const QString& name );
    QString name() const;

    void setExistingNames( const QStringList& names );

public: // overrides
    void accept();

private slots:
    void on_lineEdit_textChanged( const QString& text );

private:
    Ui::RenameDialog m_ui;

    Mode m_mode;

    QStringList m_names;
};

#endif

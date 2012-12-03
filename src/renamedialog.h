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
    RenameDialog( Mode mode, const QString& name, QWidget* parent );
    ~RenameDialog();

public:
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

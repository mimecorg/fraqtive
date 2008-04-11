/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "renamedialog.h"

#include <QPushButton>
#include <QMessageBox>

RenameDialog::RenameDialog( QWidget* parent ) : QDialog( parent )
{
    m_ui.setupUi( this );

    setMaximumHeight( sizeHint().height() );
}

RenameDialog::~RenameDialog()
{
}

void RenameDialog::setMode( Mode mode )
{
    m_mode = mode;

    switch ( mode ) {
        case PresetMode:
            setWindowTitle( tr( "Rename Preset" ) );
            break;
        case BookmarkMode:
            setWindowTitle( tr( "Rename Bookmark" ) );
            break;
    }
}

void RenameDialog::setName( const QString& name )
{
    m_ui.lineEdit->setText( name );
}

QString RenameDialog::name() const
{
    return m_ui.lineEdit->text();
}

void RenameDialog::setExistingNames( const QStringList& names )
{
    m_names = names;
}

void RenameDialog::accept()
{
    QString name = m_ui.lineEdit->text();

    if ( m_names.contains( name ) ) {
        QString message;
        switch ( m_mode ) {
            case PresetMode:
                message = tr( "<qt>Are you sure you want to overwrite the existing preset<br>named <b>%1</b>?</qt>" ).arg( name );
                break;
            case BookmarkMode:
                message = tr( "<qt>Are you sure you want to overwrite the existing bookmark<br>named <b>%1</b>?</qt>" ).arg( name );
                break;
        }

        if ( QMessageBox::warning( this, tr( "Warning" ), message,
            QMessageBox::Ok | QMessageBox::Cancel ) != QMessageBox::Ok )
            return;
    }

    QDialog::accept();
}

void RenameDialog::on_lineEdit_textChanged( const QString& text )
{
    m_ui.buttonBox->button( QDialogButtonBox::Ok )->setEnabled( !text.isEmpty() );
}

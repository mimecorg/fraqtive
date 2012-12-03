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

#include "renamedialog.h"

#include <QPushButton>
#include <QMessageBox>

#include "iconloader.h"

RenameDialog::RenameDialog( Mode mode, const QString& name, QWidget* parent ) : QDialog( parent ),
    m_mode( mode )
{
    m_ui.setupUi( this );

    m_ui.promptPixmap->setPixmap( IconLoader::pixmap( "rename", 22 ) );

    switch ( mode ) {
        case PresetMode:
            setWindowTitle( tr( "Rename Preset" ) );
            m_ui.promptLabel->setText( tr( "Rename preset <b>%1<b>:" ).arg( name ) );
            break;
        case BookmarkMode:
            setWindowTitle( tr( "Rename Bookmark" ) );
            m_ui.promptLabel->setText( tr( "Rename bookmark <b>%1<b>:" ).arg( name ) );
            break;
    }

    m_ui.promptLabel->setMinimumWidth( 350 );
    m_ui.promptLabel->setFixedHeight( m_ui.promptLabel->heightForWidth( 350 ) );

    setFixedHeight( sizeHint().height() );

    m_ui.lineEdit->setText( name );
}

RenameDialog::~RenameDialog()
{
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

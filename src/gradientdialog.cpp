/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Michał Męciński
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

#include "gradientdialog.h"

#include <QPushButton>

#include "datastructures.h"
#include "iconloader.h"

GradientDialog::GradientDialog( QWidget* parent ) : QDialog( parent )
{
    m_ui.setupUi( this );

    m_ui.promptPixmap->setPixmap( IconLoader::pixmap( "gradient", 22 ) );
    m_ui.promptLabel->setText( tr( "Edit the color gradient:" ) );

    m_ui.promptLabel->setMinimumWidth( 350 );
    m_ui.promptLabel->setFixedHeight( m_ui.promptLabel->heightForWidth( 350 ) );
}

GradientDialog::~GradientDialog()
{
}

void GradientDialog::setGradient( const Gradient& gradient )
{
    m_ui.gradient->setGradient( gradient );
    m_ui.buttonBox->button( QDialogButtonBox::Apply )->setEnabled( false );
}

Gradient GradientDialog::gradient() const
{
    return m_ui.gradient->gradient();
}

void GradientDialog::accept()
{
    emit applyGradient( gradient() );

    QDialog::accept();
}

void GradientDialog::on_gradient_gradientChanged()
{
    m_ui.buttonBox->button( QDialogButtonBox::Apply )->setEnabled( true );
}

void GradientDialog::on_buttonBox_clicked( QAbstractButton* button )
{
    if ( button == m_ui.buttonBox->button( QDialogButtonBox::Apply ) ) {
        emit applyGradient( gradient() );
        m_ui.buttonBox->button( QDialogButtonBox::Apply )->setEnabled( false );
    }
}

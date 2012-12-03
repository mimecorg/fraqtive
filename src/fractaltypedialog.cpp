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

#include "fractaltypedialog.h"

#include "fractalpresenter.h"
#include "fractalmodel.h"
#include "imageview.h"
#include "datafunctions.h"
#include "iconloader.h"

FractalTypeDialog::FractalTypeDialog( QWidget* parent, const FractalModel* model ) : QDialog( parent )
{
    m_ui.setupUi( this );

    m_ui.promptPixmap->setPixmap( IconLoader::pixmap( "edit", 22 ) );
    m_ui.promptLabel->setText( tr( "Change the type and variant of the fractal:" ) );

    m_ui.promptLabel->setMinimumWidth( 350 );
    m_ui.promptLabel->setFixedHeight( m_ui.promptLabel->heightForWidth( 350 ) );

    setFixedHeight( sizeHint().height() );

    m_presenter = new FractalPresenter( this );

    ImageView* view = new ImageView( m_ui.viewContainer, m_presenter );
    m_ui.viewContainer->setView( view );

    m_presenter->setView( view );
    m_presenter->setPriority( 1 );

    m_presenter->setParameters( fractalType(), position() );
    m_presenter->setColorSettings( model->gradient(), model->backgroundColor(), model->colorMapping() );
    m_presenter->setGeneratorSettings( model->generatorSettings() );
    m_presenter->setViewSettings( model->viewSettings() );

    setFractalType( model->fractalType() );

    m_presenter->setEnabled( true );
}

FractalTypeDialog::~FractalTypeDialog()
{
}

void FractalTypeDialog::setFractalType( const FractalType& type )
{
    m_ui.fractalTypeWidget->setFractalType( type );
}

FractalType FractalTypeDialog::fractalType() const
{
    return m_ui.fractalTypeWidget->fractalType();
}

Position FractalTypeDialog::position() const
{
    return DataFunctions::defaultPosition( fractalType() );
}

void FractalTypeDialog::on_fractalTypeWidget_fractalTypeChanged()
{
    m_presenter->setParameters( fractalType(), position() );
}

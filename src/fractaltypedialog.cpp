/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "fractaltypedialog.h"

#include "fractalpresenter.h"
#include "fractalmodel.h"
#include "imageview.h"
#include "datafunctions.h"

FractalTypeDialog::FractalTypeDialog( QWidget* parent, const FractalModel* model ) : QDialog( parent )
{
    m_ui.setupUi( this );

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

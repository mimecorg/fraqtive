/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "tutorialdialog.h"

#include <QCloseEvent>
#include <QFile>

#include "fraqtiveapplication.h"
#include "configurationdata.h"

TutorialDialog::TutorialDialog( QWidget* parent ) : QDialog( parent )
{
    m_ui.setupUi( this );

    ConfigurationData* config = fraqtive()->configuration();
    m_ui.checkStartup->setChecked( config->value( "ShowTutorial", true ).toBool() );

    loadPage( 1 );
}

TutorialDialog::~TutorialDialog()
{
}

void TutorialDialog::closeEvent( QCloseEvent* e )
{
    e->accept();

    ConfigurationData* config = fraqtive()->configuration();
    config->setValue( "ShowTutorial", m_ui.checkStartup->isChecked() );
}

void TutorialDialog::loadPage( int page )
{
    m_page = page;

    QString path = QString( ":/tutorial/page%1.html" ).arg( m_page, 2, 10, QLatin1Char( '0' ) );
    m_ui.textBrowser->setSource( QUrl::fromLocalFile( path ) );

    m_ui.buttonBack->setEnabled( m_page > 1 );

    QString nextPath = QString( ":/tutorial/page%1.html" ).arg( m_page + 1, 2, 10, QLatin1Char( '0' ) );
    m_ui.buttonNext->setEnabled( QFile::exists( nextPath ) );
}

void TutorialDialog::on_buttonBack_clicked()
{
    loadPage( m_page - 1 );
}

void TutorialDialog::on_buttonNext_clicked()
{
    loadPage( m_page + 1 );
}

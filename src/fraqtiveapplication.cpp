/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "fraqtiveapplication.h"

#include "jobscheduler.h"
#include "configurationdata.h"
#include "fraqtivemainwindow.h"
#include "datastructures.h"
#include "aboutbox.h"
#include "guidedialog.h"
#include "iconloader.h"

FraqtiveApplication::FraqtiveApplication( int& argc, char** argv ) : QApplication( argc, argv )
{
#if defined( Q_WS_WIN )
    setStyle( "XmlUi::WindowsStyle" );
#elif defined( Q_WS_MAC )
    setStyle( "XmlUi::MacStyle" );
#endif

    setWindowIcon( IconLoader::icon( "fraqtive" ) ); 

    registerDataStructures();

    m_jobScheduler = new JobScheduler();

    m_configuration = new ConfigurationData();
    m_configuration->readConfiguration();

    m_mainWindow = new FraqtiveMainWindow();
    m_mainWindow->show();

    if ( m_configuration->value( "LastVersion" ).toString() != version() ) {
        m_configuration->setValue( "LastVersion", version() );
        QTimer::singleShot( 100, this, SLOT( about() ) );
    }
}

FraqtiveApplication::~FraqtiveApplication()
{
    delete m_mainWindow;
    m_mainWindow = NULL;

    delete m_jobScheduler;
    m_jobScheduler = NULL;

    m_configuration->writeConfiguration();

    delete m_configuration;
    m_configuration = NULL;
}

QString FraqtiveApplication::version() const
{
    return "0.4.5";
}

#if defined( Q_OS_WIN )
extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif

QString FraqtiveApplication::technicalInformation()
{
#if defined( Q_OS_WIN )
#if defined( Q_OS_WIN64 )
    QString configBits = "64";
#else
    QString configBits = "32";
#endif
#if defined ( QT_DEBUG )
    QString configMode = "debug";
#else
    QString configMode = "release";
#endif
#endif
#if defined( QT_DLL )
    QString configLink = "dynamic";
#else
    QString configLink = "static";
#endif

    QString qtVersion = qVersion();

    QString infoMessage;
    infoMessage += "<h4>" + tr( "Technical Information" ) + "</h4>";
    infoMessage += "<p>";

#if defined( Q_OS_WIN )
    const IMAGE_NT_HEADERS* header = (const IMAGE_NT_HEADERS*)( (char*)&__ImageBase + __ImageBase.e_lfanew );
    QDateTime compiled = QDateTime::fromTime_t( header->FileHeader.TimeDateStamp );

    infoMessage += tr( "Built on %1 in %2-bit %3 mode." ).arg( compiled.toString( "yyyy-MM-dd HH:mm" ), configBits, configMode ) + " ";
#else
    infoMessage += tr( "Built in %1 mode." ).arg( configMode ) + " ";
#endif

    infoMessage += tr( "Using Qt %1 (%2 linking)." ).arg( qtVersion, configLink ) + "</p>";

    return infoMessage;
}

void FraqtiveApplication::about()
{
    if ( !m_aboutBox ) {
        QString message;
        message += "<h3>" + tr( "Fraqtive %1" ).arg( version() ) + "</h3>";
        message += "<p>" + tr( "Mandelbrot family fractal generator." ) + "</p>";
        message += "<p>" + tr( "This program is free software: you can redistribute it and/or modify"
            " it under the terms of the GNU General Public License as published by"
            " the Free Software Foundation, either version 3 of the License, or"
            " (at your option) any later version." ) + "</p>";
        message += "<p>" + trUtf8( "Copyright &copy; 2004-2012 Michał Męciński" ) + "</p>";

        QString link = "<a href=\"http://fraqtive.mimec.org\">fraqtive.mimec.org</a>";

        QString helpMessage;
        helpMessage += "<h4>" + tr( "Help" ) + "</h4>";
        helpMessage += "<p>" + tr( "Open the Fraqtive Quick Guide for help." ) + "</p>";

        QString webMessage;
        webMessage += "<h4>" + tr( "Website" ) + "</h4>";
        webMessage += "<p>" + tr( "Visit %1 for more information about Fraqtive." ).arg( link ) + "</p>";

        QString donateMessage;
        donateMessage += "<h4>" + tr( "Donations" ) + "</h4>";
        donateMessage += "<p>" + tr( "If you like this program, your donation will help me dedicate more time for it, support it and implement new features." ) + "</p>";

        QString infoMessage = technicalInformation();

        m_aboutBox = new AboutBox( tr( "About Fraqtive" ), message, m_mainWindow );

        AboutBoxSection* helpSection = m_aboutBox->addSection( IconLoader::pixmap( "help" ), helpMessage );

        QPushButton* helpButton = helpSection->addButton( tr( "&Quick Guide" ) );
        connect( helpButton, SIGNAL( clicked() ), this, SLOT( showQuickGuide() ) );

        m_aboutBox->addSection( IconLoader::pixmap( "web" ), webMessage );

        AboutBoxSection* donateSection = m_aboutBox->addSection( IconLoader::pixmap( "donate" ), donateMessage );

        QPushButton* donateButton = donateSection->addButton( tr( "&Donate" ) );
        connect( donateButton, SIGNAL( clicked() ), this, SLOT( openDonations() ) );

        m_aboutBox->addSection( IconLoader::pixmap( "info" ), infoMessage );
    }

    m_aboutBox->show();
    m_aboutBox->activateWindow();
}

void FraqtiveApplication::showQuickGuide()
{
    m_aboutBox->close();

    if ( !m_guideDialog ) {
        m_guideDialog = new GuideDialog( m_mainWindow );

        m_guideDialog->resize( m_mainWindow->width() / 2, m_mainWindow->height() - 100 );
        m_guideDialog->move( m_mainWindow->pos().x() + m_mainWindow->width() / 2 - 50, m_mainWindow->pos().y() + 50 );
    }

    m_guideDialog->show();
    m_guideDialog->activateWindow();
}

void FraqtiveApplication::openDonations()
{
    QDesktopServices::openUrl( QUrl( "http://fraqtive.mimec.org/donations" ) );
}

/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
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

FraqtiveApplication::FraqtiveApplication( int& argc, char** argv ) : QApplication( argc, argv )
{
#if defined( Q_WS_WIN )
    setStyle( "windowsmodernstyle" );
#endif

    QIcon icon;
    icon.addFile( ":/icons/fraqtive-16.png", QSize( 16, 16 ) );
    icon.addFile( ":/icons/fraqtive-22.png", QSize( 22, 22 ) );
    icon.addFile( ":/icons/fraqtive-32.png", QSize( 32, 32 ) );
    icon.addFile( ":/icons/fraqtive-48.png", QSize( 48, 48 ) );
    setWindowIcon( icon ); 

    registerDataStructures();

    m_jobScheduler = new JobScheduler();

    m_configuration = new ConfigurationData();
    m_configuration->readConfiguration();

    m_mainWindow = new FraqtiveMainWindow();
    m_mainWindow->show();
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

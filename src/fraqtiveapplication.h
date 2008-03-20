/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef FRAQTIVEAPPLICATION_H
#define FRAQTIVEAPPLICATION_H

#include <QApplication>

class JobScheduler;
class ConfigurationData;
class FraqtiveMainWindow;

class FraqtiveApplication : public QApplication
{
    Q_OBJECT
public:
    FraqtiveApplication( int& argc, char** argv );
    ~FraqtiveApplication();

public:
    JobScheduler* jobScheduler() const { return m_jobScheduler; }

    ConfigurationData* configuration() const { return m_configuration; }

private:
    JobScheduler* m_jobScheduler;
    ConfigurationData* m_configuration;
    FraqtiveMainWindow* m_mainWindow;
};

inline FraqtiveApplication* fraqtive()
{
    return static_cast<FraqtiveApplication*>( qApp );
}

#endif

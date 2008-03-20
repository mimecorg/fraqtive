/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef JOBSCHEDULER_H
#define JOBSCHEDULER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class AbstractJobProvider;

class JobScheduler : public QObject
{
    Q_OBJECT
public:
    JobScheduler( QObject* parent = NULL );
    ~JobScheduler();

public:
    void addJobs( AbstractJobProvider* provider, int count );

    int cancelAllJobs( AbstractJobProvider* provider );

private:
    bool executeJob();

private:
    QList<QThread*> m_threads;

    bool m_stopping;

    QMutex m_mutex;

    QList<AbstractJobProvider*> m_pendingProviders;

    QWaitCondition m_hasPendingProviders;

    friend class WorkerThread;
};

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    WorkerThread( JobScheduler* scheduler );
    ~WorkerThread();

public: // overrides
    void run();

private:
    JobScheduler* m_scheduler;
};

#endif

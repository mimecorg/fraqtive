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

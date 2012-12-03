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

#include "jobscheduler.h"

#include "abstractjobprovider.h"

JobScheduler::JobScheduler( QObject* parent ) : QObject( parent ),
    m_stopping( false )
{
    int threads = QThread::idealThreadCount();
    if ( threads < 0 )
        threads = 1;

    for ( int i = 0; i < threads; i++ ) {
        WorkerThread* thread = new WorkerThread( this );
        thread->start( QThread::LowPriority );
        m_threads.append( thread );
    }
}

JobScheduler::~JobScheduler()
{
    QMutexLocker locker( &m_mutex );

    m_stopping = true;

    m_hasPendingProviders.wakeAll();

    locker.unlock();

    for ( int i = 0; i < m_threads.count(); i++ )
        m_threads[ i ]->wait();
}

void JobScheduler::addJobs( AbstractJobProvider* provider, int count )
{
    QMutexLocker locker( &m_mutex );

    int pos = 0;
    int priority = provider->priority();
    while ( pos < m_pendingProviders.count() && m_pendingProviders[ pos ]->priority() >= priority )
        pos++;

    for ( int i = 0; i < count; i++ )
        m_pendingProviders.insert( pos, provider );

    m_hasPendingProviders.wakeAll();
}

int JobScheduler::cancelAllJobs( AbstractJobProvider* provider )
{
    QMutexLocker locker( &m_mutex );

    return m_pendingProviders.removeAll( provider );
}

bool JobScheduler::executeJob()
{
    QMutexLocker locker( &m_mutex );

    while ( !m_stopping && m_pendingProviders.count() == 0 )
        m_hasPendingProviders.wait( &m_mutex );

    if ( m_stopping )
        return false;

    AbstractJobProvider* provider = m_pendingProviders.takeFirst();

    locker.unlock();

    provider->executeJob();

    return true;
}

WorkerThread::WorkerThread( JobScheduler* scheduler ) : QThread( scheduler ),
    m_scheduler( scheduler )
{
}

WorkerThread::~WorkerThread()
{
}

void WorkerThread::run()
{
    while ( m_scheduler->executeJob() )
        ;
}

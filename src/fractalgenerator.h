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

#ifndef FRACTALGENERATOR_H
#define FRACTALGENERATOR_H

#include <QEvent>
#include <QMutex>
#include <QWaitCondition>

#include "abstractjobprovider.h"
#include "datastructures.h"
#include "generatorcore.h"

class FractalData;

class FractalGenerator : public QObject, public AbstractJobProvider
{
    Q_OBJECT
public:
    enum UpdateStatus
    {
        NoUpdate,
        InitialUpdate,
        PartialUpdate,
        FullUpdate,
        ClearUpdate
    };

    static const QEvent::Type UpdateEvent = static_cast<QEvent::Type>( QEvent::User + 1 );

public:
    FractalGenerator( QObject* parent );
    ~FractalGenerator();

public:
    void setPreviewMode( bool preview );
    void setPriority( int priority );
    void setReceiver( QObject* receiver );

    void setEnabled( bool enabled );

    void setParameters( const FractalType& type, const Position& position );
    void setFractalType( const FractalType& type );
    void setPosition( const Position& position );

    void setGeneratorSettings( const GeneratorSettings& settings );

    void setResolution( const QSize& resolution );
    QSize resolution() const { return m_resolution; }

    int maximumIterations() const;

    UpdateStatus updateData( FractalData* data );

public: // AbstractJobProvider implementation
    int priority() const;

    void executeJob();

private:
    void calculateRegion( const QRect& region );

    void reset();

    void handleState();

    void createFunctor();

    void splitRegions();

    void calculateInput( GeneratorCore::Input* input, const QRect& region );
    void calculateOutput( GeneratorCore::Output* output, const QRect& region );

    void addJobs();
    void cancelJobs();
    void finishJob();

    void appendValidRegion( const QRect& region );

    void postUpdate( UpdateStatus update );

private:
    bool m_preview;
    int m_priority;
    QObject* m_receiver;

    QMutex m_mutex;

    bool m_enabled;

    FractalType m_type;
    Position m_position;
    GeneratorSettings m_settings;

    QSize m_resolution;
    QSize m_bufferSize;

    GeneratorCore::Functor* m_functor;
#if defined( HAVE_SSE2 )
    GeneratorCore::FunctorSSE2* m_functorSSE2;
#endif

    double* m_buffer;

    QList<QRect> m_regions;

    int m_activeJobs;
    QWaitCondition m_allJobsDone;

    bool m_pending;

    FractalType m_pendingType;
    Position m_pendingPosition;
    GeneratorSettings m_pendingSettings;

    QSize m_pendingResolution;
    QSize m_pendingBufferSize;

    UpdateStatus m_update;
    QList<QRect> m_validRegions;

    double* m_previewBuffer;
};

#endif

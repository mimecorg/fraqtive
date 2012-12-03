/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Micha³ Mêciñski
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

#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QImage>

#include "abstractjobprovider.h"
#include "datastructures.h"

class ImageGenerator : public QObject, public AbstractJobProvider
{
    Q_OBJECT
public:
    ImageGenerator( QObject* parent );
    ~ImageGenerator();

public:
    void setResolution( const QSize& resolution );
    void setParameters( const FractalType& type, const Position& position );
    void setColorSettings( const Gradient& gradient, const QColor& backgroundColor, const ColorMapping& mapping );
    void setGeneratorSettings( const GeneratorSettings& settings );
    void setViewSettings( const ViewSettings& settings );

    void setImageCount( int count );
    void setCurrentImage( int image );

    int maximumProgress() const { return m_maximumProgress * m_imageCount; }

    QImage takeImage();

    bool start();

public: // AbstractJobProvider implementation
    int priority() const;

    void executeJob();

signals:
    void progressChanged( int value );
    void completed();

private:
    void calculateRegion( const QRect& region );

    void calculateInput( GeneratorCore::Input* input, const QRect& region );
    void calculateOutput( GeneratorCore::Output* output, const QRect& region );

    int maximumIterations() const;

    void addJobs();
    void cancelJobs();
    void finishJob();

private:
    QSize m_resolution;

    FractalType m_type;
    Position m_position;

    QRgb* m_gradientCache;
    QColor m_backgroundColor;
    ColorMapping m_colorMapping;

    GeneratorSettings m_generatorSettings;
    ViewSettings m_viewSettings;

    int m_maximumProgress;

    QMutex m_mutex;

    QImage m_image;

    QList<QRect> m_regions;

    int m_activeJobs;
    QWaitCondition m_allJobsDone;

    int m_imageCount;
    int m_currentImage;
};

#endif

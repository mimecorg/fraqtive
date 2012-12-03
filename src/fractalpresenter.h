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

#ifndef FRACTALPRESENTER_H
#define FRACTALPRESENTER_H

#include <QObject>

#include "datastructures.h"
#include "fractaldata.h"

class FractalModel;
class AbstractView;
class FractalGenerator;
class FractalData;

class FractalPresenter : public QObject
{
    Q_OBJECT
public:
    FractalPresenter( QObject* parent );
    ~FractalPresenter();

public:
    void setModel( FractalModel* model );
    void setView( AbstractView* view );

    void setPreviewMode( bool preview );
    void setPriority( int priority );

    void setEnabled( bool enabled );

    void setParameters( const FractalType& type, const Position& position );
    void setFractalType( const FractalType& type );
    void setPosition( const Position& position );

    void setColorSettings( const Gradient& gradient, const QColor& background, const ColorMapping& mapping );
    void setGradient( const Gradient& gradient );
    void setBackgroundColor( const QColor& color );
    void setColorMapping( const ColorMapping& mapping );

    void setGeneratorSettings( const GeneratorSettings& settings );
    void setViewSettings( const ViewSettings& settings );

    void setAnimationState( const AnimationState& state );

    const FractalData* fractalData();

    int maximumIterations() const;

    void setResolution( const QSize& resolution );

    void setHoveringPoint( const QPointF& point );
    void clearHovering();

    void setTrackingTransform( const QTransform& transform );
    void clearTracking();

    void changePosition( const QTransform& transform );

    void switchToJulia( const QPointF& point );

    void adjustCameraZoom( double delta );

    void navigateBackward();
    void navigateForward();

protected: // overrides
    void customEvent( QEvent* e );

private:
    QTransform transformFromPosition( const Position& position );
    Position positionFromTransform( const QTransform& transform );

    FractalType juliaType( const QPointF& point );
    Position juliaPosition();

private:
    FractalGenerator* m_generator;

    FractalModel* m_model;
    AbstractView* m_view;

    bool m_preview;

    bool m_enabled;

    FractalData m_data;

    FractalType m_type;
    Position m_position;

    QSize m_resolution;

    QPointF m_hoveringPoint;
};

#endif

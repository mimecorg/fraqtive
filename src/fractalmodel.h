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

#ifndef FRACTALMODEL_H
#define FRACTALMODEL_H

#include <QObject>
#include <QColor>
#include <QTime>

#include "datastructures.h"

class QTimer;

class FractalPresenter;

enum ViewMode
{
    NoViewMode,
    ImageViewMode,
    MeshViewMode
};

class FractalModel : public QObject
{
    Q_OBJECT
public:
    FractalModel( QObject* parent );
    ~FractalModel();

public:
    FractalPresenter* presenter() const { return m_presenter; }

    void setPreviewPresenter( FractalPresenter* presenter );
    FractalPresenter* previewPresenter() const { return m_previewPresenter; }

    void setEnabled( bool enabled );
    bool isEnabled() const { return m_enabled; }

    void setParameters( const FractalType& type, const Position& position );

    void setFractalType( const FractalType& type );
    FractalType fractalType() const { return m_fractalType; }

    void setPosition( const Position& position );
    Position position() const { return m_position; }

    void setDefaultPosition();
    bool hasDefaultPosition() const;

    void setTrackingPosition( const Position& position );
    void clearTracking();

    bool isTracking() const { return m_tracking; }
    Position trackingPosition() const { return m_trackingPosition; }

    void setHoveringParameters( const FractalType& type, const Position& position );
    void clearHovering();

    bool isHovering() const { return m_hovering; }
    FractalType hoveringFractalType() const { return m_hoveringFractalType; }
    Position hoveringPosition() const { return m_hoveringPosition; }

    void setNavigationEnabled( bool enable );
    bool isNavigationEnabled() const { return m_navigation; }

    bool canNavigateBackward() const;
    bool canNavigateForward() const;

    void navigateBackward();
    void navigateForward();

    void setColorSettings( const Gradient& gradient, const QColor& backround, const ColorMapping& mapping );

    void setGradient( const Gradient& gradient );
    Gradient gradient() const { return m_gradient; }

    void setBackgroundColor( const QColor& color );
    QColor backgroundColor() const { return m_backgroundColor; }

    void setColorMapping( const ColorMapping& mapping );
    ColorMapping colorMapping() const { return m_colorMapping; }

    bool hasDefaultColorSettings() const;
    void saveDefaultColorSettings() const;
    void loadDefaultColorSettings();

    void setGeneratorSettings( const GeneratorSettings& settings );
    GeneratorSettings generatorSettings() const { return m_generatorSettings; }

    bool hasDefaultGeneratorSettings() const;
    void saveDefaultGeneratorSettings() const;
    void loadDefaultGeneratorSettings();

    void setViewSettings( const ViewSettings& settings );
    ViewSettings viewSettings() const { return m_viewSettings; }

    bool hasDefaultViewSettings() const;
    void saveDefaultViewSettings() const;
    void loadDefaultViewSettings();

    void setViewMode( ViewMode mode );
    ViewMode viewMode() const { return m_viewMode; }

    void setAnimationSettings( const AnimationSettings& settings );
    AnimationSettings animationSettings() const { return m_animationSettings; }

    AnimationState animationState() const { return m_animationState; }

signals:
    void fractalTypeChanged();
    void positionChanged();

    void trackingChanged();
    void hoveringChanged();
    void navigationChanged();

    void gradientChanged();
    void backgroundColorChanged();
    void colorMappingChanged();

    void generatorSettingsChanged();
    void viewSettingsChanged();

    void viewModeChanged();

    void animationSettingsChanged();

private slots:
    void animate();

private:
    void storeParameters();
    void setParametersInternal( const FractalType& type, const Position& position );

    void updateTimer();

private:
    struct Navigation
    {
        FractalType m_type;
        Position m_position;
    };

private:
    FractalPresenter* m_presenter;
    FractalPresenter* m_previewPresenter;

    bool m_enabled;

    FractalType m_fractalType;
    Position m_position;

    bool m_tracking;
    Position m_trackingPosition;

    bool m_hovering;
    FractalType m_hoveringFractalType;
    Position m_hoveringPosition;

    bool m_navigation;
    QList<Navigation> m_navigationBackward;
    QList<Navigation> m_navigationForward;

    Gradient m_gradient;
    QColor m_backgroundColor;
    ColorMapping m_colorMapping;

    GeneratorSettings m_generatorSettings;
    ViewSettings m_viewSettings;

    ViewMode m_viewMode;

    AnimationSettings m_animationSettings;
    AnimationState m_animationState;

    QTimer* m_timer;
    QTime m_time;
};

#endif

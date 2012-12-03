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

#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>

#include "abstractview.h"
#include "datastructures.h"

class FractalPresenter;

class ImageView : public QWidget, public AbstractView
{
    Q_OBJECT
public:
    ImageView( QWidget* parent, FractalPresenter* presenter );
    ~ImageView();

public:
    void setInteractive( bool interactive );

    QImage image() const { return m_image; }

public: // AbstractView implementation
    void clearView();

    void transformView( const QTransform& transform );

    void initialUpdate( const FractalData* data );
    void partialUpdate( const FractalData* data );
    void fullUpdate( const FractalData* data );

    void setColorSettings( const Gradient& gradient, const QColor& backgroundColor, const ColorMapping& mapping );
    void setGradient( const Gradient& gradient );
    void setBackgroundColor( const QColor& color );
    void setColorMapping( const ColorMapping& mapping );

    void setViewSettings( const ViewSettings& settings );

    void setAnimationState( const AnimationState& state );

protected: // overrides
    void resizeEvent( QResizeEvent* e );
    void paintEvent( QPaintEvent* e );

    void mousePressEvent( QMouseEvent* e );
    void mouseMoveEvent( QMouseEvent* e );
    void mouseReleaseEvent( QMouseEvent* e );
    void mouseDoubleClickEvent( QMouseEvent* e );
    void wheelEvent( QWheelEvent* e );
    void leaveEvent( QEvent* e );

    void keyPressEvent( QKeyEvent* e );

private:
    void updateGradient();
    void updateBackground();
    void updateImage();

    void drawImage( const FractalData* data, const QRect& region );

    void calculateScale();

    QTransform worldTransform();

private:
    enum Tracking
    {
        NoTracking,
        DragMove,
        DragRotate,
        DragZoomIn,
        DragZoomOut,
        RotateCenter,
        ZoomCenter,
        ZoomPoint
    };

private:
    FractalPresenter* m_presenter;

    bool m_interactive;

    QImage m_image;

    Gradient m_gradient;
    QColor m_backgroundColor;
    ColorMapping m_colorMapping;

    ViewSettings m_settings;

    AnimationState m_animationState;

    QRgb* m_gradientCache;

    QRect m_updatedRegion;

    QTransform m_scale;
    QTransform m_invScale;

    Tracking m_tracking;
    QPoint m_trackStart;

    QTransform m_transform;
};

#endif

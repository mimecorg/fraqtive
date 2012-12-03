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

#ifndef MESHVIEW_H
#define MESHVIEW_H

#include <QGLWidget>

#include "abstractview.h"
#include "datastructures.h"

class FractalPresenter;

class MeshView : public QGLWidget, public AbstractView
{
    Q_OBJECT
public:
    MeshView( QWidget* parent, FractalPresenter* presenter );
    ~MeshView();

public:
    QImage image();

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
    void initializeGL();
    void resizeGL( int width, int height );
    void paintGL();

    void mousePressEvent( QMouseEvent* e );
    void mouseMoveEvent( QMouseEvent* e );
    void mouseReleaseEvent( QMouseEvent* e );
    void wheelEvent( QWheelEvent* e );

    void keyPressEvent( QKeyEvent* e );

private:
    void generateTexture();

    void initializeVertices();
    void updateVertices( const FractalData* data, const QRect& region );

private:
    enum Tracking
    {
        NoTracking,
        RotateXY,
        ZoomInOut
    };

private:
    FractalPresenter* m_presenter;

    Gradient m_gradient;
    QColor m_backgroundColor;
    ColorMapping m_colorMapping;

    ViewSettings m_settings;

    AnimationState m_animationState;

    GLuint m_textureId;
    bool m_textureDirty;

    float* m_vertexArray;
    float* m_textureCoordArray;
    QSize m_resolution;

    QRect m_updatedRegion;

    double m_maximumDepth;

    QVector<double> m_averageRowHeight;
    double m_averageHeight;

    double m_rotation;
    double m_angle;

    Tracking m_tracking;
    QPoint m_lastPos;
};

#endif

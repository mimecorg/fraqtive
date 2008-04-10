/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
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

    void transformView( const QMatrix& transform );

    void initialUpdate( const FractalData* data );
    void partialUpdate( const FractalData* data );
    void fullUpdate( const FractalData* data );

    void setColorSettings( const Gradient& gradient, const QColor& backgroundColor, const ColorMapping& mapping );
    void setGradient( const Gradient& gradient );
    void setBackgroundColor( const QColor& color );
    void setColorMapping( const ColorMapping& mapping );

    void setViewSettings( const ViewSettings& settings );

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

    QMatrix worldMatrix();

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

    QRgb* m_gradientCache;

    QRect m_updatedRegion;

    QMatrix m_scale;
    QMatrix m_invScale;

    Tracking m_tracking;
    QPoint m_trackStart;

    QMatrix m_transform;
};

#endif

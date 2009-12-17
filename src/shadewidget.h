/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef SHADEWIDGET_H
#define SHADEWIDGET_H

#include <QWidget>

class ShadeWidget : public QWidget
{
    Q_OBJECT
public:
    ShadeWidget( QWidget* parent );
    ~ShadeWidget();

public:
    void setShadeColor( const QColor& color );
    void setGradientStops( const QGradientStops& stops );

    void setPoints( const QPolygonF& points );
    QPolygonF points() const { return m_points; }

signals:
    void pointsChanged();

public: // overrides
    QSize minimumSizeHint() const;

protected: // overrides
    void paintEvent( QPaintEvent* e );

    void mousePressEvent( QMouseEvent* e );
    void mouseMoveEvent( QMouseEvent* e );
    void mouseReleaseEvent( QMouseEvent* e );

private:
    QPointF mapToWidget( const QPointF& point );
    QPointF mapFromWidget( const QPointF& point );

private:
    QColor m_color;
    QGradientStops m_stops;

    QPolygonF m_points;

    bool m_dragging;
    int m_index;
};

#endif

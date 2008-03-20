/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "shadewidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QToolTip>

ShadeWidget::ShadeWidget( QWidget* parent ) : QWidget( parent ),
    m_dragging( false )
{
}

ShadeWidget::~ShadeWidget()
{
}

void ShadeWidget::setShadeColor( const QColor& color )
{
    m_color = color;
    m_stops.clear();
    update();
}

void ShadeWidget::setGradientStops( const QGradientStops& stops )
{
    m_color = QColor();
    m_stops = stops;
    update();
}

void ShadeWidget::setPoints( const QPolygonF& points )
{
    if ( m_points != points ) {
        m_points = points;
        emit pointsChanged();
        update();
    }
}

QSize ShadeWidget::minimumSizeHint() const
{
    return QSize( 30, 30 );
}

void ShadeWidget::paintEvent( QPaintEvent* e )
{
    QPainter painter( this );

    painter.setRenderHint( QPainter::Antialiasing );

    painter.setPen( QColor( 80, 80, 80 ) );
    painter.setBrush( QColor( 160, 160, 160 ) );

    QPainterPath border;
    border.moveTo( 5.5, 0.0 );
    border.arcTo( e->rect().width() - 11.5, 0.0, 11.0, 11.0, 90.0, -90.0 );
    border.arcTo( e->rect().width() - 11.5, e->rect().height() - 11.5, 11.0, 11.0, 0.0, -90.0 );
    border.arcTo( 0.0, e->rect().height() - 11.5, 11.0, 11.0, -90.0, -90.0 );
    border.arcTo( 0.0, 0.0, 11.0, 11.0, -180.0, -90.0 );

    painter.drawPath( border );

    QRectF rect = e->rect();
    rect.adjust( 5.5, 5.5, -5.5, -5.5 );

    painter.setPen( QColor( 128, 128, 128 ) );

    for ( int i = 1; i <= 9; i++ ) {
        double x = mapToWidget( QPointF( i / 10.0, 0.0 ) ).x();
        painter.drawLine( QPointF( x, 0.0 ), QPointF( x, e->rect().bottom() ) );
    }

    if ( m_color.isValid() ) {
        QLinearGradient gradient( rect.bottomLeft(), rect.topLeft() );
        gradient.setColorAt( 0.0, Qt::black );
        gradient.setColorAt( 1.0, m_color );
        painter.setBrush( gradient );
    } else if ( !m_stops.isEmpty() ) {
        QLinearGradient gradient( rect.topLeft(), rect.topRight() );
        gradient.setStops( m_stops );
        painter.setBrush( gradient );
    }

    painter.setPen( QColor( 80, 80, 80 ) );

    painter.drawRect( rect );

    if ( !m_points.isEmpty() ) {
        painter.setPen( QPen( QColor( 255, 255, 255, 128 ), 2.0 ) );
        painter.setBrush( Qt::NoBrush );

        QPainterPath path;
        path.moveTo( mapToWidget( m_points.at( 0 ) ) );

        for ( int i = 1; i < m_points.size(); i++ ) {
            QPointF p1 = mapToWidget( m_points.at( i - 1 ) );
            QPointF p2 = mapToWidget( m_points.at( i ) );
            path.cubicTo( ( p1.x() + p2.x() ) / 2, p1.y(), ( p1.x() + p2.x() ) / 2, p2.y(), p2.x(), p2.y() );
        }

        painter.drawPath( path );

        QPen pointPen( QColor( 255, 255, 255, 192 ) );
        pointPen.setWidth( 1 );

        painter.setPen( QPen( QColor( 255, 255, 255, 192 ), 1.0 ) );
        painter.setBrush( QColor( 255, 255, 255, 144 ) );

        for ( int i = 0; i < m_points.size(); i++ ) {
            QPointF point = mapToWidget( m_points.at( i ) );
            painter.drawEllipse( QRectF( point.x() - 4.5, point.y() - 4.5, 9.0, 9.0 ) );
        }
    }
}

void ShadeWidget::mousePressEvent( QMouseEvent* e )
{
    if ( e->button() != Qt::LeftButton || m_points.isEmpty() )
        return;

    int index = -1;
    double minDistance = 0.0;

    for ( int i = 0; i < m_points.count(); i++ ) {
        QPointF center = mapToWidget( m_points.at( i ) );
        double distance = QLineF( e->pos(), center ).length();
        if ( index < 0 || distance < minDistance ) {
            index = i;
            minDistance = distance;
        }
    }

    QPointF point = mapFromWidget( e->pos() );
    point.setX( qBound( 0.0, point.x(), 1.0 ) );
    point.setY( qBound( 0.0, point.y(), 1.0 ) );

    if ( index >= 0 && minDistance <= 10.0 ) {
        m_dragging = true;
        m_index = index;
        if ( index == 0 )
            point.setX( 0.0 );
        else if ( index == m_points.count() - 1 )
            point.setX( 1.0 );
        m_points[ index ] = point;
    } else if ( QRectF( 0.0, 0.0, 1.0, 1.0 ).contains( point ) ) {
        index = 1;
        while ( index < m_points.count() - 1 && m_points.at( index ).x() < point.x() )
            index++;
        m_points.insert( index, point );
        m_dragging = true;
        m_index = index;
    }

    if ( m_dragging ) {
        QToolTip::showText( mapToGlobal( mapToWidget( point ).toPoint() ), QString( "%1, %2" ).arg( point.x(), 0, 'f', 3 ).arg( point.y(), 0, 'f', 2 ), this );

        emit pointsChanged();
        update();
    }
}

void ShadeWidget::mouseMoveEvent( QMouseEvent* e )
{
    if ( !m_dragging )
        return;

    QPointF point = mapFromWidget( e->pos() );
    point.setX( qBound( 0.0, point.x(), 1.0 ) );
    point.setY( qBound( 0.0, point.y(), 1.0 ) );

    if ( m_index == 0 ) {
        point.setX( 0.0 );
        m_points[ m_index ] = point;
    } else if ( m_index == m_points.count() - 1 ) {
        point.setX( 1.0 );
        m_points[ m_index ] = point;
    } else {
        if ( m_index >= 0 )
            m_points.remove( m_index );
        if ( rect().adjusted( -30, -30, 30, 30 ).contains( e->pos() ) ) {
            int index = 1;
            while ( index < m_points.count() - 1 && m_points.at( index ).x() < point.x() )
                index++;
            m_points.insert( index, point );
            m_index = index;
        } else {
            m_index = -1;
        }
    }

    if ( m_index >= 0 ) {
        QToolTip::showText( mapToGlobal( mapToWidget( point ).toPoint() ),
            QString( "%1, %2" ).arg( point.x(), 0, 'f', 3 ).arg( point.y(), 0, 'f', 2 ), this );
    } else {
        QToolTip::hideText();
    }

    emit pointsChanged();
    update();
}

void ShadeWidget::mouseReleaseEvent( QMouseEvent* e )
{
    if ( e->button() == Qt::LeftButton )
        m_dragging = false;
}

QPointF ShadeWidget::mapToWidget( const QPointF& point )
{
    return QPointF( point.x() * ( width() - 14.0 ) + 7.0,
        height() - point.y() * ( height() - 14.0 ) - 7.0 );
}

QPointF ShadeWidget::mapFromWidget( const QPointF& point )
{
    return QPointF( ( point.x() - 7.0 ) / ( width() - 14.0 ),
        ( height() - point.y() - 7.0 ) / ( height() - 14.0 ) );
}

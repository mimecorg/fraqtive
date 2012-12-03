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

#include "fractalpresenter.h"

#include <math.h>

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

#include <QTransform>

#include "fractalgenerator.h"
#include "fractalmodel.h"
#include "fractaldata.h"
#include "abstractview.h"

FractalPresenter::FractalPresenter( QObject* parent ) : QObject( parent ),
    m_model( NULL ),
    m_view( NULL ),
    m_preview( false ),
    m_enabled( false )
{
    m_generator = new FractalGenerator( this );
    m_generator->setReceiver( this );
}

FractalPresenter::~FractalPresenter()
{
}

void FractalPresenter::setModel( FractalModel* model )
{
    m_model = model;
}

void FractalPresenter::setView( AbstractView* view )
{
    m_view = view;

    if ( m_model ) {
        m_view->setColorSettings( m_model->gradient(), m_model->backgroundColor(), m_model->colorMapping() );
        m_view->setViewSettings( m_model->viewSettings() );
        m_view->setAnimationState( m_model->animationState() );
    }
}

void FractalPresenter::setPreviewMode( bool preview )
{
    if ( m_preview != preview ) {
        m_preview = preview;
        m_generator->setPreviewMode( preview );
    }
}

void FractalPresenter::setPriority( int priority )
{
    m_generator->setPriority( priority );
}

void FractalPresenter::setEnabled( bool enabled )
{
    if ( m_enabled != enabled ) {
        m_enabled = enabled;
        m_generator->setEnabled( enabled );
        if ( !enabled )
            m_view->clearView();
        else
            m_view->initialUpdate( fractalData() );
    }
}

void FractalPresenter::setParameters( const FractalType& type, const Position& position )
{
    if ( m_type != type ) {
        m_type = type;
        m_position = position;
        m_generator->setParameters( type, position );
        if ( type.fractal() == JuliaFractal )
            clearHovering();
    } else {
        setPosition( position );
    }
}

void FractalPresenter::setFractalType( const FractalType& type )
{
    if ( m_type != type ) {
        m_type = type;
        m_generator->setFractalType( type );
        if ( type.fractal() == JuliaFractal )
            clearHovering();
    }
}

static QTransform preciselyInverted( const QTransform& transform )
{
    QTransform inverted;

    switch ( transform.type() ) {
        case QTransform::TxNone:
            break;

        case QTransform::TxTranslate:
            inverted.translate( -transform.dx(), -transform.dy() );
            break;

        case QTransform::TxScale:
            inverted.scale( 1.0 / transform.m11(), 1.0 / transform.m22() );
            inverted.translate( -transform.dx(), -transform.dy() );
            break;

        default:
            inverted = transform.inverted();
    }

    return inverted;
}

static QLineF preciselyMap( const QTransform& transform, const QLineF& line )
{
    qreal fx1 = line.x1();
    qreal fy1 = line.y1();
    qreal fx2 = line.x2();
    qreal fy2 = line.y2();

    qreal x1 = transform.m11() * fx1 + transform.m21() * fy1 + transform.dx();
    qreal y1 = transform.m12() * fx1 + transform.m22() * fy1 + transform.dy();
    qreal x2 = transform.m11() * fx2 + transform.m21() * fy2 + transform.dx();
    qreal y2 = transform.m12() * fx2 + transform.m22() * fy2 + transform.dy();

    return QLineF( x1, y1, x2, y2 );
}

void FractalPresenter::setPosition( const Position& position )
{
    if ( m_position != position ) {
        if ( m_enabled && !m_preview ) {
            QTransform oldTransform = transformFromPosition( m_position );
            QTransform newTransform = transformFromPosition( position );
            QTransform difference = preciselyInverted( newTransform * preciselyInverted( oldTransform ) );
            m_view->transformView( difference );
        }
        m_position = position;
        m_generator->setPosition( position );

        if ( m_model && m_model->isHovering() && !m_model->isTracking() )
            setHoveringPoint( m_hoveringPoint );
    }
}

void FractalPresenter::setColorSettings( const Gradient& gradient, const QColor& background, const ColorMapping& mapping )
{
    m_view->setColorSettings( gradient, background, mapping );
}

void FractalPresenter::setGradient( const Gradient& gradient )
{
    m_view->setGradient( gradient );
}

void FractalPresenter::setBackgroundColor( const QColor& color )
{
    m_view->setBackgroundColor( color );
}

void FractalPresenter::setColorMapping( const ColorMapping& mapping )
{
    m_view->setColorMapping( mapping );
}

void FractalPresenter::setGeneratorSettings( const GeneratorSettings& settings )
{
    m_generator->setGeneratorSettings( settings );
}

void FractalPresenter::setViewSettings( const ViewSettings& settings )
{
    m_view->setViewSettings( settings );
}

void FractalPresenter::setAnimationState( const AnimationState& state )
{
    m_view->setAnimationState( state );
}

const FractalData* FractalPresenter::fractalData()
{
    m_generator->updateData( &m_data );
    return &m_data;
}

int FractalPresenter::maximumIterations() const
{
    return m_generator->maximumIterations();
}

void FractalPresenter::setResolution( const QSize& resolution )
{
    if ( m_resolution != resolution ) {
        m_resolution = resolution;
        m_generator->setResolution( resolution );
    }
}

void FractalPresenter::setHoveringPoint( const QPointF& point )
{
    if ( m_model && m_model->fractalType().fractal() != JuliaFractal ) {
        m_hoveringPoint = point;
        m_model->setHoveringParameters( juliaType( point ), juliaPosition() );
    }
}

void FractalPresenter::clearHovering()
{
    if ( m_model )
        m_model->clearHovering();
}

void FractalPresenter::setTrackingTransform( const QTransform& transform )
{
    QTransform oldTransform = transformFromPosition( m_position );
    QTransform newTransform = preciselyInverted( transform ) * oldTransform;

    Position position = positionFromTransform( newTransform );
    m_model->setTrackingPosition( position );
}

void FractalPresenter::clearTracking()
{
    m_model->clearTracking();
}

void FractalPresenter::changePosition( const QTransform& transform )
{
    QTransform oldTransform = transformFromPosition( m_position );
    QTransform newTransform = preciselyInverted( transform ) * oldTransform;

    Position position = positionFromTransform( newTransform );
    m_model->setPosition( position );
}

void FractalPresenter::switchToJulia( const QPointF& point )
{
    if ( m_model && m_model->fractalType().fractal() != JuliaFractal )
        m_model->setParameters( juliaType( point ), juliaPosition() );
}

void FractalPresenter::adjustCameraZoom( double delta )
{
    ViewSettings settings = m_model->viewSettings();

    double zoom = qBound( 10.0, settings.cameraZoom() + 3.5 * delta, 45.0 );

    settings.setCameraZoom( zoom );
    m_model->setViewSettings( settings );
}


void FractalPresenter::navigateBackward()
{
    if ( m_model && m_model->canNavigateBackward() )
        m_model->navigateBackward();
}

void FractalPresenter::navigateForward()
{
    if ( m_model && m_model->canNavigateForward() )
        m_model->navigateForward();
}

void FractalPresenter::customEvent( QEvent* e )
{
    if ( e->type() == FractalGenerator::UpdateEvent && m_enabled ) {
        FractalGenerator::UpdateStatus status = m_generator->updateData( &m_data );
        switch ( status ) {
            case FractalGenerator::InitialUpdate:
                m_view->initialUpdate( &m_data );
                break;

            case FractalGenerator::PartialUpdate:
                m_view->partialUpdate( &m_data );
                break;

            case FractalGenerator::FullUpdate:
                m_view->fullUpdate( &m_data );
                break;

            default:
                break;
        }
    }
}

QTransform FractalPresenter::transformFromPosition( const Position& position )
{
    QPointF center( m_resolution.width() / 2.0, m_resolution.height() / 2.0 );

    double scale = pow( 10.0, -position.zoomFactor() ) / m_resolution.height();

    QTransform transform;
    transform.translate( position.center().x(), position.center().y() );
    transform.rotate( -position.angle() );
    transform.scale( scale, scale );
    transform.translate( -center.x(), -center.y() );

    return transform;
}

Position FractalPresenter::positionFromTransform( const QTransform& transform )
{
    QPointF center( m_resolution.width() / 2.0, m_resolution.height() / 2.0 );

    QLineF line( center, QPointF( center.x() + m_resolution.height(), center.y() ) );
    QLineF mapped = preciselyMap( transform, line );

    Position position;
    position.setCenter( mapped.p1() );
    position.setZoomFactor( -log10( mapped.length() ) );
    position.setAngle( -atan2( mapped.dy(), mapped.dx() ) * 180.0 / M_PI );

    if ( position.angle() < 0.0 )
        position.setAngle( position.angle() + 360.0 );

    return position;
}

FractalType FractalPresenter::juliaType( const QPointF& point )
{
    QTransform transform = transformFromPosition( m_position );
    QPointF mapped = transform.map( point );

    FractalType type = m_type;
    type.setFractal( JuliaFractal );
    type.setParameter( mapped );

    return type;
}

Position FractalPresenter::juliaPosition()
{
    double exponent = m_type.exponentType() == IntegralExponent ? m_type.integralExponent() : m_type.realExponent();
    double zoom = ( m_position.zoomFactor() + 0.45 ) / exponent - 0.45;

    Position position;
    position.setZoomFactor( zoom );

    return position;
}

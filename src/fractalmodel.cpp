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

#include "fractalmodel.h"

#include <QTimer>

#include "fractalpresenter.h"
#include "datafunctions.h"
#include "fraqtiveapplication.h"
#include "configurationdata.h"

static void initializeDefaultSettings()
{
    static bool initialized = false;
    if ( !initialized ) {
        ConfigurationData* config = fraqtive()->configuration();

        if ( !config->contains( "Gradient" ) )
            config->setValue( "Gradient", QVariant::fromValue( DataFunctions::defaultGradient() ) );
        if ( !config->contains( "Background" ) )
            config->setValue( "Background", QColor( Qt::black ) );
        if ( !config->contains( "ColorMapping" ) )
            config->setValue( "ColorMapping", QVariant::fromValue( DataFunctions::defaultColorMapping() ) );
        if ( !config->contains( "GeneratorSettings" ) )
            config->setValue( "GeneratorSettings", QVariant::fromValue( DataFunctions::defaultGeneratorSettings() ) );
        if ( !config->contains( "ViewSettings" ) )
            config->setValue( "ViewSettings", QVariant::fromValue( DataFunctions::defaultViewSettings() ) );

        initialized = true;
    }
}

FractalModel::FractalModel( QObject* parent ) : QObject( parent ),
    m_previewPresenter( NULL ),
    m_enabled( false ),
    m_tracking( false ),
    m_hovering( false ),
    m_navigation( false ),
    m_viewMode( NoViewMode )
{
    initializeDefaultSettings();

    m_presenter = new FractalPresenter( this );
    m_presenter->setModel( this );

    m_timer = new QTimer( this );
    m_timer->setInterval( 50 );

    connect( m_timer, SIGNAL( timeout() ), this, SLOT( animate() ) );
}

FractalModel::~FractalModel()
{
}

void FractalModel::setPreviewPresenter( FractalPresenter* presenter )
{
    m_previewPresenter = presenter;
}

void FractalModel::setEnabled( bool enabled )
{
    if ( m_enabled != enabled ) {
        m_enabled = enabled;
        m_presenter->setEnabled( enabled );
    }
}

void FractalModel::setParameters( const FractalType& type, const Position& position )
{
    if ( m_fractalType != type || m_position != position ) {
        storeParameters();
        setParametersInternal( type, position );
    }
}

void FractalModel::storeParameters()
{
    if ( m_navigation ) {
        Navigation navigation = { m_fractalType, m_position };
        m_navigationBackward.prepend( navigation );
        while ( m_navigationBackward.count() > 100 )
            m_navigationBackward.removeLast();
        m_navigationForward.clear();
        emit navigationChanged();
    }
}

void FractalModel::setParametersInternal( const FractalType& type, const Position& position )
{
    m_fractalType = type;
    m_position = position;
    m_presenter->setParameters( type, position );
    emit fractalTypeChanged();
    emit positionChanged();
}

void FractalModel::setFractalType( const FractalType& type )
{
    if ( m_fractalType != type ) {
        storeParameters();
        m_fractalType = type;
        m_presenter->setFractalType( type );
        emit fractalTypeChanged();
    }
}

void FractalModel::setPosition( const Position& position )
{
    if ( m_position != position ) {
        storeParameters();
        m_position = position;
        m_presenter->setPosition( position );
        emit positionChanged();
    }
}

void FractalModel::setDefaultPosition()
{
    setPosition( DataFunctions::defaultPosition( m_fractalType ) );
}

bool FractalModel::hasDefaultPosition() const
{
    return m_position == DataFunctions::defaultPosition( m_fractalType );
}

void FractalModel::setTrackingPosition( const Position& position )
{
    if ( !m_tracking || m_trackingPosition != position ) {
        m_tracking = true;
        m_trackingPosition = position;
        emit trackingChanged();
    }
}

void FractalModel::clearTracking()
{
    if ( m_tracking ) {
        m_tracking = false;
        emit trackingChanged();
    }
}

void FractalModel::setHoveringParameters( const FractalType& type, const Position& position )
{
    if ( !m_hovering || m_hoveringFractalType != type || m_hoveringPosition != position ) {
        m_hovering = true;
        m_hoveringFractalType = type;
        m_hoveringPosition = position;
        if ( m_previewPresenter ) {
            m_previewPresenter->setParameters( type, position );
            m_previewPresenter->setEnabled( true );
        }
        emit hoveringChanged();
    }
}

void FractalModel::clearHovering()
{
    if ( m_hovering ) {
        m_hovering = false;
        if ( m_previewPresenter )
            m_previewPresenter->setEnabled( false );
        emit hoveringChanged();
    }
}

void FractalModel::setNavigationEnabled( bool enable )
{
    if ( m_navigation != enable ) {
        m_navigation = enable;
        m_navigationBackward.clear();
        m_navigationForward.clear();
        emit navigationChanged();
    }
}

bool FractalModel::canNavigateBackward() const
{
    return m_navigation && !m_navigationBackward.isEmpty();
}

bool FractalModel::canNavigateForward() const
{
    return m_navigation && !m_navigationForward.isEmpty();
}

void FractalModel::navigateBackward()
{
    Navigation newNavigation = m_navigationBackward.takeFirst();

    Navigation navigation = { m_fractalType, m_position };
    m_navigationForward.prepend( navigation );
    emit navigationChanged();

    setParametersInternal( newNavigation.m_type, newNavigation.m_position );
}

void FractalModel::navigateForward()
{
    Navigation newNavigation = m_navigationForward.takeFirst();

    Navigation navigation = { m_fractalType, m_position };
    m_navigationBackward.prepend( navigation );
    emit navigationChanged();

    setParametersInternal( newNavigation.m_type, newNavigation.m_position );
}

void FractalModel::setColorSettings( const Gradient& gradient, const QColor& backround, const ColorMapping& mapping )
{
    if ( m_gradient != gradient || m_backgroundColor != backround || m_colorMapping != mapping ) {
        m_gradient = gradient;
        m_backgroundColor = backround;
        m_colorMapping = mapping;
        m_presenter->setColorSettings( gradient, backround, mapping );
        if ( m_previewPresenter )
            m_previewPresenter->setColorSettings( gradient, backround, mapping );
        emit gradientChanged();
        emit backgroundColorChanged();
        emit colorMappingChanged();
    }
}

void FractalModel::setGradient( const Gradient& gradient )
{
    if ( m_gradient != gradient ) {
        m_gradient = gradient;
        m_presenter->setGradient( gradient );
        if ( m_previewPresenter )
            m_previewPresenter->setGradient( gradient );
        emit gradientChanged();
    }
}

void FractalModel::setBackgroundColor( const QColor& color )
{
    if ( m_backgroundColor != color ) {
        m_backgroundColor = color;
        m_presenter->setBackgroundColor( color );
        if ( m_previewPresenter )
            m_previewPresenter->setBackgroundColor( color );
        emit backgroundColorChanged();
    }
}

void FractalModel::setColorMapping( const ColorMapping& mapping )
{
    if ( m_colorMapping != mapping ) {
        m_colorMapping = mapping;
        m_presenter->setColorMapping( mapping );
        if ( m_previewPresenter )
            m_previewPresenter->setColorMapping( mapping );
        emit colorMappingChanged();
    }
}

void FractalModel::setGeneratorSettings( const GeneratorSettings& settings )
{
    if ( m_generatorSettings != settings ) {
        m_generatorSettings = settings;
        m_presenter->setGeneratorSettings( settings );
        if ( m_previewPresenter )
            m_previewPresenter->setGeneratorSettings( settings );
        emit generatorSettingsChanged();
    }
}

void FractalModel::setViewSettings( const ViewSettings& settings )
{
    if ( m_viewSettings != settings ) {
        m_viewSettings = settings;
        m_presenter->setViewSettings( settings );
        if ( m_previewPresenter )
            m_previewPresenter->setViewSettings( settings );
        emit viewSettingsChanged();
    }
}

bool FractalModel::hasDefaultColorSettings() const
{
    ConfigurationData* config = fraqtive()->configuration();
    return config->value( "Gradient" ).value<Gradient>() == m_gradient
        && config->value( "Background" ).value<QColor>() == m_backgroundColor
        && config->value( "ColorMapping" ).value<ColorMapping>() == m_colorMapping;
}

void FractalModel::saveDefaultColorSettings() const
{
    ConfigurationData* config = fraqtive()->configuration();
    config->setValue( "Gradient", QVariant::fromValue( m_gradient ) );
    config->setValue( "Background", QVariant::fromValue( m_backgroundColor ) );
    config->setValue( "ColorMapping", QVariant::fromValue( m_colorMapping ) );
}

void FractalModel::loadDefaultColorSettings()
{
    ConfigurationData* config = fraqtive()->configuration();
    setColorSettings( config->value( "Gradient" ).value<Gradient>(),
        config->value( "Background" ).value<QColor>(),
        config->value( "ColorMapping" ).value<ColorMapping>() );
}

bool FractalModel::hasDefaultGeneratorSettings() const
{
    ConfigurationData* config = fraqtive()->configuration();
    return config->value( "GeneratorSettings" ).value<GeneratorSettings>() == m_generatorSettings;
}

void FractalModel::saveDefaultGeneratorSettings() const
{
    ConfigurationData* config = fraqtive()->configuration();
    config->setValue( "GeneratorSettings", QVariant::fromValue( m_generatorSettings ) );
}

void FractalModel::loadDefaultGeneratorSettings()
{
    ConfigurationData* config = fraqtive()->configuration();
    setGeneratorSettings( config->value( "GeneratorSettings" ).value<GeneratorSettings>() );
}

bool FractalModel::hasDefaultViewSettings() const
{
    ConfigurationData* config = fraqtive()->configuration();
    return config->value( "ViewSettings" ).value<ViewSettings>() == m_viewSettings;
}

void FractalModel::saveDefaultViewSettings() const
{
    ConfigurationData* config = fraqtive()->configuration();
    config->setValue( "ViewSettings", QVariant::fromValue( m_viewSettings ) );
}

void FractalModel::loadDefaultViewSettings()
{
    ConfigurationData* config = fraqtive()->configuration();
    setViewSettings( config->value( "ViewSettings" ).value<ViewSettings>() );
}

void FractalModel::setViewMode( ViewMode mode )
{
    if ( m_viewMode != mode ) {
        m_viewMode = mode;
        updateTimer();
        emit viewModeChanged();
    }
}

void FractalModel::setAnimationSettings( const AnimationSettings& settings )
{
    if ( m_animationSettings != settings ) {
        m_animationSettings = settings;
        updateTimer();
        emit animationSettingsChanged();
    }
}

void FractalModel::updateTimer()
{
    AnimationState lastState = m_animationState;

    // reset state if animation was stopped
    if ( !m_animationSettings.isScrollingEnabled() )
        m_animationState.setScrolling( 0.0 );
    if ( !m_animationSettings.isRotationEnabled() )
        m_animationState.setRotation( 0.0 );

    if ( m_animationState != lastState ) {
        m_presenter->setAnimationState( m_animationState );
        if ( m_previewPresenter )
            m_previewPresenter->setAnimationState( m_animationState );
    }

    bool isScrolling = ( m_animationSettings.isScrollingEnabled() && m_animationSettings.scrollingSpeed() != 0.0 );
    bool isRotating = ( m_viewMode == MeshViewMode && m_animationSettings.isRotationEnabled()
        && m_animationSettings.rotationSpeed() != 0.0 );

    // only enable timer if either animation is active
    if ( isScrolling || isRotating ) {
        if ( !m_timer->isActive() ) {
            m_time.start();
            m_timer->start();
        }
    } else {
        if ( m_timer->isActive() )
            m_timer->stop();
    }
}

static double adjustState( double value )
{
    while ( value > 1.0 )
        value -= 1.0;
    while ( value < 0.0 )
        value += 1.0;
    return value;
}

void FractalModel::animate()
{
    int elapsed = m_time.restart();

    // ignore large changes caused by time settings changes and wrapping at midnight
    if ( elapsed > 0 && elapsed < 5000 ) {
        if ( m_animationSettings.isScrollingEnabled() ) {
            double scrolling = m_animationState.scrolling() + 0.001 * elapsed * m_animationSettings.scrollingSpeed();
            m_animationState.setScrolling( adjustState( scrolling ) );
        }
        if ( m_viewMode == MeshViewMode && m_animationSettings.isRotationEnabled() ) {
            double rotation = m_animationState.rotation() + 0.001 * elapsed * m_animationSettings.rotationSpeed();
            m_animationState.setRotation( adjustState( rotation ) );
        }
        m_presenter->setAnimationState( m_animationState );
        if ( m_previewPresenter )
            m_previewPresenter->setAnimationState( m_animationState );
    }
}

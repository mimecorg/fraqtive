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

#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QPolygonF>
#include <QMetaType>
#include <QColor>

#include "generatorcore.h"

enum Fractal
{
    MandelbrotFractal,
    JuliaFractal
};

enum ExponentType
{
    IntegralExponent,
    RealExponent
};

class FractalType
{
public:
    FractalType();

public:
    void setFractal( Fractal fractal ) { m_fractal = fractal; }
    Fractal fractal() const { return m_fractal; }

    void setParameter( const QPointF& param ) { m_parameter = param; }
    QPointF parameter() const { return m_fractal == JuliaFractal ? m_parameter : QPointF(); }

    void setExponentType( ExponentType type ) { m_exponentType = type; }
    ExponentType exponentType() const { return m_exponentType; }

    void setIntegralExponent( int exponent ) { m_integralExponent = exponent; }
    int integralExponent() const { return m_exponentType == IntegralExponent ? m_integralExponent : 0; }

    void setRealExponent( double exponent ) { m_realExponent = exponent; }
    double realExponent() const { return m_exponentType == RealExponent ? m_realExponent : 0.0; }

    void setVariant( GeneratorCore::Variant variant ) { m_variant = variant; }
    GeneratorCore::Variant variant() const { return m_variant; }

public:
    friend QDataStream& operator <<( QDataStream& stream, const FractalType& type );
    friend QDataStream& operator >>( QDataStream& stream, FractalType& type );

    friend bool operator ==( const FractalType& lhv, const FractalType& rhv );
    friend bool operator !=( const FractalType& lhv, const FractalType& rhv ) { return !( lhv == rhv ); }

private:
    Fractal m_fractal;
    QPointF m_parameter;

    ExponentType m_exponentType;
    int m_integralExponent;
    double m_realExponent;

    GeneratorCore::Variant m_variant;
};

inline FractalType::FractalType() :
    m_fractal( MandelbrotFractal ),
    m_exponentType( IntegralExponent ),
    m_integralExponent( 0 ),
    m_realExponent( 0.0 ),
    m_variant( GeneratorCore::NormalVariant )
{
}

inline bool operator ==( const FractalType& lhv, const FractalType& rhv )
{
    return ( lhv.m_fractal == rhv.m_fractal )
        && ( lhv.m_fractal != JuliaFractal || ( lhv.m_parameter.x() == rhv.m_parameter.x() && lhv.m_parameter.y() == rhv.m_parameter.y() ) ) // exact compare
        && ( lhv.m_exponentType == rhv.m_exponentType )
        && ( lhv.m_exponentType != IntegralExponent || lhv.m_integralExponent == rhv.m_integralExponent )
        && ( lhv.m_exponentType != RealExponent || qFuzzyCompare( lhv.m_realExponent, rhv.m_realExponent ) )
        && ( lhv.m_variant == rhv.m_variant );
}

Q_DECLARE_METATYPE( FractalType )

class Position
{
public:
    Position();

public:
    void setCenter( const QPointF& center ) { m_center = center; }
    QPointF center() const { return m_center; }

    void setZoomFactor( double factor ) { m_zoomFactor = factor; }
    double zoomFactor() const { return m_zoomFactor; }

    void setAngle( double angle ) { m_angle = angle; }
    double angle() const { return m_angle; }

public:
    friend QDataStream& operator <<( QDataStream& stream, const Position& position );
    friend QDataStream& operator >>( QDataStream& stream, Position& position );

    friend bool operator ==( const Position& lhv, const Position& rhv );
    friend bool operator !=( const Position& lhv, const Position& rhv ) { return !( lhv == rhv ); }

private:
    QPointF m_center;
    double m_zoomFactor; // log10
    double m_angle; // degrees
};

inline Position::Position() :
    m_zoomFactor( 0.0 ),
    m_angle( 0.0 )
{
}

inline bool operator ==( const Position& lhv, const Position& rhv )
{
    return ( lhv.m_center.x() == rhv.m_center.x() && lhv.m_center.y() == rhv.m_center.y() ) // exact compare
        && qFuzzyCompare( lhv.m_zoomFactor, rhv.m_zoomFactor )
        && qFuzzyCompare( lhv.m_angle, rhv.m_angle );
}

Q_DECLARE_METATYPE( Position )

class Gradient
{
public:
    Gradient() { }
    Gradient( const QPolygonF& red, const QPolygonF& green, const QPolygonF& blue );

public:
    void setRed( const QPolygonF& red ) { m_red = red; }
    QPolygonF red() const { return m_red; }

    void setGreen( const QPolygonF& green ) { m_green = green; }
    QPolygonF green() const { return m_green; }

    void setBlue( const QPolygonF& blue ) { m_blue = blue; }
    QPolygonF blue() const { return m_blue; }

    bool isEmpty() const;

public:
    friend QDataStream& operator <<( QDataStream& stream, const Gradient& gradient );
    friend QDataStream& operator >>( QDataStream& stream, Gradient& gradient );

    friend bool operator ==( const Gradient& lhv, const Gradient& rhv );
    friend bool operator !=( const Gradient& lhv, const Gradient& rhv ) { return !( lhv == rhv ); }

private:
    QPolygonF m_red;
    QPolygonF m_green;
    QPolygonF m_blue;
};

inline Gradient::Gradient( const QPolygonF& red, const QPolygonF& green, const QPolygonF& blue ) :
    m_red( red ),
    m_green( green ),
    m_blue( blue )
{
}

inline bool operator ==( const Gradient& lhv, const Gradient& rhv )
{
    return ( lhv.m_red == rhv.m_red )
        && ( lhv.m_green == rhv.m_green )
        && ( lhv.m_blue == rhv.m_blue );
}

Q_DECLARE_METATYPE( Gradient )

class ColorMapping
{
public:
    ColorMapping();

public:
    void setMirrored( bool mirrored ) { m_mirrored = mirrored; }
    bool isMirrored() const { return m_mirrored; }

    void setReversed( bool reversed ) { m_reversed = reversed; }
    bool isReversed() const { return m_reversed; }

    void setScale( double scale ) { m_scale = scale; }
    double scale() const { return m_scale; }

    void setOffset( double offset ) { m_offset = offset; }
    double offset() const { return m_offset; }

public:
    friend QDataStream& operator <<( QDataStream& stream, const ColorMapping& mapping );
    friend QDataStream& operator >>( QDataStream& stream, ColorMapping& mapping );

    friend bool operator ==( const ColorMapping& lhv, const ColorMapping& rhv );
    friend bool operator !=( const ColorMapping& lhv, const ColorMapping& rhv ) { return !( lhv == rhv ); }

private:
    bool m_mirrored;
    bool m_reversed;
    double m_scale;
    double m_offset;
};

inline ColorMapping::ColorMapping() :
    m_mirrored( false ),
    m_reversed( false ),
    m_scale( 0.0 ),
    m_offset( 0.0 )
{
}

inline bool operator ==( const ColorMapping& lhv, const ColorMapping& rhv )
{
    return ( lhv.m_mirrored == rhv.m_mirrored )
        && ( lhv.m_reversed == rhv.m_reversed )
        && qFuzzyCompare( lhv.m_scale, rhv.m_scale )
        && qFuzzyCompare( lhv.m_offset, rhv.m_offset );
}

Q_DECLARE_METATYPE( ColorMapping )

class GeneratorSettings
{
public:
    GeneratorSettings();

public:
    void setCalculationDepth( double depth ) { m_calculationDepth = depth; }
    double calculationDepth() const { return m_calculationDepth; }

    void setDetailThreshold( double threshold ) { m_detailThreshold = threshold; }
    double detailThreshold() const { return m_detailThreshold; }

public:
    friend QDataStream& operator <<( QDataStream& stream, const GeneratorSettings& settings );
    friend QDataStream& operator >>( QDataStream& stream, GeneratorSettings& settings );

    friend bool operator ==( const GeneratorSettings& lhv, const GeneratorSettings& rhv );
    friend bool operator !=( const GeneratorSettings& lhv, const GeneratorSettings& rhv ) { return !( lhv == rhv ); }

private:
    double m_calculationDepth;
    double m_detailThreshold;
};

inline GeneratorSettings::GeneratorSettings() :
    m_calculationDepth( 0.0 ),
    m_detailThreshold( 0.0 )
{
}

inline bool operator ==( const GeneratorSettings& lhv, const GeneratorSettings& rhv )
{
    return qFuzzyCompare( lhv.m_calculationDepth, rhv.m_calculationDepth )
        && qFuzzyCompare( lhv.m_detailThreshold, rhv.m_detailThreshold );
}

Q_DECLARE_METATYPE( GeneratorSettings )

enum AntiAliasing
{
    NoAntiAliasing,
    LowAntiAliasing,
    MediumAntiAliasing,
    HighAntiAliasing
};

enum Resolution
{
    LowResolution,
    MediumResolution,
    HighResolution,
    VeryHighResolution
};

class ViewSettings
{
public:
    ViewSettings();

public:
    void setAntiAliasing( AntiAliasing antiAliasing ) { m_antiAliasing = antiAliasing; }
    AntiAliasing antiAliasing() const { return m_antiAliasing; }

    void setMeshResolution( Resolution resolution ) { m_meshResolution = resolution; }
    Resolution meshResolution() const { return m_meshResolution; }

    void setHeightScale( double scale ) { m_heightScale = scale; }
    double heightScale() const { return m_heightScale; }

    void setCameraZoom( double Zoom ) { m_cameraZoom = Zoom; }
    double cameraZoom() const { return m_cameraZoom; }

public:
    friend QDataStream& operator <<( QDataStream& stream, const ViewSettings& settings );
    friend QDataStream& operator >>( QDataStream& stream, ViewSettings& settings );

    friend bool operator ==( const ViewSettings& lhv, const ViewSettings& rhv );
    friend bool operator !=( const ViewSettings& lhv, const ViewSettings& rhv ) { return !( lhv == rhv ); }

private:
    AntiAliasing m_antiAliasing;
    Resolution m_meshResolution;
    double m_heightScale;
    double m_cameraZoom; // degrees
};

inline ViewSettings::ViewSettings() :
    m_antiAliasing( NoAntiAliasing ),
    m_meshResolution( LowResolution ),
    m_heightScale( 0.0 ),
    m_cameraZoom( 0.0 )
{
}

inline bool operator ==( const ViewSettings& lhv, const ViewSettings& rhv )
{
    return lhv.m_antiAliasing == rhv.m_antiAliasing
        && lhv.m_meshResolution == rhv.m_meshResolution
        && qFuzzyCompare( lhv.m_heightScale, rhv.m_heightScale )
        && qFuzzyCompare( lhv.m_cameraZoom, rhv.m_cameraZoom );
}

Q_DECLARE_METATYPE( ViewSettings )

class Bookmark
{
public:
    Bookmark();

public:
    void setFractalType( const FractalType& type ) { m_fractalType = type; }
    FractalType fractalType() const { return m_fractalType; }

    void setPosition( const Position& position ) { m_position = position; }
    Position position() const { return m_position; }

public:
    friend QDataStream& operator <<( QDataStream& stream, const Bookmark& bookmark );
    friend QDataStream& operator >>( QDataStream& stream, Bookmark& bookmark );

    friend bool operator ==( const Bookmark& lhv, const Bookmark& rhv );
    friend bool operator !=( const Bookmark& lhv, const Bookmark& rhv ) { return !( lhv == rhv ); }

private:
    FractalType m_fractalType;
    Position m_position;
};

inline Bookmark::Bookmark()
{
}

inline bool operator ==( const Bookmark& lhv, const Bookmark& rhv )
{
    return ( lhv.m_fractalType == rhv.m_fractalType )
        && ( lhv.m_position == rhv.m_position );
}

Q_DECLARE_METATYPE( Bookmark )

typedef QMap<QString, Bookmark> BookmarkMap;

class Preset
{
public:
    Preset();

public:
    void setGradient( const Gradient& gradient ) { m_gradient = gradient; }
    Gradient gradient() const { return m_gradient; }

    void setBackgroundColor( const QColor& color ) { m_backgroundColor = color; }
    QColor backgroundColor() const { return m_backgroundColor; }

    void setColorMapping( const ColorMapping& mapping ) { m_colorMapping = mapping; }
    ColorMapping colorMapping() const { return m_colorMapping; }

public:
    friend QDataStream& operator <<( QDataStream& stream, const Preset& preset );
    friend QDataStream& operator >>( QDataStream& stream, Preset& preset );

    friend bool operator ==( const Preset& lhv, const Preset& rhv );
    friend bool operator !=( const Preset& lhv, const Preset& rhv ) { return !( lhv == rhv ); }

private:
    Gradient m_gradient;
    QColor m_backgroundColor;
    ColorMapping m_colorMapping;
};

inline Preset::Preset()
{
}

inline bool operator ==( const Preset& lhv, const Preset& rhv )
{
    return ( lhv.m_gradient == rhv.m_gradient )
        && ( lhv.m_backgroundColor == rhv.m_backgroundColor )
        && ( lhv.m_colorMapping == rhv.m_colorMapping );
}

Q_DECLARE_METATYPE( Preset )

typedef QMap<QString, Preset> PresetMap;

class AnimationSettings
{
public:
    AnimationSettings();

public:
    void setScrollingEnabled( bool enabled ) { m_scrollingEnabled = enabled; }
    bool isScrollingEnabled() const { return m_scrollingEnabled; }

    void setScrollingSpeed( double speed ) { m_scrollingSpeed = speed; }
    double scrollingSpeed() const { return m_scrollingSpeed; }

    void setRotationEnabled( bool enabled ) { m_rotationEnabled = enabled; }
    bool isRotationEnabled() const { return m_rotationEnabled; }

    void setRotationSpeed( double speed ) { m_rotationSpeed = speed; }
    double rotationSpeed() const { return m_rotationSpeed; }

public:
    friend bool operator ==( const AnimationSettings& lhv, const AnimationSettings& rhv );
    friend bool operator !=( const AnimationSettings& lhv, const AnimationSettings& rhv ) { return !( lhv == rhv ); }

private:
    bool m_scrollingEnabled;
    bool m_rotationEnabled;
    double m_scrollingSpeed;
    double m_rotationSpeed;
};

inline AnimationSettings::AnimationSettings() :
    m_scrollingEnabled( false ),
    m_rotationEnabled( false ),
    m_scrollingSpeed( 0.0 ),
    m_rotationSpeed( 0.0 )
{
}

inline bool operator ==( const AnimationSettings& lhv, const AnimationSettings& rhv )
{
    return lhv.m_scrollingEnabled == rhv.m_scrollingEnabled
        && lhv.m_rotationEnabled == rhv.m_rotationEnabled
        && qFuzzyCompare( lhv.m_scrollingSpeed, rhv.m_scrollingSpeed )
        && qFuzzyCompare( lhv.m_rotationSpeed, rhv.m_rotationSpeed );
}

class AnimationState
{
public:
    AnimationState();

public:
    void setScrolling( double scrolling ) { m_scrolling = scrolling; }
    double scrolling() const { return m_scrolling; }

    void setRotation( double rotation ) { m_rotation = rotation; }
    double rotation() const { return m_rotation; }

public:
    friend bool operator ==( const AnimationState& lhv, const AnimationState& rhv );
    friend bool operator !=( const AnimationState& lhv, const AnimationState& rhv ) { return !( lhv == rhv ); }

private:
    double m_scrolling;
    double m_rotation;
};

inline AnimationState::AnimationState() :
    m_scrolling( 0.0 ),
    m_rotation( 0.0 )
{
}

inline bool operator ==( const AnimationState& lhv, const AnimationState& rhv )
{
    return qFuzzyCompare( lhv.m_scrolling, rhv.m_scrolling )
        && qFuzzyCompare( lhv.m_rotation, rhv.m_rotation );
}

void registerDataStructures();

#endif

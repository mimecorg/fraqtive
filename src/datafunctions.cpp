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

#include "datafunctions.h"
#include "fractaldata.h"

namespace DataFunctions
{

Position defaultPosition( const FractalType& type )
{
    Position position;
    position.setZoomFactor( -0.45 );

    if ( type.fractal() == MandelbrotFractal && type.exponentType() == IntegralExponent ) {
        if ( type.variant() == GeneratorCore::NormalVariant && type.integralExponent() == 2 ) {
            position.setCenter( QPointF( -0.7, 0 ) );
        } else if ( type.variant() == GeneratorCore::NormalVariant && type.integralExponent() == 4 ) {
            position.setCenter( QPointF( -0.2, 0 ) );
        } else if ( type.variant() == GeneratorCore::ConjugateVariant && type.integralExponent() == 2 ) {
            position.setCenter( QPointF( -0.3, 0 ) );
            position.setZoomFactor( -0.6 );
        } else if ( type.variant() == GeneratorCore::AbsoluteVariant && type.integralExponent() == 2 ) {
            position.setCenter( QPointF( -0.5, -0.5 ) );
            position.setZoomFactor( -0.5 );
        } else if ( type.variant() == GeneratorCore::AbsoluteVariant && type.integralExponent() == 3 ) {
            position.setCenter( QPointF( -0.15, 0.15 ) );
        } else if ( type.variant() == GeneratorCore::AbsoluteImVariant && type.integralExponent() == 2 ) {
            position.setCenter( QPointF( -0.5, 0 ) );
            position.setZoomFactor( -0.5 );
        } else if ( type.variant() == GeneratorCore::AbsoluteImVariant && type.integralExponent() == 3 ) {
            position.setCenter( QPointF( 0, 0.15 ) );
        }
    }

    return position;
}

Gradient defaultGradient()
{
    QPolygonF red;
    red.append( QPointF( 0.0, 0.0 ) );
    red.append( QPointF( 0.1667, 1.0 ) );
    red.append( QPointF( 0.5, 0.0 ) );
    red.append( QPointF( 0.8333, 1.0 ) );
    red.append( QPointF( 1.0, 0.0 ) );

    QPolygonF green;
    green.append( QPointF( 0.0, 0.0 ) );
    green.append( QPointF( 0.25, 1.0 ) );
    green.append( QPointF( 0.5, 0.0 ) );
    green.append( QPointF( 0.6667, 1.0 ) );
    green.append( QPointF( 1.0, 0.0 ) );

    QPolygonF blue;
    blue.append( QPointF( 0.0, 0.0 ) );
    blue.append( QPointF( 0.3333, 1.0 ) );
    blue.append( QPointF( 0.5, 0.0 ) );
    blue.append( QPointF( 0.75, 1.0 ) );
    blue.append( QPointF( 1.0, 0.0 ) );

    Gradient gradient;
    gradient.setRed( red );
    gradient.setGreen( green );
    gradient.setBlue( blue );

    return gradient;
}

ColorMapping defaultColorMapping()
{
    ColorMapping mapping;

    mapping.setScale( 0.056 );

    return mapping;
}

GeneratorSettings defaultGeneratorSettings()
{
    GeneratorSettings settings;

    settings.setCalculationDepth( 2.5 );
    settings.setDetailThreshold( 0.9 );

    return settings;
}

ViewSettings defaultViewSettings()
{
    ViewSettings settings;

    settings.setAntiAliasing( MediumAntiAliasing );
    settings.setMeshResolution( MediumResolution );
    settings.setHeightScale( 0.14 );
    settings.setCameraZoom( 24.0 );

    return settings;
}


static QPolygonF interpolateCubic( const QPolygonF& polygon )
{
    QPolygonF interpolated;

    if ( !polygon.isEmpty() )
        interpolated.append( polygon.first() );

    for ( int i = 1; i < polygon.count(); i++ ) {
        QPointF p1 = polygon.at( i - 1 );
        QPointF p2 = polygon.at( i );
        if ( ( p2.x() - p1.x() ) > 0.03 ) {
            QPainterPath path;
            path.moveTo( p1 );
            path.cubicTo( ( p1.x() + p2.x() ) / 2, p1.y(), ( p1.x() + p2.x() ) / 2, p2.y(), p2.x(), p2.y() );
            int segments = qBound( 1, qRound( ( p2.x() - p1.x() ) * 50.0 ), 10 );
            for ( int j = 1; j <= segments; j++ ) {
                QPointF point = path.pointAtPercent( (double)j / (double)segments );
                interpolated.append( point );
            }
        } else {
            interpolated.append( p2 );
        }
    }

    return interpolated;
}

static double lerp( const QPolygonF& points, int index, double x )
{
    QLineF line( points.at( index - 1 ), points.at( index ) );
    if ( line.dx() == 0.0 )
        return line.y2();
    return line.y1() + line.dy() * ( x - line.x1() ) / line.dx();
}

static QGradientStop makeStop( double x, double ry, double gy, double by )
{
    return QGradientStop( x, QColor( qRound( 255.0 * ry ), qRound( 255.0 * gy ), qRound( 255.0 * by ) ) );
}

QGradientStops calculateGradientStops( const Gradient& gradient )
{
    QPolygonF red = interpolateCubic( gradient.red() );
    QPolygonF green = interpolateCubic( gradient.green() );
    QPolygonF blue = interpolateCubic( gradient.blue() );

    QGradientStops stops;

    if ( red.isEmpty() || green.isEmpty() || blue.isEmpty() )
        return stops;

    stops.append( makeStop( 0.0, red.first().y(), green.first().y(), blue.first().y() ) );

    double x = 0.0;
    int ri = 1, gi = 1, bi = 1;
    while ( ri < red.count() && gi < green.count() && bi < blue.count() ) {
        double rx = red.at( ri ).x();
        double gx = green.at( gi ).x();
        double bx = blue.at( bi ).x();
        x = qMin( qMin( rx, gx ), bx );
        stops.append( makeStop( x, lerp( red, ri, x ), lerp( green, gi, x ), lerp( blue, bi, x ) ) );
        if ( rx == x )
            ri++;
        if ( gx == x )
            gi++;
        if ( bx == x )
            bi++;
    }

    return stops;
}

void fillGradientCache( const Gradient& gradient, QRgb* cache, int size )
{
    if ( gradient.isEmpty() ) {
        for ( int i = 0; i < size; i++ )
            cache[ i ] = qRgb( 0, 0, 0 );
        return;
    }

    QPolygonF red = interpolateCubic( gradient.red() );
    QPolygonF green = interpolateCubic( gradient.green() );
    QPolygonF blue = interpolateCubic( gradient.blue() );

    int ri = 1, gi = 1, bi = 1;
    for ( int i = 0; i < size; i++ ) {
        double x = (double)i / (double)( size - 1 );
        while ( red.at( ri ).x() < x )
            ri++;
        while ( green.at( gi ).x() < x )
            gi++;
        while ( blue.at( bi ).x() < x )
            bi++;
        double ry = lerp( red, ri, x );
        double gy = lerp( green, gi, x );
        double by = lerp( blue, bi, x );
        cache[ i ] = qRgb( qRound( 255.0 * ry ), qRound( 255.0 * gy ), qRound( 255.0 * by ) );
    }
}

ColorMapper::ColorMapper( const QRgb* gradientCache, int gradientSize, QRgb backgroundColor, const ColorMapping& mapping ) :
    m_gradientCache( gradientCache ),
    m_gradientSize( gradientSize ),
    m_backgroundColor( backgroundColor ),
    m_mapping( mapping )
{
}

QRgb ColorMapper::map( double value ) const
{
    if ( value == 0.0 )
        return m_backgroundColor;

    int index;
    if ( m_mapping.isMirrored() ) {
        int scaled = (int)( ( m_mapping.scale() * value + 2 * m_mapping.offset() ) * m_gradientSize );
        index = scaled % ( 2 * m_gradientSize );
        if ( index >= m_gradientSize )
            index = 2 * m_gradientSize - index - 1;
    } else {
        int scaled = (int)( ( m_mapping.scale() * value + m_mapping.offset() ) * m_gradientSize );
        index = scaled % m_gradientSize;
    }

    if ( m_mapping.isReversed() )
        index = m_gradientSize - index - 1;

    return m_gradientCache[ index ];
}

template<int A, int B, int C>
static inline QRgb maskedSum( QRgb color[ 3 ][ 3 ], int mask )
{
    QRgb middle = ( color[ 1 ][ 1 ] & mask );
    QRgb sides = ( color[ 0 ][ 1 ] & mask ) + ( color[ 2 ][ 1 ] & mask ) + ( color[ 1 ][ 0 ] & mask ) + ( color[ 1 ][ 2 ] & mask );
    QRgb corners = ( color[ 0 ][ 0 ] & mask ) + ( color[ 2 ][ 0 ] & mask ) + ( color[ 0 ][ 2 ] & mask ) + ( color[ 2 ][ 2 ] & mask );
    return A * middle + B * sides + C * corners;
}

template<int A, int B, int C>
static inline QRgb calcAntiAliased( QRgb color[ 3 ][ 3 ] )
{
    QRgb sum = maskedSum<A, B, C>( color, 0xf0f0f0 );
    QRgb carry = maskedSum<A, B, C>( color, 0x0f0f0f ) & 0xf0f0f0;
    return ( ( sum + carry ) >> 4 ) | 0xff000000;
}

template<int A, int B, int C>
static void drawAntiAliased( QImage& image, const QPoint& point, const FractalData* data, const QRect& region, const ColorMapper& mapper )
{
    QRgb color[ 3 ][ 3 ];

    int stride = data->stride();
    int width = region.width();

    for ( int y = 0; y < region.height(); y++ ) {
        const double* src = data->buffer() + ( y + region.top() ) * stride + region.left() ;
        QRgb* dest = reinterpret_cast<QRgb*>( image.scanLine( y + point.y() ) ) + point.x();

        for ( int i = 0; i < 3; i++ ) {
            color[ i ][ 1 ] = mapper.map( src[ i * stride ] );
            color[ i ][ 2 ] = mapper.map( src[ i * stride + 1 ] );
        }

        for ( int x = 0; x < width; x++ ) {
            for ( int i = 0; i < 3; i++ ) {
                color[ i ][ 0 ] = color[ i ][ 1 ];
                color[ i ][ 1 ] = color[ i ][ 2 ];
                color[ i ][ 2 ] = mapper.map( src[ i * stride + x + 2 ] );
            }
            dest[ x ] = calcAntiAliased<A, B, C>( color );
        }
    }
}

void drawImage( QImage& image, const FractalData* data, const QRect& region, const ColorMapper& mapper, AntiAliasing antiAliasing )
{
    drawImage( image, region.topLeft(), data, region, mapper, antiAliasing );
}

void drawImage( QImage& image, const QPoint& point, const FractalData* data, const QRect& region, const ColorMapper& mapper, AntiAliasing antiAliasing )
{
    switch ( antiAliasing ) {
        case NoAntiAliasing:
            drawAntiAliased<16, 0, 0>( image, point, data, region, mapper );
            break;
        case LowAntiAliasing:
            drawAntiAliased<12, 1, 0>( image, point, data, region, mapper );
            break;
        case MediumAntiAliasing:
            drawAntiAliased<8, 1, 1>( image, point, data, region, mapper );
            break;
        case HighAntiAliasing:
            drawAntiAliased<4, 2, 1>( image, point, data, region, mapper );
            break;
    }
}

GeneratorCore::Functor* createFunctor( const FractalType& type )
{
    switch ( type.exponentType() ) {
        case IntegralExponent:
            if ( type.fractal() == MandelbrotFractal ) {
                return GeneratorCore::createMandelbrotFastFunctor( type.integralExponent(), type.variant() );
            } else {
                return GeneratorCore::createJuliaFastFunctor( type.parameter().x(),
                    type.parameter().y(), type.integralExponent(), type.variant() );
            }
            break;

        case RealExponent:
            if ( type.fractal() == MandelbrotFractal ) {
                return GeneratorCore::createMandelbrotFunctor( type.realExponent(), type.variant() );
            } else if ( type.fractal() == JuliaFractal ) {
                return GeneratorCore::createJuliaFunctor( type.parameter().x(),
                    type.parameter().y(), type.realExponent(), type.variant() );
            }
            break;
    }

    return NULL;
}

#if defined( HAVE_SSE2 )

GeneratorCore::FunctorSSE2* createFunctorSSE2( const FractalType& type )
{
    switch ( type.exponentType() ) {
        case IntegralExponent:
            if ( GeneratorCore::isSSE2Available() ) {
                if ( type.fractal() == MandelbrotFractal ) {
                    return GeneratorCore::createMandelbrotFunctorSSE2( type.integralExponent(), type.variant() );
                } else if ( type.fractal() == JuliaFractal ) {
                    return GeneratorCore::createJuliaFunctorSSE2( type.parameter().x(),
                        type.parameter().y(), type.integralExponent(), type.variant() );
                }
            }
            break;

        default:
            break;
    }

    return NULL;
}

#endif

} // namespace DataFunctions

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

#include "generatorcore.h"

#include <qglobal.h>

#include <math.h>
#include <cstdlib>

#if defined( HAVE_SSE2 )
# include <emmintrin.h>
#endif

// see http://wiki.mimec.org/wiki/Fraqtive/Generator_Core for a description of this code

namespace GeneratorCore
{

static const double BailoutRadius = 64.0;

#if defined( Q_CC_MSVC )
# pragma float_control( precise, off )
# pragma intrinsic( log, sqrt, exp, atan2, sin, cos, fabs )
#endif

static const double BailoutLog = log( 2.0 * log( BailoutRadius ) );

static inline double calculateResult( int maxIterations, int count, double final, double exponent )
{
    if ( count == 0 )
        return 0.0;

    double value = ( maxIterations - count ) + ( BailoutLog - log( log( sqrt( final ) ) ) ) / log( exponent );

    return sqrt( value );
}

template<Variant VARIANT>
static void adjust( double& /*zx*/, double& /*zy*/ );

template<>
inline void adjust<NormalVariant>( double& /*zx*/, double& /*zy*/ )
{
}

template<>
inline void adjust<ConjugateVariant>( double& /*zx*/, double& zy )
{
    zy = -zy;
}

template<>
inline void adjust<AbsoluteVariant>( double& zx, double& zy )
{
    zx = fabs( zx );
    zy = fabs( zy );
}

template<>
inline void adjust<AbsoluteImVariant>( double& /*zx*/, double& zy )
{
    zy = fabs( zy );
}

template<Variant VARIANT>
static inline double calculate( double x, double y, double cx, double cy, double exponent, int maxIterations )
{
    double zx = x;
    double zy = y;
    double radius;

    double exp2 = 0.5 * exponent;

    for ( int k = maxIterations; k > 0; k-- ) {
        adjust<VARIANT>( zx, zy );

        double zxx = zx * zx;
        double zyy = zy * zy;
        radius = zxx + zyy;

        if ( radius >= BailoutRadius )
            return calculateResult( maxIterations, k, radius, exponent );

        double z = exp( log( radius ) * exp2 );
        double fi = exponent * atan2( zy, zx );

        zx = z * cos( fi ) + cx;
        zy = z * sin( fi ) + cy;
    }

    return 0.0;
}

#if defined( Q_CC_MSVC )
# pragma float_control( precise, on )
# pragma function( log, sqrt, exp, atan2, sin, cos, fabs )
#endif

class MandelbrotParams
{
public:
    MandelbrotParams( double exponent ) :
        m_exponent( exponent )
    {
    }

protected:
    double m_exponent;
};

template<Variant VARIANT>
class MandelbrotFunctor : public Functor, public MandelbrotParams
{
public:
    MandelbrotFunctor( const MandelbrotParams& params ) : MandelbrotParams( params )
    {
    }

    double operator()( double zx, double zy, int maxIterations )
    {
        return calculate<VARIANT>( zx, zy, zx, zy, m_exponent, maxIterations );
    }
};

class JuliaParams : public MandelbrotParams
{
public:
    JuliaParams( double cx, double cy, double exponent ) : MandelbrotParams( exponent ),
        m_cx( cx ),
        m_cy( cy )
    {
    }

protected:
    double m_cx;
    double m_cy;
};

template<Variant VARIANT>
class JuliaFunctor : public Functor, public JuliaParams
{
public:
    JuliaFunctor( const JuliaParams& params ) : JuliaParams( params )
    {
    }

    double operator()( double zx, double zy, int maxIterations )
    {
        return calculate<VARIANT>( zx, zy, m_cx, m_cy, m_exponent, maxIterations );
    }
};

template<typename BASE, template<Variant VARIANT> class FACTORY>
class VariantDispatcher
{
public:
    template<typename PARAMS>
    static BASE* create( Variant variant, const PARAMS& params )
    {
        switch ( variant ) {
            case NormalVariant:
                return FACTORY<NormalVariant>::create( params );
            case ConjugateVariant:
                return FACTORY<ConjugateVariant>::create( params );
            case AbsoluteVariant:
                return FACTORY<AbsoluteVariant>::create( params );
            case AbsoluteImVariant:
                return FACTORY<AbsoluteImVariant>::create( params );
        }
        return NULL;
    }
};

template<typename BASE, template<Variant VARIANT> class FUNCTOR>
class FunctorFactory
{
public:
    template<typename PARAMS>
    static BASE* create( Variant variant, const PARAMS& params )
    {
        return VariantDispatcher<BASE, InnerFactory>::create( variant, params );
    }

private:
    template<Variant VARIANT>
    class InnerFactory
    {
    public:
        template<typename PARAMS>
        static BASE* create( const PARAMS& params )
        {
            return new FUNCTOR<VARIANT>( params );
        }
    };
};

Functor* createMandelbrotFunctor( double exponent, Variant variant )
{
    return FunctorFactory<Functor, MandelbrotFunctor>::create( variant, MandelbrotParams( exponent ) );
}

Functor* createJuliaFunctor( double cx, double cy, double exponent, Variant variant )
{
    return FunctorFactory<Functor, JuliaFunctor>::create( variant, JuliaParams( cx, cy, exponent ) );
}

template<int N>
static inline void calculatePower( double& zx, double& zy, double& radius )
{
    if ( N % 2 == 0 ) {
        calculatePower<N / 2>( zx, zy, radius );

        double zxx = zx * zx;
        double zyy = zy * zy;
        double zxy = zx * zy;

        zx = zxx - zyy;
        zy = zxy + zxy;
    } else {
        double zx2 = zx;
        double zy2 = zy;
        calculatePower<N - 1>( zx2, zy2, radius );

        double zxx2 = zx * zx2;
        double zxy2 = zx * zy2;
        double zyx2 = zy * zx2;
        double zyy2 = zy * zy2;

        zx = zxx2 - zyy2;
        zy = zxy2 + zyx2;
    }
}

template<>
inline void calculatePower<2>( double& zx, double& zy, double& radius )
{
    double zxx = zx * zx;
    double zyy = zy * zy;
    double zxy = zx * zy;

    zx = zxx - zyy;
    zy = zxy + zxy;
    radius = zxx + zyy;
}

template<>
inline void calculatePower<1>( double& /*zx*/, double& /*zy*/, double& /*radius*/ )
{
}

template<int N, Variant VARIANT>
static double calculateFast( double x, double y, double cx, double cy, int maxIterations )
{
    double zx = x;
    double zy = y;

    for ( int k = maxIterations; k > 0; k-- ) {
        adjust<VARIANT>( zx, zy );

        double radius;
        calculatePower<N>( zx, zy, radius );

        if ( radius >= BailoutRadius )
            return calculateResult( maxIterations, k, radius, N );

        zx += cx;
        zy += cy;
    }

    return 0.0;
}

class MandelbrotFastParams
{
public:
    MandelbrotFastParams()
    {
    }
};

template<int N, Variant VARIANT>
class MandelbrotFastFunctor : public Functor, public MandelbrotFastParams
{
public:
    MandelbrotFastFunctor( const MandelbrotFastParams& params ) : MandelbrotFastParams( params )
    {
    }

    double operator()( double zx, double zy, int maxIterations )
    {
        return calculateFast<N, VARIANT>( zx, zy, zx, zy, maxIterations );
    }
};

class JuliaFastParams : public MandelbrotFastParams
{
public:
    JuliaFastParams( double cx, double cy ) : MandelbrotFastParams(),
        m_cx( cx ),
        m_cy( cy )
    {
    }

protected:
    double m_cx;
    double m_cy;
};

template<int N, Variant VARIANT>
class JuliaFastFunctor : public Functor, public JuliaFastParams
{
public:
    JuliaFastFunctor( const JuliaFastParams& params ) : JuliaFastParams( params )
    {
    }

    double operator()( double zx, double zy, int maxIterations )
    {
        return calculateFast<N, VARIANT>( zx, zy, m_cx, m_cy, maxIterations );
    }

};

template<typename BASE, template<int N, Variant VARIANT> class FACTORY, int EXPONENT = MaxExponent>
class ExponentDispatcher
{
public:
    template<typename PARAMS>
    static BASE* create( int exponent, Variant variant, const PARAMS& params )
    {
        if ( exponent == EXPONENT )
            return VariantDispatcher<BASE, FactoryAdapter>::create( variant, params );
        return ExponentDispatcher<BASE, FACTORY, EXPONENT - 1>::create( exponent, variant, params );
    }

private:
    template<Variant VARIANT>
    class FactoryAdapter
    {
    public:
        template<typename PARAMS>
        static BASE* create( const PARAMS& params )
        {
            return FACTORY<EXPONENT, VARIANT>::create( params );
        }
    };
};

template<typename BASE, template<int N, Variant VARIANT> class FACTORY>
class ExponentDispatcher<BASE, FACTORY, 1>
{
public:
    template<typename PARAMS>
    static BASE* create( int /*exponent*/, Variant /*variant*/, const PARAMS& /*params*/ )
    {
        return NULL;
    }
};

template<typename BASE, template<int N, Variant VARIANT> class FUNCTOR>
class FastFunctorFactory
{
public:
    template<typename PARAMS>
    static BASE* create( int exponent, Variant variant, const PARAMS& params )
    {
        return ExponentDispatcher<BASE, InnerFactory>::create( exponent, variant, params );
    }

private:
    template<int N, Variant VARIANT>
    class InnerFactory
    {
    public:
        template<typename PARAMS>
        static BASE* create( const PARAMS& params )
        {
            return new FUNCTOR<N, VARIANT>( params );
        }
    };
};

Functor* createMandelbrotFastFunctor( int exponent, Variant variant )
{
    return FastFunctorFactory<Functor, MandelbrotFastFunctor>::create( exponent, variant, MandelbrotFastParams() );
}

Functor* createJuliaFastFunctor( double cx, double cy, int exponent, Variant variant )
{
    return FastFunctorFactory<Functor, JuliaFastFunctor>::create( exponent, variant, JuliaFastParams( cx, cy ) );
}

void generatePreview( const Input& input, const Output& output, Functor* functor, int maxIterations )
{
    for ( int y = 0; y < output.m_height; y += CellSize ) {
        double* row = output.m_buffer + output.m_stride * y;
        for ( int x = 0; x < output.m_width; x += CellSize ) {
            double zx = input.m_x + input.m_ca * x + input.m_sa * y;
            double zy = input.m_y - input.m_sa * x + input.m_ca * y;
            row[ x ] = ( *functor )( zx, zy, maxIterations );
        }
    }
}

static inline bool checkThreshold( double p1, double p2, double threshold )
{
    double pmin, pmax;
    if ( p1 < p2 )
        pmin = p1, pmax = p2;
    else
        pmin = p2, pmax = p1;

    if ( pmin == 0.0 && pmax != 0.0 )
        return true;
    
    if ( ( pmax - pmin ) > threshold )
        return true;

    return false;
}

static inline bool checkThreshold( double p1, double p2, double p3, double p4, double threshold )
{
    return checkThreshold( p1, p2, threshold )
        || checkThreshold( p3, p4, threshold )
        || checkThreshold( p1, p3, threshold )
        || checkThreshold( p2, p4, threshold );
}

void generateDetails( const Input& input, const Output& output, Functor* functor, int maxIterations, double threshold )
{
    for ( int y = 0; y < output.m_height; y += CellSize ) {
        double* row = output.m_buffer + output.m_stride * y;
        for ( int x = 0; x < output.m_width - CellSize; x += CellSize ) {
            double p1 = row[ x ];
            double p2 = row[ x + CellSize ];
            if ( checkThreshold( p1, p2, threshold ) ) {
                for ( int i = 1; i < CellSize; i++ ) {
                    double zx = input.m_x + input.m_ca * ( x + i ) + input.m_sa * y;
                    double zy = input.m_y - input.m_sa * ( x + i ) + input.m_ca * y;
                    row[ x + i ] = ( *functor )( zx, zy, maxIterations );
                }
            }
        }
    }

    for ( int y = 0; y < output.m_height - CellSize; y += CellSize ) {
        double* row = output.m_buffer + output.m_stride * y;
        for ( int x = 0; x < output.m_width; x += CellSize ) {
            double p1 = row[ x ];
            double p2 = row[ output.m_stride * CellSize + x ];
            if ( checkThreshold( p1, p2, threshold ) ) {
                for ( int i = 1; i < CellSize; i++ ) {
                    double zx = input.m_x + input.m_ca * x + input.m_sa * ( y + i );
                    double zy = input.m_y - input.m_sa * x + input.m_ca * ( y + i );
                    row[ output.m_stride * i + x ] = ( *functor )( zx, zy, maxIterations );
                }
            }
        }
    }

    for ( int y = 0; y < output.m_height - CellSize; y += CellSize ) {
        double* row = output.m_buffer + output.m_stride * y;
        for ( int x = 0; x < output.m_width - CellSize; x += CellSize ) {
            double p1 = row[ x ];
            double p2 = row[ x + CellSize ];
            double p3 = row[ output.m_stride * CellSize + x ];
            double p4 = row[ output.m_stride * CellSize + x + CellSize ];
            if ( checkThreshold( p1, p2, p3, p4, threshold ) ) {
                for ( int i = 1; i < CellSize; i++ ) {
                    for ( int j = 1; j < CellSize; j++ ) {
                        double zx = input.m_x + input.m_ca * ( x + j ) + input.m_sa * ( y + i );
                        double zy = input.m_y - input.m_sa * ( x + j ) + input.m_ca * ( y + i );
                        row[ output.m_stride * i + x + j ] = ( *functor )( zx, zy, maxIterations );
                    }
                }
            }
        }
    }
}

void interpolate( const Output& output )
{
    for ( int y = 0; y < output.m_height; y += CellSize ) {
        double* row = output.m_buffer + output.m_stride * y;
        for ( int x = 0; x < output.m_width - CellSize; x += CellSize ) {
            double p1 = row[ x ];
            double p2 = row[ x + CellSize ];
            for ( int i = 1; i < CellSize; i++ )
                row[ x + i ] = (double)( CellSize - i ) / (double)CellSize * p1 + (double)i / (double)CellSize * p2;
        }
    }
    for ( int y = 0; y < output.m_height - CellSize; y += CellSize ) {
        double* row = output.m_buffer + output.m_stride * y;
        for ( int x = 0; x < output.m_width; x++ ) {
            double p1 = row[ x ];
            double p2 = row[ output.m_stride * CellSize + x ];
            for ( int i = 1; i < CellSize; i++ )
                row[ output.m_stride * i + x ] = (double)( CellSize - i ) / (double)CellSize * p1 + (double)i / (double)CellSize * p2;
        }
    }
}

#if defined( HAVE_SSE2 )

#if defined( Q_CC_MSVC )
# define ALIGNXMM( var ) __declspec(align(16)) var
#else
# define ALIGNXMM( var ) var __attribute__((aligned(16)))
#endif

enum CPUFeatures
{
    MMX = 1,
    SSE = 2,
    SSE2 = 4
};

// based on qdrawhelper.cpp
static int detectCPUFeatures()
{
#if defined( __x86_64__ ) || defined( __ia64__ ) || defined( Q_OS_WIN64 )
    return MMX | SSE | SSE2;
#elif defined( __i386__ ) || defined( _M_IX86 )
    int result = 0;
#if defined( Q_CC_GNU )
    asm( "push %%ebx\n"
         "pushf\n"
         "pop %%eax\n"
         "mov %%eax, %%ebx\n"
         "xor $0x00200000, %%eax\n"
         "push %%eax\n"
         "popf\n"
         "pushf\n"
         "pop %%eax\n"
         "xor %%edx, %%edx\n"
         "xor %%ebx, %%eax\n"
         "jz 1f\n"
         "mov $0x00000001, %%eax\n"
         "cpuid\n"
         "1:\n"
         "pop %%ebx\n"
         "mov %%edx, %0\n"
        : "=r" ( result )
        :
        : "%eax", "%ecx", "%edx"
        );
#elif defined ( Q_OS_WIN )
    _asm {
        push eax
        push ebx
        push ecx
        push edx
        pushfd
        pop eax
        mov ebx, eax
        xor eax, 00200000h
        push eax
        popfd
        pushfd
        pop eax
        mov edx, 0
        xor eax, ebx
        jz skip
        mov eax, 1
        cpuid
        mov result, edx
    skip:
        pop edx
        pop ecx
        pop ebx
        pop eax
    }
#endif
    int features = 0;
    if ( result & ( 1 << 23 ) )
        features |= MMX;
    if ( result & ( 1 << 25 ) )
        features |= SSE;
    if ( result & ( 1 << 26 ) )
        features |= SSE2;
    return features;
#else
    return 0;
#endif
}

static const int AvailableCPUFeatures = detectCPUFeatures();

bool isSSE2Available()
{
    return AvailableCPUFeatures & SSE2;
}

template<int N>
static inline void calculatePowerSSE2( __m128d& zx, __m128d& zy, __m128d& radius )
{
    if ( N % 2 == 0 ) {
        calculatePowerSSE2<N / 2>( zx, zy, radius );

        __m128d zxx = _mm_mul_pd( zx, zx );
        __m128d zyy = _mm_mul_pd( zy, zy );
        __m128d zxy = _mm_mul_pd( zx, zy );

        zx = _mm_sub_pd( zxx, zyy );
        zy = _mm_add_pd( zxy, zxy );
    } else {
        __m128d zx2 = zx;
        __m128d zy2 = zy;
        calculatePowerSSE2<N - 1>( zx2, zy2, radius );

        __m128d zxx2 = _mm_mul_pd( zx, zx2 );
        __m128d zxy2 = _mm_mul_pd( zx, zy2 );
        __m128d zyx2 = _mm_mul_pd( zy, zx2 );
        __m128d zyy2 = _mm_mul_pd( zy, zy2 );

        zx = _mm_sub_pd( zxx2, zyy2 );
        zy = _mm_add_pd( zxy2, zyx2 );
    }
}

template<>
inline void calculatePowerSSE2<2>( __m128d& zx, __m128d& zy, __m128d& radius )
{
    __m128d zxx = _mm_mul_pd( zx, zx );
    __m128d zyy = _mm_mul_pd( zy, zy );
    __m128d zxy = _mm_mul_pd( zx, zy );

    zx = _mm_sub_pd( zxx, zyy );
    zy = _mm_add_pd( zxy, zxy );
    radius = _mm_add_pd( zxx, zyy );
}

template<>
inline void calculatePowerSSE2<1>( __m128d& /*zx*/, __m128d& /*zy*/, __m128d& /*radius*/ )
{
}

template<Variant VARIANT>
static void adjustSSE2( __m128d& /*zx*/, __m128d& /*zy*/ );

template<>
inline void adjustSSE2<NormalVariant>( __m128d& /*zx*/, __m128d& /*zy*/ )
{
}

template<>
inline void adjustSSE2<ConjugateVariant>( __m128d& /*zx*/, __m128d& zy )
{
    __m128d mask = _mm_castsi128_pd( _mm_set_epi32( int( 0x80000000 ), 0, int( 0x80000000 ), 0 ) );
    zy = _mm_xor_pd( mask, zy );
}

template<>
inline void adjustSSE2<AbsoluteVariant>( __m128d& zx, __m128d& zy )
{
    __m128d mask = _mm_castsi128_pd( _mm_set_epi32( 0x7fffffff, int( 0xffffffff ), 0x7fffffff, int( 0xffffffff ) ) );
    zx = _mm_and_pd( zx, mask );
    zy = _mm_and_pd( zy, mask );
}

template<>
inline void adjustSSE2<AbsoluteImVariant>( __m128d& /*zx*/, __m128d& zy )
{
    __m128d mask = _mm_castsi128_pd( _mm_set_epi32( 0x7fffffff, int( 0xffffffff ), 0x7fffffff, int( 0xffffffff ) ) );
    zy = _mm_and_pd( zy, mask );
}

template<int N, Variant VARIANT>
static inline bool calculateStepSSE2( int k, __m128d& zx, __m128d& zy, __m128d cx, __m128d cy, __m128d rmax, int count[], double final[] )
{
    adjustSSE2<VARIANT>( zx, zy );

    __m128d radius;
    calculatePowerSSE2<N>( zx, zy, radius );

    int mask = _mm_movemask_pd( _mm_cmpge_pd( radius, rmax ) );

    zx = _mm_add_pd( zx, cx );
    zy = _mm_add_pd( zy, cy );

    if ( mask ) {
        if ( mask & 1 ) {
            if ( !count[ 0 ] ) {
                count[ 0 ] = k;
                _mm_storel_pd( &final[ 0 ], radius );

                if ( count[ 1 ] )
                    return true;
            }
        }
        if ( mask & 2 ) {
            if ( !count[ 1 ] ) {
                count[ 1 ] = k;
                _mm_storeh_pd( &final[ 1 ], radius );

                if ( count[ 0 ] )
                    return true;
            }
        }
    }

    return false;
}

template<int N, Variant VARIANT, int STEPS>
class RepeatStepsSSE2
{
public:
    static inline bool calculate( int k, __m128d& zx, __m128d& zy, __m128d cx, __m128d cy, __m128d rmax, int count[], double final[] )
    {
        if ( calculateStepSSE2<N, VARIANT>( k, zx, zy, cx, cy, rmax, count, final ) )
            return true;

        if ( RepeatStepsSSE2<N, VARIANT, STEPS - 1>::calculate( k - 1, zx, zy, cx, cy, rmax, count, final ) )
            return true;

        return false;
    }

    static const int Steps = STEPS;
};

template<int N, Variant VARIANT>
class RepeatStepsSSE2<N, VARIANT, 0>
{
public:
    static inline bool calculate( int /*k*/, __m128d& /*zx*/, __m128d& /*zy*/, __m128d /*cx*/, __m128d /*cy*/, __m128d /*rmax*/, int /*count*/[], double /*final*/[] )
    {
        return false;
    }
};

template<int N, Variant VARIANT>
class AutoStepsSSE2 : public RepeatStepsSSE2<N, VARIANT, ( N <= 5 ) ? 2 : 1>
{
};

template<int N, Variant VARIANT>
static inline void calculateSSE2( double result[], double x[], double y[], double cx[], double cy[], int maxIterations )
{
    __m128d zx = _mm_load_pd( x );
    __m128d zy = _mm_load_pd( y );

    __m128d rcx = _mm_load_pd( cx );
    __m128d rcy = _mm_load_pd( cy ); 

    __m128d rmax = _mm_set1_pd( BailoutRadius ); 

    int count[ 2 ] = { 0, 0 };
    double final[ 2 ] = { 0.0, 0.0 };

    for ( int k = maxIterations; k > 0; k -= AutoStepsSSE2<N, VARIANT>::Steps ) {
        if ( AutoStepsSSE2<N, VARIANT>::calculate( k, zx, zy, rcx, rcy, rmax, count, final ) )
            break;
    }

    result[ 0 ] = count[ 0 ] ? calculateResult( maxIterations, count[ 0 ], final[ 0 ], N ) : 0.0;
    result[ 1 ] = count[ 1 ] ? calculateResult( maxIterations, count[ 1 ], final[ 1 ], N ) : 0.0;
}

template<int N, Variant VARIANT>
class MandelbrotFunctorSSE2 : public FunctorSSE2, public MandelbrotFastParams
{
public:
    MandelbrotFunctorSSE2( const MandelbrotFastParams& params ) : MandelbrotFastParams( params )
    {
    }

    void operator()( double result[], double zx[], double zy[], int maxIterations )
    {
        calculateSSE2<N, VARIANT>( result, zx, zy, zx, zy, maxIterations );
    }
};

template<int N, Variant VARIANT>
class JuliaFunctorSSE2 : public FunctorSSE2, public JuliaFastParams
{
public:
    JuliaFunctorSSE2( const JuliaFastParams& params ) : JuliaFastParams( params )
    {
    }

    void operator()( double result[], double zx[], double zy[], int maxIterations )
    {
        ALIGNXMM( double cx[ 2 ] ) = { m_cx, m_cx };
        ALIGNXMM( double cy[ 2 ] ) = { m_cy, m_cy };
        calculateSSE2<N, VARIANT>( result, zx, zy, cx, cy, maxIterations );
    }
};

FunctorSSE2* createMandelbrotFunctorSSE2( int exponent, Variant variant )
{
    return FastFunctorFactory<FunctorSSE2, MandelbrotFunctorSSE2>::create( exponent, variant, MandelbrotFastParams() );
}

FunctorSSE2* createJuliaFunctorSSE2( double cx, double cy, int exponent, Variant variant )
{
    return FastFunctorFactory<FunctorSSE2, JuliaFunctorSSE2>::create( exponent, variant, JuliaFastParams( cx, cy ) );
}

void generatePreviewSSE2( const Input& input, const Output& output, FunctorSSE2* functor, int maxIterations )
{
    ALIGNXMM( double zx[ 2 ] );
    ALIGNXMM( double zy[ 2 ] );

    double result[ 2 ];

    for ( int y = 0; y < output.m_height; y += CellSize ) {
        double* row = output.m_buffer + output.m_stride * y;
        for ( int x = 0; x < output.m_width; x += CellSize ) {
            zx[ 0 ] = input.m_x + input.m_ca * x + input.m_sa * y;
            zx[ 1 ] = zx[ 0 ] + input.m_ca * CellSize;
            zy[ 0 ] = input.m_y - input.m_sa * x + input.m_ca * y;
            zy[ 1 ] = zy[ 0 ] - input.m_sa * CellSize;
            ( *functor )( result, zx, zy, maxIterations );
            row[ x ] = result[ 0 ];
            if ( x + CellSize < output.m_width )
                row[ x + CellSize ] = result[ 1 ];
        }
    }
}

void generateDetailsSSE2( const Input& input, const Output& output, FunctorSSE2* functor, int maxIterations, double threshold )
{
    ALIGNXMM( double zx[ 2 ] );
    ALIGNXMM( double zy[ 2 ] );

    double result[ 2 ];

    for ( int y = 0; y < output.m_height; y += CellSize ) {
        double* row = output.m_buffer + output.m_stride * y;
        for ( int x = 0; x < output.m_width - CellSize; x += CellSize ) {
            double p1 = row[ x ];
            double p2 = row[ x + CellSize ];
            if ( checkThreshold( p1, p2, threshold ) ) {
                for ( int i = 1; i < CellSize; i += 2 ) {
                    zx[ 0 ] = input.m_x + input.m_ca * ( x + i ) + input.m_sa * y;
                    zx[ 1 ] = zx[ 0 ] + input.m_ca;
                    zy[ 0 ] = input.m_y - input.m_sa * ( x + i ) + input.m_ca * y;
                    zy[ 1 ] = zy[ 0 ] - input.m_sa;
                    ( *functor )( result, zx, zy, maxIterations );
                    row[ x + i ] = result[ 0 ];
                    if ( i + 1 < CellSize )
                        row[ x + i + 1 ] = result[ 1 ];
                }
            }
        }
    }

    for ( int y = 0; y < output.m_height - CellSize; y += CellSize ) {
        double* row = output.m_buffer + output.m_stride * y;
        for ( int x = 0; x < output.m_width; x += CellSize ) {
            double p1 = row[ x ];
            double p2 = row[ output.m_stride * CellSize + x ];
            if ( checkThreshold( p1, p2, threshold ) ) {
                for ( int i = 1; i < CellSize; i += 2 ) {
                    zx[ 0 ] = input.m_x + input.m_ca * x + input.m_sa * ( y + i );
                    zx[ 1 ] = zx[ 0 ] + input.m_sa;
                    zy[ 0 ] = input.m_y - input.m_sa * x + input.m_ca * ( y + i );
                    zy[ 1 ] = zy[ 0 ] + input.m_ca;
                    ( *functor )( result, zx, zy, maxIterations );
                    row[ output.m_stride * i + x ] = result[ 0 ];
                    if ( i + 1 < CellSize )
                        row[ output.m_stride * ( i + 1 ) + x ] = result[ 1 ];
                }
            }
        }
    }

    for ( int y = 0; y < output.m_height - CellSize; y += CellSize ) {
        double* row = output.m_buffer + output.m_stride * y;
        for ( int x = 0; x < output.m_width - CellSize; x += CellSize ) {
            double p1 = row[ x ];
            double p2 = row[ x + CellSize ];
            double p3 = row[ output.m_stride * CellSize + x ];
            double p4 = row[ output.m_stride * CellSize + x + CellSize ];
            if ( checkThreshold( p1, p2, p3, p4, threshold ) ) {
                for ( int i = 1; i < CellSize; i++ ) {
                    for ( int j = 1; j < CellSize; j += 2 ) {
                        zx[ 0 ] = input.m_x + input.m_ca * ( x + j ) + input.m_sa * ( y + i );
                        zx[ 1 ] = zx[ 0 ] + input.m_ca;
                        zy[ 0 ] = input.m_y - input.m_sa * ( x + j ) + input.m_ca * ( y + i );
                        zy[ 1 ] = zy[ 0 ] - input.m_sa;
                        ( *functor )( result, zx, zy, maxIterations );
                        row[ output.m_stride * i + x + j ] = result[ 0 ];
                        if ( j + 1 < CellSize )
                            row[ output.m_stride * i + x + j + 1 ] = result[ 1 ];
                    }
                }
            }
        }
    }
}

#endif // defined( HAVE_SSE2 )

} // namespace GeneratorCore

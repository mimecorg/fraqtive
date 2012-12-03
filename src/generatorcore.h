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

#ifndef GENERATORCORE_H
#define GENERATORCORE_H

#if defined( Q_OS_WIN64 ) && defined( _M_X64 )
#undef HAVE_SSE2
#endif

namespace GeneratorCore
{

static const int MaxExponent = 6;

enum Variant
{
    NormalVariant,
    ConjugateVariant,
    AbsoluteVariant,
    AbsoluteImVariant
};

class Functor
{
public:
    virtual ~Functor() {}

    virtual double operator()( double zx, double zy, int maxIterations ) = 0;
};

Functor* createMandelbrotFunctor( double exponent, Variant variant );
Functor* createJuliaFunctor( double cx, double cy, double exponent, Variant variant );

Functor* createMandelbrotFastFunctor( int exponent, Variant variant );
Functor* createJuliaFastFunctor( double cx, double cy, int exponent, Variant variant );

static const int CellSize = 3;

struct Input
{
    double m_x;
    double m_y;
    double m_sa;
    double m_ca;
};

struct Output
{
    double* m_buffer;
    int m_stride;
    int m_width;  // M * CellSize + 1
    int m_height; // N * CellSize + 1
};

void generatePreview( const Input& input, const Output& output, Functor* functor, int maxIterations );
void generateDetails( const Input& input, const Output& output, Functor* functor, int maxIterations, double threshold );

void interpolate( const Output& output );

#if defined( HAVE_SSE2 )

bool isSSE2Available();

class FunctorSSE2
{
public:
    virtual ~FunctorSSE2() {}

    virtual void operator()( double result[], double zx[], double zy[], int maxIterations ) = 0;
};

FunctorSSE2* createMandelbrotFunctorSSE2( int exponent, Variant variant );
FunctorSSE2* createJuliaFunctorSSE2( double cx, double cy, int exponent, Variant variant ); 

void generatePreviewSSE2( const Input& input, const Output& output, FunctorSSE2* functor, int maxIterations );
void generateDetailsSSE2( const Input& input, const Output& output, FunctorSSE2* functor, int maxIterations, double threshold );

#endif // defined( HAVE_SSE2 )

} // namespace GeneratorCore

#endif

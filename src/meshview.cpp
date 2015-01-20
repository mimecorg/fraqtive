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

#include "meshview.h"

#include <math.h>

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

#include <GL/glu.h>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include "fractalpresenter.h"
#include "datafunctions.h"

MeshView::MeshView( QWidget* parent, FractalPresenter* presenter ) : QGLWidget( parent ),
    m_presenter( presenter ),
    m_textureId( 0 ),
    m_textureDirty( false ),
    m_vertexArray( NULL ),
    m_textureCoordArray( NULL ),
    m_maximumDepth( 0.0 ),
    m_averageHeight( 0.0 ),
    m_rotation( 0.0 ),
    m_angle( -45.0 ),
    m_tracking( NoTracking )
{
    setContextMenuPolicy( Qt::PreventContextMenu );
    setFocusPolicy( Qt::WheelFocus );
}

MeshView::~MeshView()
{
    if ( m_textureId != 0 ) {
        makeCurrent();
        glDeleteTextures( 1, &m_textureId );
    }

    delete[] m_vertexArray;
    delete[] m_textureCoordArray;
}

QImage MeshView::image()
{
    return grabFrameBuffer();
}

void MeshView::clearView()
{
    delete[] m_vertexArray;
    m_vertexArray = NULL;
    delete[] m_textureCoordArray;
    m_textureCoordArray = NULL;

    m_resolution = QSize();

    m_updatedRegion = QRect();

    updateGL();
}

void MeshView::transformView( const QTransform& /*transform*/ )
{
    // not used in 3D mode
}

void MeshView::initialUpdate( const FractalData* data )
{
    if ( m_resolution.isNull() || m_resolution != data->size() ) {
        m_resolution = data->size();

        int vertices = m_resolution.width() * m_resolution.height();

        delete[] m_vertexArray;
        m_vertexArray = new float[ 3 * vertices ];
        delete[] m_textureCoordArray;
        m_textureCoordArray = new float[ vertices ];

        initializeVertices();
    }

    m_maximumDepth = sqrt( (double)m_presenter->maximumIterations() );

    m_updatedRegion = QRect();
    partialUpdate( data );
}

void MeshView::partialUpdate( const FractalData* data )
{
    if ( m_resolution.isNull() || m_resolution != data->size() )
        return;

    const QList<QRect> validRegions = data->validRegions();

    if ( validRegions.count() > 0 && validRegions.first().top() == 0 && validRegions.first().bottom() > m_updatedRegion.bottom() ) {
        int top = m_updatedRegion.bottom() + 1;
        QRect region( 0, top, validRegions.first().width(), validRegions.first().bottom() - top + 1 );
        updateVertices( data, region );

        m_updatedRegion = validRegions.first();

        updateGL();
    }
}

void MeshView::fullUpdate( const FractalData* data )
{
    initialUpdate( data );
}

void MeshView::setColorSettings( const Gradient& gradient, const QColor& backgroundColor, const ColorMapping& mapping )
{
    if ( m_gradient != gradient ) {
        m_gradient = gradient;
        m_textureDirty = true;
    }

    m_backgroundColor = backgroundColor;
    m_colorMapping = mapping;

    updateGL();
}

void MeshView::setGradient( const Gradient& gradient )
{
    m_gradient = gradient;
    m_textureDirty = true;

    updateGL();
}

void MeshView::setBackgroundColor( const QColor& backgroundColor )
{
    m_backgroundColor = backgroundColor;

    updateGL();
}

void MeshView::setColorMapping( const ColorMapping& mapping )
{
    m_colorMapping = mapping;

    updateGL();
}

void MeshView::setViewSettings( const ViewSettings& settings )
{
    m_settings = settings;

    switch ( settings.meshResolution() ) {
        case LowResolution:
            m_presenter->setResolution( QSize( 256, 256 ) );
            break;
        case MediumResolution:
            m_presenter->setResolution( QSize( 512, 512 ) );
            break;
        case HighResolution:
            m_presenter->setResolution( QSize( 1024, 1024 ) );
            break;
        case VeryHighResolution:
            m_presenter->setResolution( QSize( 2048, 2048 ) );
            break;
    }

    updateGL();
}

void MeshView::setAnimationState( const AnimationState& state )
{
    m_animationState = state;

    updateGL();
}

void MeshView::initializeGL()
{
    glEnable( GL_DEPTH_TEST );

    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void MeshView::resizeGL( int width, int height )
{
    glViewport( 0, 0, width, height );
}

#ifndef GL_MIRRORED_REPEAT_ARB
# define GL_MIRRORED_REPEAT_ARB 0x8370
#endif

static bool hasMirroredRepeat()
{
    static bool checked = false;
    static bool result = false;

    if ( !checked ) {
        QStringList extensions = QString::fromLatin1( (const char*)glGetString( GL_EXTENSIONS ) ).split( ' ' );
        if ( extensions.contains( "GL_ARB_texture_mirrored_repeat" ) || extensions.contains( "GL_IBM_texture_mirrored_repeat" ) )
            result = true;
        checked = true;
    }

    return result;
}

#ifndef APIENTRY
# define APIENTRY
#endif

typedef void ( APIENTRY *_glLockArraysEXT )( GLint first, GLsizei count );
typedef void ( APIENTRY *_glUnlockArraysEXT )(); 

static _glLockArraysEXT p_glLockArraysEXT = NULL;
static _glUnlockArraysEXT p_glUnlockArraysEXT = NULL;

static bool hasLockArrays( const QGLContext* context )
{
    static bool checked = false;
    static bool result = false;

    if ( !checked ) {
        p_glLockArraysEXT = (_glLockArraysEXT)context->getProcAddress( "glLockArraysEXT" );
        p_glUnlockArraysEXT = (_glUnlockArraysEXT)context->getProcAddress( "glUnlockArraysEXT" );
        if ( p_glLockArraysEXT != NULL && p_glUnlockArraysEXT != NULL )
            result = true;
        checked = true;
    }

    return result;
}

static const double CameraDistance = 40.0;

static const double NearClipping = 10.0;
static const double FarClipping = 70.0;

void MeshView::paintGL()
{
    qglClearColor( m_backgroundColor );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if ( m_resolution.isEmpty() )
        return;

    if ( m_textureDirty ) {
        generateTexture();
        m_textureDirty = false;
    }

    double offset = m_colorMapping.offset() + m_animationState.scrolling();
    if ( offset > 1.0 )
        offset -= 1.0;
    double scale = m_colorMapping.scale();

    glMatrixMode( GL_TEXTURE );
    glLoadIdentity();

    if ( m_colorMapping.isMirrored() && hasMirroredRepeat() ) {
        if ( m_colorMapping.isReversed() )
            glTranslated( 2.0 * offset + 1.0, 0.0, 0.0 );
        else
            glTranslated( 2.0 * offset, 0.0, 0.0 );
        glScaled( scale, 1.0, 1.0 );

        glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT_ARB );
    } else {
        if ( m_colorMapping.isReversed() ) {
            glTranslated( -offset, 0.0, 0.0 );
            glScaled( -scale, 1.0, 1.0 );
        } else {
            glTranslated( offset, 0.0, 0.0 );
            glScaled( scale, 1.0, 1.0 );
        }

        glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    }

    GLint viewport[ 4 ];
    glGetIntegerv( GL_VIEWPORT, viewport );

    double vw = viewport[ 2 ];
    double vh = viewport[ 3 ];

    double fy = tan( 0.5 * m_settings.cameraZoom() * M_PI / 180.0 ) * NearClipping;
    double fx = fy * vw / vh;

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -fx, fx, -fy, fy, NearClipping, FarClipping );

    double rotation = m_rotation - m_animationState.rotation() * 360.0;

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslated( 0.0, 0.0, -CameraDistance );
    glRotated( m_angle, 1.0, 0.0, 0.0 );
    glRotated( rotation, 0.0, 0.0, 1.0 );
    glScaled( 1.0, 1.0, m_settings.heightScale() );
    glTranslated( 0.0, 0.0, m_averageHeight );

    double plane[ 4 ] = { 0.0, 0.0, 1.0, m_maximumDepth - 0.001 };

    glClipPlane( GL_CLIP_PLANE0, plane );
    glEnable( GL_CLIP_PLANE0 );

    int width = m_resolution.width();

    if ( hasLockArrays( context() ) )
        p_glLockArraysEXT( 0, m_resolution.height() * m_resolution.width() );

    int* indexArray = new int[ 2 * width ];

    for ( int y = 0; y < m_resolution.height() - 1; y++ ) {
        int offset = y * width;
        for ( int x = 0; x < width; x++ ) {
            indexArray[ 2 * x ] = x + offset;
            indexArray[ 2 * x + 1 ] = x + offset + width;
        }
        glDrawElements( GL_TRIANGLE_STRIP, 2 * width - 2, GL_UNSIGNED_INT, indexArray );
    }

    delete[] indexArray;

    if ( hasLockArrays( context() ) )
        p_glUnlockArraysEXT();
}

void MeshView::generateTexture()
{
    const int size = 1024;

    QImage image( size, 1, QImage::Format_ARGB32 );

    QRgb* line = (QRgb*)image.scanLine( 0 );
    DataFunctions::fillGradientCache( m_gradient, line, size );

    QImage textureImage = QGLWidget::convertToGLFormat( image );

    if ( m_textureId == 0 )
        glGenTextures( 1, &m_textureId );

    glBindTexture( GL_TEXTURE_1D, m_textureId );
    gluBuild1DMipmaps( GL_TEXTURE_1D, 4, size, GL_RGBA, GL_UNSIGNED_BYTE, textureImage.bits() );

    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glEnable( GL_TEXTURE_1D );
}

// should always be greater than maximumDepth
static const float InfiniteDepth = 100.0f;

void MeshView::initializeVertices()
{
    int width = m_resolution.width();

    for ( int y = 0; y < m_resolution.height(); y++ ) {
        float* vertices = m_vertexArray + y * 3 * width;
        float* coords = m_textureCoordArray + y * width;
        float vy = 10.0f - ( 20.0f * y ) / (float)( m_resolution.height() - 1 );
        for ( int x = 0; x < width; x++ ) {
            vertices[ 3 * x ] = ( 20.0f * x ) / (float)( width - 1 ) - 10.0f;
            vertices[ 3 * x + 1 ] = vy;
            vertices[ 3 * x + 2 ] = -InfiniteDepth;
            coords[ x ] = 0.0f;
        }
    }

    m_averageRowHeight.fill( 0.0, m_resolution.height() );
    m_averageHeight = 0.0;

    glVertexPointer( 3, GL_FLOAT, 0, m_vertexArray );
    glEnableClientState( GL_VERTEX_ARRAY );
    glTexCoordPointer( 1, GL_FLOAT, 0, m_textureCoordArray );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
}

void MeshView::updateVertices( const FractalData* data, const QRect& region )
{
    int stride = data->stride();
    int width = region.width();

    for ( int y = region.top(); y <= region.bottom(); y++ ) {
        const double* src = data->buffer() + y * stride + region.left();
        float* vertices = m_vertexArray + y * 3 * width;
        float* coords = m_textureCoordArray + y * width;

        double sum = 0.0;
        int count = 0;

        for ( int x = 0; x < width; x++ ) {
            double value = src[ x ];
            if ( value > 0.0 )
                sum += value, count++;
            else
                value = InfiniteDepth;
            vertices[ 3 * x + 2 ] = -(float)value;
            coords[ x ] = (float)value;
        }

        if ( count > 0 )
            m_averageRowHeight[ y ] = sum / (double)count;
    }

    double sum = 0.0;
    int count = 0;

    for ( int i = 0; i < m_averageRowHeight.count(); i++ ) {
        double value = m_averageRowHeight[ i ];
        if ( value > 0.0 )
            sum += value, count++;
    }

    if ( count > 0 )
        m_averageHeight = sum / (double)count;
}

void MeshView::mousePressEvent( QMouseEvent* e )
{
    if ( m_resolution.isEmpty() )
        return;

    if ( m_tracking != NoTracking ) {
        m_tracking = NoTracking;
        return;
    }

    if ( e->button() == Qt::LeftButton ) {
        if ( e->modifiers() & Qt::ControlModifier )
            m_tracking = ZoomInOut;
        else
            m_tracking = RotateXY;
    } else if ( e->button() == Qt::RightButton ) {
        m_tracking = ZoomInOut;
    }

    m_lastPos = e->pos();
}

void MeshView::mouseMoveEvent( QMouseEvent* e )
{
    if ( m_tracking == NoTracking )
        return;

    switch ( m_tracking ) {
        case RotateXY: {
            m_rotation += 0.4 * ( e->pos().x() - m_lastPos.x() );
            m_angle += 0.4 * ( e->pos().y() - m_lastPos.y() );
            updateGL();
            break;
        }

        case ZoomInOut: {
            m_presenter->adjustCameraZoom( -0.02 * ( e->pos().y() - m_lastPos.y() ) );
            break;
        }

        default:
            break;
    }

    m_lastPos = e->pos();
}

void MeshView::mouseReleaseEvent( QMouseEvent* /*e*/ )
{
    m_tracking = NoTracking;
}

void MeshView::wheelEvent( QWheelEvent* e )
{
    double delta = e->delta() / 120.0;

    m_presenter->adjustCameraZoom( delta );
}

void MeshView::keyPressEvent( QKeyEvent* e )
{
    if ( m_resolution.isEmpty() || ( e->modifiers() & Qt::AltModifier ) != 0 ) {
        e->ignore();
        return;
    }

    if ( e->key() == Qt::Key_Escape && m_tracking != NoTracking ) {
        e->accept();
        m_tracking = NoTracking;
        return;
    }

    switch ( e->key() ) {
        case Qt::Key_Up:
        case Qt::Key_Down:
            m_angle += ( e->key() == Qt::Key_Up ) ? 5.0 : -5.0;
            e->accept();
            updateGL();
            break;

        case Qt::Key_Left:
        case Qt::Key_Right:
            m_rotation += ( e->key() == Qt::Key_Left ) ? 5.0 : -5.0;
            e->accept();
            updateGL();
            break;

        case Qt::Key_Plus:
        case Qt::Key_Equal:
            m_presenter->adjustCameraZoom( 1.0 );
            e->accept();
            break;

        case Qt::Key_Minus:
            m_presenter->adjustCameraZoom( -1.0 );
            e->accept();
            break;

        default:
            e->ignore();
            break;
    }
}

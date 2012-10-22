/****************************************************************************
* Simple XML-based UI builder for Qt4
* Copyright (C) 2007-2012 Michał Męciński
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright
*      notice, this list of conditions and the following disclaimer in the
*      documentation and/or other materials provided with the distribution.
*   3. Neither the name of the copyright holder nor the names of the
*      contributors may be used to endorse or promote products derived from
*      this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
****************************************************************************/

#include "macstyle.h"

#if !defined( XMLUI_NO_STYLE_MAC )

#include <QStyleOption>
#include <QPainter>
#include <QLibrary>
#include <QApplication>
#include <QMainWindow>
#include <QAbstractButton>
#include <QScrollArea>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QLayout>
#include <QDebug>

#if !defined( XMLUI_NO_STYLE_PLUGIN )
#include <QStylePlugin>
#endif

using namespace XmlUi;

MacStyle::MacStyle() : QMacStyle()
{
}

MacStyle::~MacStyle()
{
}

static void drawHighlightFrameMac( QPainter* painter, const QWidget* widget, const QStyleOptionComplex* option, const QRect& rect )
{
    const QSize size( 50, 34 );
    
    QStyleOptionButton opt;
    opt.initFrom( widget );
    opt.direction = option->direction;
    opt.fontMetrics = option->fontMetrics;
    opt.palette = option->palette;
    opt.rect = QRect( QPoint(), size );
    opt.state = option->state | QStyle::State_Sunken;
    
    // grab original rendering
    QPixmap pixmap( size );
    pixmap.fill( QColor( Qt::transparent ) );
    {
        QPainter p( &pixmap );
        widget->style()->drawControl( QStyle::CE_PushButton, &opt, &p, 0 );
    }
    
    pixmap = pixmap.copy( QRect( QPoint(), size ).adjusted( 6, 6, -6, -6 ) ); // 38x22
    
    // create circle pixmap
    {
        QPixmap pix = QPixmap( pixmap.height(), pixmap.height() );
        const QSize pixSize( pix.width() /2, pix.height() );
        pix.fill( QColor( Qt::transparent ) );
        {
            QPainter p( &pix );
            
            p.drawPixmap( QPoint(), pixmap.copy( QRect( QPoint(), pixSize ) ) );
            p.drawPixmap( QPoint( pix.width() /2, 0 ), pixmap.copy( QRect( QPoint( pixmap.width() -pix.width() /2, 0 ), pixSize ) ) );
        }
        
        pixmap = pix;
    }
    
    // paint final pixmap
    {
        QPixmap topLeft;
        QPixmap topRight;
        QPixmap bottomLeft;
        QPixmap bottomRight;
        QPixmap left;
        QPixmap top;
        QPixmap right;
        QPixmap bottom;
        QPixmap center;
        
        const int width = qBound( 0, rect.width() /2, pixmap.width() /2 );
        const int height = qBound( 0, rect.height() /2, pixmap.height() /2 );
        const QSize pixSize( width, height );
        
        topLeft = pixmap.copy( QRect( QPoint(), pixSize ) );
        topRight = pixmap.copy( QRect( QPoint( pixmap.width() -pixSize.width(), 0 ), pixSize ) );
        bottomLeft = pixmap.copy( QRect( QPoint( 0, pixmap.height() -pixSize.height() ), pixSize ) );
        bottomRight = pixmap.copy( QRect( QPoint( pixmap.width() -pixSize.width(), pixmap.height() -pixSize.height() ), pixSize ) );
        left = pixmap.copy( QRect( QPoint( 0, pixmap.height() /2 ), QSize( pixSize.width(), 1 ) ) );
        top = pixmap.copy( QRect( QPoint( pixmap.width() /2, 0 ), QSize( 1, pixSize.height() ) ) );
        right = pixmap.copy( QRect( QPoint( pixmap.width() -pixSize.width(), pixmap.height() /2 ), QSize( pixSize.width(), 1 ) ) );
        bottom = pixmap.copy( QRect( QPoint( pixmap.width() /2, pixmap.height() -pixSize.height() ), QSize( 1, pixSize.height() ) ) );
        center = pixmap.copy( QRect( QPoint( pixmap.width() /2, pixmap.height() /2 ), QSize( 1, 1 ) ) );
        
        QPixmap pix = QPixmap( rect.size() );
        pix.fill( QColor( Qt::transparent ) );
        
        QRect r( QPoint(), rect.size() );
        
        {
            QPainter p( &pix );
            
            p.drawPixmap( r.topLeft(), topLeft );
            p.drawPixmap( r.topRight() +QPoint( -topRight.width(), 0 ), topRight );
            p.drawPixmap( r.bottomLeft() +QPoint( 0, -bottomLeft.height() ), bottomLeft );
            p.drawPixmap( r.bottomRight() +QPoint( -bottomRight.width(), -bottomRight.height() ), bottomRight );
            
            p.drawPixmap( QRect( QPoint( 0, topRight.height() ), QSize( left.width(), rect.height() -( topRight.height() +bottomRight.height() ) ) ), left );
            p.drawPixmap( QRect( QPoint( topLeft.width(), 0 ), QSize( r.width() -( topLeft.width() +topRight.width() ), top.height() ) ), top );
            p.drawPixmap( QRect( QPoint( r.width() -topRight.width() -1, topRight.height() ), QSize( right.width(), r.height() -( topRight.height() +bottomRight.height() ) ) ), right );
            p.drawPixmap( QRect( QPoint( bottomLeft.width(), r.height() -bottom.height() -1 ), QSize( r.width() -( bottomLeft.width() +bottomRight.width() ), bottom.height() ) ), bottom );
            
            p.drawPixmap( rect.adjusted( left.width(), top.height(), -right.width(), -bottom.height() ), center );
        }
        
        pixmap = pix;
    }
    
    // draw scaled content
    painter->setRenderHint( QPainter::SmoothPixmapTransform );
    painter->drawPixmap( rect.topLeft(), pixmap );
}

void MacStyle::drawComplexControl( ComplexControl control, const QStyleOptionComplex* option,
    QPainter* painter, const QWidget* widget ) const
{
    switch ( control ) {
        case CC_ToolButton:
            if ( option->state & State_AutoRaise ) {
                if ( const QStyleOptionToolButton* optionToolButton = qstyleoption_cast<const QStyleOptionToolButton*>( option ) ) {
                    QRect buttonRect = subControlRect( control, option, SC_ToolButton, widget );
                    QStyle::State buttonState = option->state & ~State_Sunken;
                    if ( option->state & State_Sunken ) {
                        if ( optionToolButton->activeSubControls & SC_ToolButton )
                            buttonState |= State_Sunken;
                        else if ( optionToolButton->activeSubControls & SC_ToolButtonMenu )
                            buttonState |= State_MouseOver;
                    }
                    bool selected = buttonState & State_MouseOver && option->state & State_Enabled;
                    bool checked = buttonState & State_On;
                    bool sunken = buttonState & State_Sunken;
                    if ( selected || checked || sunken ) {
                        if ( sunken || selected && checked )
                            drawHighlightFrameMac( painter, widget, option, buttonRect );
                        else if ( checked )
                            drawHighlightFrameMac( painter, widget, option, buttonRect );
                        else
                            drawHighlightFrameMac( painter, widget, option, buttonRect );
                    }
                    QStyleOptionToolButton optionLabel = *optionToolButton;
                    optionLabel.state = buttonState;
                    int fw = pixelMetric( PM_DefaultFrameWidth, option, widget );
                    optionLabel.rect = buttonRect.adjusted( fw, fw, -fw, -fw );
                    drawControl( CE_ToolButtonLabel, &optionLabel, painter, widget );
                    if ( optionToolButton->subControls & SC_ToolButtonMenu ) {
                        QRect menuRect = subControlRect( control, option, SC_ToolButtonMenu, widget );
                        menuRect.adjust( -1, 0, 0, 0 );
                        if ( sunken || optionToolButton->state & State_Sunken && optionToolButton->activeSubControls & SC_ToolButtonMenu )
                            drawHighlightFrameMac( painter, widget, option, menuRect );
                        else if ( selected )
                            drawHighlightFrameMac( painter, widget, option, menuRect );
                        QStyleOptionToolButton optionArrow = *optionToolButton;
                        optionArrow.rect = menuRect.adjusted( 2, 3, -5, -5 );
                        drawPrimitive( PE_IndicatorArrowDown, &optionArrow, painter, widget );
                    } else if ( optionToolButton->features & QStyleOptionToolButton::HasMenu ) {
                        int size = pixelMetric( PM_MenuButtonIndicator, option, widget );
                        QRect rect = optionToolButton->rect;
                        QStyleOptionToolButton optionArrow = *optionToolButton;
                        optionArrow.rect = QRect( rect.right() + 4 - size, rect.height() - size + 4, size - 5, size - 5 );
                        drawPrimitive( PE_IndicatorArrowDown, &optionArrow, painter, widget );
                    }
                    return;
            }
            break;
        }

        default:
            break;
    }

    QMacStyle::drawComplexControl( control, option, painter, widget );
}

#if !defined( XMLUI_NO_STYLE_PLUGIN )

namespace XmlUi
{

class MacStylePlugin : public QStylePlugin
{
public: // overrides
    QStringList keys() const;
    QStyle* create( const QString& key );
};

QStringList MacStylePlugin::keys() const
{
    return QStringList() << "XmlUi::MacStyle";
}

QStyle* MacStylePlugin::create( const QString& key )
{
    if ( key.toLower() == QLatin1String( "xmlui::macstyle" ) )
        return new MacStyle();
    return NULL;
}

#if !defined( XMLUI_EXPORT_STYLE_PLUGIN )

QObject* qt_plugin_instance_xmlui_macstyle()
{
    static QPointer<QObject> instance;
    if ( !instance )
        instance = new MacStylePlugin();
    return instance;
}

Q_IMPORT_PLUGIN( xmlui_macstyle )

#else

Q_EXPORT_PLUGIN2( xmlui_macstyle, MacStylePlugin )

#endif // !defined( XMLUI_EXPORT_STYLE_PLUGIN )

}

#endif // !defined( XMLUI_NO_STYLE_PLUGIN )

#endif // !defined( XMLUI_NO_STYLE_MAC )

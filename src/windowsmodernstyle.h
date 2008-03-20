/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This code is partially based on the QtDotNet Style, a Qt Solutions
* component, licensed under the GNU General Public License.
* Copyright (C) 2003-2007 Trolltech ASA.
**************************************************************************/

#ifndef WINDOWSMODERNSTYLE_H
#define WINDOWSMODERNSTYLE_H

#include <QtGlobal>

#if !defined( Q_WS_WIN ) || ( QT_VERSION < 0x040300 ) || defined( QT_NO_STYLE_WINDOWSVISTA )
#define NO_STYLE_WINDOWSMODERN
#endif

#if !defined( NO_STYLE_WINDOWSMODERN )

#include <QWindowsVistaStyle>

class WindowsModernStyle : public QWindowsVistaStyle
{
    Q_OBJECT
public:
    WindowsModernStyle();
    ~WindowsModernStyle();

public: // overrides
    void polish( QApplication* application );
    void unpolish( QApplication* application );

    void polish( QPalette& palette );

    void polish( QWidget* widget );
    void unpolish( QWidget* widget );

    int pixelMetric( PixelMetric metric, const QStyleOption* option, const QWidget* widget ) const;

    QSize sizeFromContents( ContentsType type, const QStyleOption* option,
        const QSize& contentsSize, const QWidget* widget ) const;

    QRect subElementRect( SubElement element, const QStyleOption* option, const QWidget* widget ) const;
    QRect subControlRect( ComplexControl control, const QStyleOptionComplex* option,
        SubControl subControl, const QWidget* widget ) const;

    void drawPrimitive( PrimitiveElement element, const QStyleOption* option,
        QPainter* painter, const QWidget* widget ) const;
    void drawControl( ControlElement element, const QStyleOption* option,
        QPainter* painter, const QWidget* widget ) const;
    void drawComplexControl( ComplexControl control, const QStyleOptionComplex* option,
        QPainter* painter, const QWidget* widget ) const;

protected slots:
    int layoutSpacingImplementation( QSizePolicy::ControlType control1, QSizePolicy::ControlType control2,
        Qt::Orientation orientation, const QStyleOption* option, const QWidget* widget ) const;

private:
    enum Mode
    {
        Blue,
        Silver,
        Olive,
        Classic
    };

private:
    Mode m_mode;

    // main window, toolbox
    QColor m_colorBackgroundBegin;
    QColor m_colorBackgroundEnd;

    // menu
    QColor m_colorMenuBorder;
    QColor m_colorMenuBackground;
    QColor m_colorMenuTitleBegin;
    QColor m_colorMenuTitleEnd;

    // toolbar, tab, toolbox
    QColor m_colorBarBegin;
    QColor m_colorBarMiddle;
    QColor m_colorBarEnd;

    // toolbar handle
    QColor m_colorHandle;
    QColor m_colorHandleLight;

    // menu, toolbar
    QColor m_colorSeparator;
    QColor m_colorSeparatorLight;

    // menu, toolbar, tab, toolbox
    QColor m_colorItemBorder;
    QColor m_colorItemBackgroundBegin;
    QColor m_colorItemBackgroundMiddle;
    QColor m_colorItemBackgroundEnd;
    QColor m_colorItemCheckedBegin;
    QColor m_colorItemCheckedMiddle;
    QColor m_colorItemCheckedEnd;
    QColor m_colorItemSunkenBegin;
    QColor m_colorItemSunkenMiddle;
    QColor m_colorItemSunkenEnd;

    // toolbar shadow, tab, toolbox tab
    QColor m_colorBorder;
    QColor m_colorBorderLight;
};

#endif // !defined( NO_STYLE_WINDOWSMODERN )

#endif

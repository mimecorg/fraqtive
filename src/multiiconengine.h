/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef MULTIICONENGINE_H
#define MULTIICONENGINE_H

#include <QIconEngine>

class MultiIconEngine : public QIconEngine
{
public:
    MultiIconEngine();
    MultiIconEngine( const MultiIconEngine& other );

    explicit MultiIconEngine( const QString& fileName );

    ~MultiIconEngine();

public: // overrides
    void addPixmap( const QPixmap& pixmap, QIcon::Mode mode, QIcon::State state );
    void addFile( const QString& fileName, const QSize& size, QIcon::Mode mode, QIcon::State state );

    QSize actualSize( const QSize& size, QIcon::Mode mode, QIcon::State state );
    QPixmap pixmap( const QSize& size, QIcon::Mode mode, QIcon::State state );

    void paint( QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state );

private:
    QIcon m_icon;
};

#endif

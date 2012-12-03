/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef ICONLOADER_H
#define ICONLOADER_H

#include <QPixmap>
#include <QIcon>

class IconLoader
{
public:
    static QPixmap pixmap( const QString& name, int size = 16 );

    static QPixmap overlayedPixmap( const QString& name, const QString& overlay, int size = 16 );
    static QPixmap overlayedPixmap( const QString& name, const QString& overlay1, const QString& overlay2, int size = 16 );

    static QIcon icon( const QString& name );

    static QIcon overlayedIcon( const QString& name, const QString& overlay );
};

#endif

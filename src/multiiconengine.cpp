/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "multiiconengine.h"

#include <QFile>
#include <QIconEnginePlugin>

MultiIconEngine::MultiIconEngine()
{
}

MultiIconEngine::MultiIconEngine( const MultiIconEngine& other ) :
    m_icon( other.m_icon )
{
}

MultiIconEngine::MultiIconEngine( const QString& fileName )
{
    if ( fileName.isNull() )
        return;

    int index = fileName.lastIndexOf( '-' );
    if ( index < 0 ) {
        m_icon.addPixmap( QPixmap( fileName ) );
        return;
    }
    QString baseName = fileName.left( index + 1 );

    static const int sizes[] = { 16, 22, 32, 48, -1 };

    for ( int i = 0; sizes[ i ] > 0; i++ ) {
        int size = sizes[ i ];
        QString iconName = baseName + QString::number( size ) + ".png";
        if ( QFile::exists( iconName ) )
            m_icon.addPixmap( QPixmap( iconName ) );
    }
}

MultiIconEngine::~MultiIconEngine()
{
}

void MultiIconEngine::addPixmap( const QPixmap& pixmap, QIcon::Mode mode, QIcon::State state )
{
    m_icon.addPixmap( pixmap, mode, state );
}

void MultiIconEngine::addFile( const QString& fileName, const QSize& size, QIcon::Mode mode, QIcon::State state )
{
    m_icon.addFile( fileName, size, mode, state );
}

QSize MultiIconEngine::actualSize( const QSize& size, QIcon::Mode mode, QIcon::State state )
{
    return m_icon.actualSize( size, mode, state );
}

QPixmap MultiIconEngine::pixmap( const QSize& size, QIcon::Mode mode, QIcon::State state )
{
    return m_icon.pixmap( size, mode, state );
}

void MultiIconEngine::paint( QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state )
{
    m_icon.paint( painter, rect, Qt::AlignCenter, mode, state );
}

class MultiIconEnginePlugin : public QIconEnginePlugin
{
public: // overrides
    QStringList keys() const;
    QIconEngine* create( const QString& fileName );
};

QStringList MultiIconEnginePlugin::keys() const
{
    return QStringList() << "png";
}

QIconEngine* MultiIconEnginePlugin::create( const QString& fileName )
{
    return new MultiIconEngine( fileName );
}

QObject* qt_plugin_instance_multiiconengine()
{
    static QPointer<QObject> instance;
    if ( !instance )
        instance = new MultiIconEnginePlugin();
    return instance;
}

Q_IMPORT_PLUGIN( multiiconengine )

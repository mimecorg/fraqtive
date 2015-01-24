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

#include "configurationdata.h"

#if defined( Q_OS_WIN )
#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
#endif
#include <shlobj.h>
#endif

#include <QDir>
#include <cstdlib>

ConfigurationData::ConfigurationData()
{
    QString homePath = QDir::homePath();

#if defined( Q_OS_WIN )
    TCHAR appDataPath[ MAX_PATH ];
    if ( SHGetSpecialFolderPath( 0, appDataPath, CSIDL_APPDATA, FALSE ) )
        m_dataPath = QDir::fromNativeSeparators( QString::fromUtf16( (ushort*)appDataPath ) );
    if ( m_dataPath.isEmpty() )
        m_dataPath = homePath;
    m_dataPath += "/Fraqtive";
#else
    char* dataHome = getenv( "XDG_DATA_HOME" );
    if ( dataHome ) {
        if ( dataHome[ 0 ] == '/' )
            m_dataPath = dataHome;
        else
            m_dataPath = homePath + '/' + dataHome;
    } else {
        m_dataPath = homePath + "/.local/share";
    }
    m_dataPath += "/fraqtive";
#endif
}

ConfigurationData::~ConfigurationData()
{
}

bool ConfigurationData::contains( const QString& key ) const
{
    return m_data.contains( key );
}

void ConfigurationData::setValue( const QString& key, const QVariant& value )
{
    m_data[ key ] = value;
}

QVariant ConfigurationData::value( const QString& key, const QVariant& defaultValue /*= QVariant()*/ ) const
{
    QVariantMap::const_iterator it = m_data.find( key );
    if ( it == m_data.end() )
        return defaultValue;
    return it.value();
}

void ConfigurationData::readConfiguration()
{
    QFile file;
    QDataStream stream;

    if ( readFile( &file, &stream, "config.dat" ) )
        stream >> m_data;

    if ( readFile( &file, &stream, "bookmarks.dat" ) )
        stream >> m_bookmarks;

    if ( readFile( &file, &stream, "presets.dat" ) )
        stream >> m_userPresets;

    if ( readFile( &file, &stream, ":/data/presets.dat" ) )
        stream >> m_defaultPresets;
}

bool ConfigurationData::readFile( QFile* file, QDataStream* stream, const QString& fileName )
{
    QString path;
    if ( QDir::isRelativePath( fileName ) )
        path = locateDataFile( fileName );
    else
        path = fileName;

    file->close();
    file->setFileName( path );

    if ( !file->open( QIODevice::ReadOnly ) )
        return false;

    stream->setDevice( file );
    stream->setVersion( QDataStream::Qt_4_2 );

    qint32 version;
    *stream >> version;

    if ( version < 1 || version > 2 )
        return false;

    m_dataVersion = version;

    return true;
}

void ConfigurationData::writeConfiguration()
{
    QFile file;
    QDataStream stream;

    if ( writeFile( &file, &stream, "config.dat" ) )
        stream << m_data;

    if ( writeFile( &file, &stream, "bookmarks.dat" ) )
        stream << m_bookmarks;

    if ( writeFile( &file, &stream, "presets.dat" ) )
        stream << m_userPresets;
}

bool ConfigurationData::writeFile( QFile* file, QDataStream* stream, const QString& fileName )
{
    QString path;
    if ( QDir::isRelativePath( fileName ) )
        path = locateDataFile( fileName );
    else
        path = fileName;

    file->close();
    file->setFileName( path );

    if ( !file->open( QIODevice::WriteOnly ) )
        return false;

    stream->setDevice( file );
    stream->setVersion( QDataStream::Qt_4_2 );

    // increment version when adding / modifying fields
    m_dataVersion = 2;

    *stream << (qint32)m_dataVersion;

    return true;
}

QString ConfigurationData::locateDataFile( const QString& name )
{
    QString path = m_dataPath + '/' + name;

    checkAccess( path );

    return path;
}

bool ConfigurationData::checkAccess( const QString& path )
{
    QFileInfo fileInfo( path );

    if ( fileInfo.exists() )
        return fileInfo.isReadable();

    QDir dir = QDir::root();

    QStringList pathParts = path.split( '/', QString::SkipEmptyParts );

    for ( int i = 0; i < pathParts.size() - 1; i++ ) {
        const QString& part = pathParts.at( i );

        if ( dir.cd( part ) )
            continue;

        if ( dir.mkdir( part ) && dir.cd( part ) )
            continue;

        return false;
    }

    return true;
}

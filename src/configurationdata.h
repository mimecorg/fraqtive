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

#ifndef CONFIGURATIONDATA_H
#define CONFIGURATIONDATA_H

#include <QVariant>

class QFile;

#include "datastructures.h"

class ConfigurationData
{
public:
    ConfigurationData();
    ~ConfigurationData();

public:
    bool contains( const QString& key ) const;

    void setValue( const QString& key, const QVariant& value );
    QVariant value( const QString& key, const QVariant& defaultValue = QVariant() ) const;

    BookmarkMap* bookmarks() { return &m_bookmarks; }
    const BookmarkMap* bookmarks() const { return &m_bookmarks; }

    PresetMap* defaultPresets() { return &m_defaultPresets; }
    const PresetMap* defaultPresets() const { return &m_defaultPresets; }

    PresetMap* userPresets() { return &m_userPresets; }
    const PresetMap* userPresets() const { return &m_userPresets; }

    void readConfiguration();
    void writeConfiguration();

    int dataVersion() const { return m_dataVersion; }

private:
    bool readFile( QFile* file, QDataStream* stream, const QString& path );
    bool writeFile( QFile* file, QDataStream* stream, const QString& path );

    QString locateDataFile( const QString& name );

    bool checkAccess( const QString& path );

private:
    QString m_dataPath;

    QVariantMap m_data;

    BookmarkMap m_bookmarks;
    PresetMap m_defaultPresets;
    PresetMap m_userPresets;

    int m_dataVersion;
};

#endif

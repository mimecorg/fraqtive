/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef CONFIGURATIONDATA_H
#define CONFIGURATIONDATA_H

#include <QVariant>

class ConfigurationData
{
public:
    ConfigurationData();
    ~ConfigurationData();

public:
    bool contains( const QString& key ) const;

    void setValue( const QString& key, const QVariant& value );
    QVariant value( const QString& key, const QVariant& defaultValue = QVariant() ) const;

    void readConfiguration();
    void writeConfiguration();

private:
    QString locateDataFile( const QString& name );

    bool checkAccess( const QString& path );

private:
    QString m_dataPath;

    QVariantMap m_data;
};

#endif

/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef PROPERTYTOOLBOX_H
#define PROPERTYTOOLBOX_H

#include <QToolBox>

class FractalModel;
class ParametersPage;
class ColorSettingsPage;
class AdvancedSettingsPage;

class PropertyToolBox : public QToolBox
{
public:
    PropertyToolBox( QWidget* parent );
    ~PropertyToolBox();

public:
    void setModel( FractalModel* model );

public: // overrides
    QSize sizeHint() const;

private:
    void addPage( QWidget* page );

private:
    ParametersPage* m_parameters;
    ColorSettingsPage* m_colorSettings;
    AdvancedSettingsPage* m_advancedSettings;
};

#endif

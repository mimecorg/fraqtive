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

#ifndef PROPERTYTOOLBOX_H
#define PROPERTYTOOLBOX_H

#include <QToolBox>

class FractalModel;
class ParametersPage;
class ColorSettingsPage;
class AdvancedSettingsPage;
class AnimationPage;

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
    AnimationPage* m_animationPage;
};

#endif

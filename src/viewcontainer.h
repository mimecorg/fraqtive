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

#ifndef VIEWCONTAINER_H
#define VIEWCONTAINER_H

#include <QWidget>

class QBoxLayout;

class ViewContainer : public QWidget
{
    Q_OBJECT
public:
    ViewContainer( QWidget* parent );
    ~ViewContainer();

public:
    void setView( QWidget* view );
    QWidget* view() const { return m_view; }

public: // overrides
    QSize sizeHint() const;

private:
    QBoxLayout* m_layout;

    QWidget* m_view;
};

#endif

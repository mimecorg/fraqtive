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

#include "viewcontainer.h"

#include <QVBoxLayout>

ViewContainer::ViewContainer( QWidget* parent ) : QWidget( parent ),
    m_view( NULL )
{
    m_layout = new QVBoxLayout( this );
    m_layout->setMargin( 0 );
}

ViewContainer::~ViewContainer()
{
}

void ViewContainer::setView( QWidget* view )
{
    if ( m_view != view ) {
        delete m_view;
        m_view = view;
        m_layout->addWidget( view );
    }
}

QSize ViewContainer::sizeHint() const
{
    return QSize( 200, 200 );
}

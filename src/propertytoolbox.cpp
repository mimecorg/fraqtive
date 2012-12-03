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

#include "propertytoolbox.h"

#include "parameterspage.h"
#include "colorsettingspage.h"
#include "advancedsettingspage.h"
#include "animationpage.h"

PropertyToolBox::PropertyToolBox( QWidget* parent ) : QToolBox( parent )
{
    m_parameters = new ParametersPage( this );
    m_colorSettings = new ColorSettingsPage( this );
    m_advancedSettings = new AdvancedSettingsPage( this );
    m_animationPage = new AnimationPage( this );

    addPage( m_parameters );
    addPage( m_colorSettings );
    addPage( m_advancedSettings );
    addPage( m_animationPage );

    setItemIcon( 0, QIcon( ":/icons/edit-16.png" ) );
    setItemIcon( 1, QIcon( ":/icons/gradient-16.png" ) );
    setItemIcon( 2, QIcon( ":/icons/configure-16.png" ) );
    setItemIcon( 3, QIcon( ":/icons/arrows-16.png" ) );
}

PropertyToolBox::~PropertyToolBox()
{
}

void PropertyToolBox::setModel( FractalModel* model )
{
    m_parameters->setModel( model );
    m_colorSettings->setModel( model );
    m_advancedSettings->setModel( model );
    m_animationPage->setModel( model );
}

QSize PropertyToolBox::sizeHint() const
{
    return QSize( 300, 400 );
}

void PropertyToolBox::addPage( QWidget* page )
{
    QBoxLayout* pageLayout = qobject_cast<QBoxLayout*>( page->layout() );
    if ( pageLayout && pageLayout->direction() == QBoxLayout::TopToBottom ) {
        pageLayout->setContentsMargins( 5, 5, 5, 5 );
        pageLayout->setSpacing( 3 );
        for ( int i = 0; i < pageLayout->count(); i++ ) {
            QLayoutItem* item = pageLayout->itemAt( i );
            if ( item->spacerItem() )
                continue;
            QLabel* label = qobject_cast<QLabel*>( item->widget() );
            if ( label ) {
                QString style = "border: none; border-bottom: 1px solid palette(dark);";
                if ( i > 0 )
                    style += "margin-top: 2px;";
                label->setStyleSheet( style );
                continue;
            }
            QBoxLayout* itemLayout = qobject_cast<QBoxLayout*>( item->layout() );
            if ( itemLayout && itemLayout->direction() == QBoxLayout::LeftToRight ) {
                itemLayout->insertSpacing( 0, 10 );
            } else {
                pageLayout->removeItem( item );
                QHBoxLayout* wrapperLayout = new QHBoxLayout();
                wrapperLayout->addSpacing( 10 );
                wrapperLayout->addItem( item );
                pageLayout->insertLayout( i, wrapperLayout );
            }
        }
    }

    page->setBackgroundRole( QPalette::Base );

    addItem( page, page->windowTitle() );
}

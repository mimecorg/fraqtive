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

#include "animationpage.h"

#include "fractalmodel.h"

AnimationPage::AnimationPage( QWidget* parent ) : QWidget( parent ),
    m_model( NULL ),
    m_loading( false )
{
    m_ui.setupUi( this );

    m_ui.sliderScrolling->setScaledRange( -0.5, 0.5 );
    m_ui.sliderRotation->setScaledRange( -0.25, 0.25 );
}

AnimationPage::~AnimationPage()
{
}

void AnimationPage::setModel( FractalModel* model )
{
    m_model = model;

    connect( m_model, SIGNAL( animationSettingsChanged() ), this, SLOT( animationSettingsChanged() ) );
    connect( m_model, SIGNAL( viewModeChanged() ), this, SLOT( viewModeChanged() ) );
}

void AnimationPage::on_sliderScrolling_valueChanged()
{
    saveSettings();
    updateButtons();
}

void AnimationPage::on_sliderRotation_valueChanged()
{
    saveSettings();
    updateButtons();
}

void AnimationPage::on_stopScrolling_clicked()
{
    AnimationSettings settings = m_model->animationSettings();

    settings.setScrollingEnabled( false );
    settings.setScrollingSpeed( 0.0 );

    m_model->setAnimationSettings( settings );
}

void AnimationPage::on_stopRotation_clicked()
{
    AnimationSettings settings = m_model->animationSettings();

    settings.setRotationEnabled( false );
    settings.setRotationSpeed( 0.0 );

    m_model->setAnimationSettings( settings );
}

void AnimationPage::animationSettingsChanged()
{
    loadSettings();
}

void AnimationPage::viewModeChanged()
{
    m_ui.sliderRotation->setEnabled( m_model->viewMode() == MeshViewMode );

    updateButtons();
}

void AnimationPage::updateButtons()
{
    AnimationSettings settings = m_model->animationSettings();

    m_ui.stopScrolling->setEnabled( settings.isScrollingEnabled() );

    if ( m_model->viewMode() == MeshViewMode )
        m_ui.stopRotation->setEnabled( settings.isRotationEnabled() );
    else
        m_ui.stopRotation->setEnabled( false );
}

void AnimationPage::loadSettings()
{
    m_loading = true;

    AnimationSettings settings = m_model->animationSettings();

    m_ui.sliderScrolling->setScaledValue( settings.scrollingSpeed() );
    m_ui.sliderRotation->setScaledValue( settings.rotationSpeed() );

    m_loading = false;
}

void AnimationPage::saveSettings()
{
    if ( m_loading )
        return;

    AnimationSettings settings = m_model->animationSettings();

    settings.setScrollingSpeed( m_ui.sliderScrolling->scaledValue() );
    settings.setRotationSpeed( m_ui.sliderRotation->scaledValue() );

    if ( settings.scrollingSpeed() != 0.0 )
        settings.setScrollingEnabled( true );
    if ( settings.rotationSpeed() != 0.0 )
        settings.setRotationEnabled( true );

    m_model->setAnimationSettings( settings );
}

/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "fraqtivemainwindow.h"

#include <QCloseEvent>
#include <QMessageBox>

#include "tutorialdialog.h"
#include "datastructures.h"
#include "fractalmodel.h"
#include "fractalpresenter.h"
#include "fractaltypedialog.h"
#include "gradientdialog.h"
#include "imageview.h"
#include "fraqtiveapplication.h"
#include "configurationdata.h"

FraqtiveMainWindow::FraqtiveMainWindow() :
    m_tutorialDialog( NULL )
{
    m_ui.setupUi( this );

    m_ui.menuView->addAction( m_ui.dockProperties->toggleViewAction() );
    m_ui.menuView->addAction( m_ui.dockPreview->toggleViewAction() );
    m_ui.menuView->addSeparator();
    m_ui.menuView->addAction( m_ui.toolBar->toggleViewAction() );

    m_model = new FractalModel( this );

    connect( m_model, SIGNAL( positionChanged() ), this, SLOT( positionChanged() ) );
    connect( m_model, SIGNAL( navigationChanged() ), this, SLOT( navigationChanged() ) );

    ImageView* view = new ImageView( m_ui.mainContainer, m_model->presenter() );
    view->setInteractive( true );

    m_model->presenter()->setView( view );

    m_ui.mainContainer->setView( view );

    m_ui.propertyToolBox->setModel( m_model );

    FractalPresenter* previewPresenter = new FractalPresenter( this );
    ImageView* preview = new ImageView( m_ui.previewContainer, previewPresenter );

    previewPresenter->setView( preview );
    previewPresenter->setPreviewMode( true );
    previewPresenter->setPriority( -1 );

    m_ui.previewContainer->setView( preview );

    m_model->setPreviewPresenter( previewPresenter );

    FractalType type;
    type.setIntegralExponent( 2 );

    m_model->setFractalType( type );
    m_model->setDefaultPosition();

    m_model->loadDefaultColorSettings();
    m_model->loadDefaultGeneratorSettings();
    m_model->loadDefaultViewSettings();

    m_model->setNavigationEnabled( true );
    m_model->setEnabled( true );

    view->setFocus();

    ConfigurationData* config = fraqtive()->configuration();

    if ( config->contains( "Geometry" ) )
        restoreGeometry( config->value( "Geometry" ).toByteArray() );

    if ( config->contains( "State" ) )
        restoreState( config->value( "State" ).toByteArray(), 1 );

    if ( config->value( "ShowTutorial", true ).toBool() )
        on_actionQuickTutorial_activated();
}

FraqtiveMainWindow::~FraqtiveMainWindow()
{
}

void FraqtiveMainWindow::closeEvent( QCloseEvent* e )
{
    e->accept();

    ConfigurationData* config = fraqtive()->configuration();
    config->setValue( "Geometry", saveGeometry() );
    config->setValue( "State", saveState( 1 ) );

    if ( m_tutorialDialog )
        m_tutorialDialog->close();
}

void FraqtiveMainWindow::on_actionQuit_activated()
{
    if ( m_ui.mainContainer->windowState() & Qt::WindowFullScreen )
        on_actionFullScreen_activated();

    close();
}

void FraqtiveMainWindow::on_actionFractalType_activated()
{
    FractalTypeDialog dialog( this, m_model );

    if ( dialog.exec() == QDialog::Accepted )
        m_model->setParameters( dialog.fractalType(), dialog.position() );
}

void FraqtiveMainWindow::on_actionEditGradient_activated()
{
    GradientDialog dialog( this );
    dialog.setGradient( m_model->gradient() );

    connect( &dialog, SIGNAL( applyGradient( const Gradient& ) ), this, SLOT( applyGradient( const Gradient& ) ) );

    dialog.exec();
}

void FraqtiveMainWindow::applyGradient( const Gradient& gradient )
{
    m_model->setGradient( gradient );
}

void FraqtiveMainWindow::on_actionFullScreen_activated()
{
    if ( m_ui.mainContainer->windowState() & Qt::WindowFullScreen ) {
        m_ui.mainContainer->setParent( m_ui.centralWidget );
        m_ui.vboxLayout->addWidget( m_ui.mainContainer );
        QList<QAction*> actions = menuBar()->actions();
        for ( int i = 0; i < actions.count(); i++ )
            m_ui.mainContainer->removeAction( actions.at( i ) );
        show();
    } else {
        m_ui.mainContainer->setParent( NULL );
        m_ui.mainContainer->showFullScreen();
        m_ui.mainContainer->addActions( menuBar()->actions() );
        m_ui.mainContainer->setWindowTitle( windowTitle() );
        hide();
    }
}

void FraqtiveMainWindow::on_actionDefaultPosition_activated()
{
    m_model->setDefaultPosition();
}

void FraqtiveMainWindow::positionChanged()
{
    m_ui.actionDefaultPosition->setEnabled( !m_model->hasDefaultPosition() );
}

void FraqtiveMainWindow::on_actionNavigateBack_activated()
{
    m_model->navigateBackward();
}

void FraqtiveMainWindow::on_actionNavigateForward_activated()
{
    m_model->navigateForward();
}

void FraqtiveMainWindow::navigationChanged()
{
    m_ui.actionNavigateBack->setEnabled( m_model->canNavigateBackward() );
    m_ui.actionNavigateForward->setEnabled( m_model->canNavigateForward() );
}

void FraqtiveMainWindow::on_actionQuickTutorial_activated()
{
    if ( !m_tutorialDialog )
        m_tutorialDialog = new TutorialDialog( this );

    m_tutorialDialog->show();
    m_tutorialDialog->activateWindow();
}

void FraqtiveMainWindow::on_actionAboutFraqtive_activated()
{
    QString version = "0.4.0";
    QString link = "<a href=\"http://fraqtive.mimec.org\">fraqtive.mimec.org</a>";

    QString message;
    message += "<h3>" + tr( "Fraqtive %1" ).arg( version ) + "</h3>";
    message += "<p>" + tr( "A Mandelbrot family fractal generator." ) + "</p>";
    message += "<p>" + trUtf8( "Copyright (C) 2004-2008 Michał Męciński" ) + "</p>";
    message += "<p>" + tr( "This program is licensed under the GNU General Public License." ) + "</p>";
    message += "<p>" + tr( "Visit %1 for more information." ).arg( link ) + "</p>";

    QMessageBox::about( this, tr( "About Fraqtive" ), message );
}

void FraqtiveMainWindow::on_actionAboutQt_activated()
{
    QMessageBox::aboutQt( this );
}

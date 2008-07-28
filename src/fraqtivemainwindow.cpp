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
#include <QImageWriter>
#include <QFileDialog>
#include <QClipboard>

#include "tutorialdialog.h"
#include "datastructures.h"
#include "fractalmodel.h"
#include "fractalpresenter.h"
#include "fractaltypedialog.h"
#include "gradientdialog.h"
#include "imageview.h"
#include "meshview.h"
#include "fraqtiveapplication.h"
#include "configurationdata.h"
#include "loadbookmarkdialog.h"
#include "savebookmarkdialog.h"
#include "loadpresetdialog.h"
#include "savepresetdialog.h"

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

    m_ui.mainContainer->installEventFilter( this );

    m_ui.action3DView->setEnabled( QGLFormat::hasOpenGL() );

    m_model->setViewMode( ImageViewMode );

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

    if ( isFullScreenMode() )
        leaveFullScreenMode();

    ConfigurationData* config = fraqtive()->configuration();
    config->setValue( "Geometry", saveGeometry() );
    config->setValue( "State", saveState( 1 ) );

    if ( m_tutorialDialog )
        m_tutorialDialog->close();
}

bool FraqtiveMainWindow::eventFilter( QObject* watched, QEvent* e )
{
    if ( watched == m_ui.mainContainer ) {
        if ( e->type() == QEvent::Close ) {
            close();
            return true;
        }

        if ( e->type() == QEvent::KeyPress && static_cast<QKeyEvent*>( e )->key() == Qt::Key_Escape ) {
            if ( isFullScreenMode() )
                leaveFullScreenMode();
            return true;
        }
    }

    return QMainWindow::eventFilter( watched, e );
}

void FraqtiveMainWindow::on_actionQuit_activated()
{
    if ( isFullScreenMode() )
        leaveFullScreenMode();

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
    if ( isFullScreenMode() )
        leaveFullScreenMode();
    else
        enterFullScreenMode();
}

bool FraqtiveMainWindow::isFullScreenMode() const
{
    return m_ui.mainContainer->windowState() & Qt::WindowFullScreen;
}

void FraqtiveMainWindow::enterFullScreenMode()
{
    m_ui.mainContainer->setParent( NULL );
    m_ui.mainContainer->showFullScreen();
    m_ui.mainContainer->addActions( menuBar()->actions() );
    m_ui.mainContainer->setWindowTitle( windowTitle() );
    hide();
}

void FraqtiveMainWindow::leaveFullScreenMode()
{
    m_ui.mainContainer->setParent( m_ui.centralWidget );
    m_ui.vboxLayout->addWidget( m_ui.mainContainer );
    QList<QAction*> actions = menuBar()->actions();
    for ( int i = 0; i < actions.count(); i++ )
        m_ui.mainContainer->removeAction( actions.at( i ) );
    show();
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

void FraqtiveMainWindow::on_actionLoadPreset_activated()
{
    LoadPresetDialog dialog( this );
    dialog.setModel( m_model );
    dialog.exec();
}

void FraqtiveMainWindow::on_actionSavePreset_activated()
{
    SavePresetDialog dialog( this );
    dialog.setModel( m_model );
    dialog.exec();
}

void FraqtiveMainWindow::on_actionLoadBookmark_activated()
{
    LoadBookmarkDialog dialog( this );
    dialog.setModel( m_model );
    dialog.exec();
}

void FraqtiveMainWindow::on_actionSaveBookmark_activated()
{
    SaveBookmarkDialog dialog( this );
    dialog.setModel( m_model );
    dialog.exec();
}

void FraqtiveMainWindow::on_actionSaveImage_activated()
{
    QList<QByteArray> supportedFormats = QImageWriter::supportedImageFormats();

    QList<QByteArray> formats;
    QStringList filters;

    const char* data[] = {
        "png", QT_TR_NOOP( "PNG Image" ), "*.png",
        "jpeg", QT_TR_NOOP( "JPEG Image" ), "*.jpeg;*.jpg",
        "tiff", QT_TR_NOOP( "TIFF Image" ), "*.tiff;*.tif",
        "bmp", QT_TR_NOOP( "Windows Bitmap" ), "*.bmp",
        "ppm", QT_TR_NOOP( "Portable Pixmap" ), "*.ppm",
        NULL
    };

    for ( int i = 0; data[ i ] != NULL; i += 3 ) {
        if ( supportedFormats.contains( data[ i ] ) ) {
            formats.append( data[ i ] );
            filters.append( QString( "%1 (%2)" ).arg( tr( data[ i + 1 ] ), QString::fromLatin1( data[ i + 2 ] ) ) );
        }
    }

    ConfigurationData* config = fraqtive()->configuration();

    QByteArray format = config->value( "SaveFormat" ).toByteArray();
    if ( format.isEmpty() || !formats.contains( format ) )
        format = formats.first();

    QString selectedFilter = filters.at( formats.indexOf( format ) );

    QString path = config->value( "SavePath", QDir::homePath() ).toString();

    QString fileName = QFileInfo( QDir( path ), tr( "fractal" ) ).absoluteFilePath();

    fileName = QFileDialog::getSaveFileName( this, tr( "Save Image" ), fileName,
        filters.join( ";;" ), &selectedFilter );

    if ( !fileName.isEmpty() ) {
        int index = filters.indexOf( selectedFilter );
        if ( index >= 0 )
            format = formats.at( index );

        if ( QFileInfo( fileName ).suffix().isEmpty() )
            fileName += '.' + format;

        config->setValue( "SaveFormat", format );
        config->setValue( "SavePath", QFileInfo( fileName ).absolutePath() );

        QImageWriter writer( fileName, format );

        if ( format == "tiff" )
            writer.setCompression( 1 );

        QImage image = currentImage();

        if ( !writer.write( image ) ) {
            QMessageBox::warning( this, tr( "Error" ), tr( "The selected file could not be saved." ) );
        }
    }
}

void FraqtiveMainWindow::on_actionCopyImage_activated()
{
    QImage image = currentImage();

    QApplication::clipboard()->setImage( image );
}

QImage FraqtiveMainWindow::currentImage()
{
    if ( ImageView* imageView = qobject_cast<ImageView*>( m_ui.mainContainer->view() ) )
        return imageView->image();
    if ( MeshView* meshView = qobject_cast<MeshView*>( m_ui.mainContainer->view() ) )
        return meshView->image();
    return QImage();
}

void FraqtiveMainWindow::on_action2DView_activated()
{
    if ( !qobject_cast<ImageView*>( m_ui.mainContainer->view() ) ) {
        ImageView* view = new ImageView( m_ui.mainContainer, m_model->presenter() );
        view->setInteractive( true );

        m_model->presenter()->setView( view );
        m_ui.mainContainer->setView( view );

        m_model->setViewMode( ImageViewMode );

        view->setFocus();
    }

    m_ui.action2DView->setChecked( true );
    m_ui.action3DView->setChecked( false );
}

void FraqtiveMainWindow::on_action3DView_activated()
{
    if ( !qobject_cast<MeshView*>( m_ui.mainContainer->view() ) ) {
        MeshView* view = new MeshView( m_ui.mainContainer, m_model->presenter() );

        m_model->presenter()->setView( view );
        m_ui.mainContainer->setView( view );

        m_model->clearHovering();

        m_model->setViewMode( MeshViewMode );

        view->setFocus();
    }

    m_ui.action2DView->setChecked( false );
    m_ui.action3DView->setChecked( true );
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
    QString version = "0.4.4";
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

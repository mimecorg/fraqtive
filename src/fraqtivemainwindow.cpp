/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Michał Męciński
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
#include <QClipboard>
#include <QProgressDialog>
#include <QPainter>
#include <QMenu>

#include <math.h>

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
#include "generateimagedialog.h"
#include "generateseriesdialog.h"
#include "imagegenerator.h"
#include "iconloader.h"
#include "xmlui/toolstrip.h"
#include "xmlui/builder.h"

FraqtiveMainWindow::FraqtiveMainWindow()
{
    QAction* action;

    action = new QAction( IconLoader::icon( "edit" ), tr( "Fractal Type..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_actionFractalType_triggered() ) );
    setAction( "fractalType", action );

    action = new QAction( IconLoader::icon( "gradient" ), tr( "Edit Gradient..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_actionEditGradient_triggered() ) );
    setAction( "editGradient", action );

    action = new QAction( IconLoader::icon( "fullscreen" ), tr( "Full Screen" ), this );
    action->setShortcut( QKeySequence( Qt::Key_F11 ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_actionFullScreen_triggered() ) );
    setAction( "fullScreen", action );

    action = new QAction( IconLoader::icon( "back" ), tr( "Navigate Back" ), this );
    action->setShortcut( QKeySequence( Qt::ALT + Qt::Key_Left ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_actionNavigateBack_triggered() ) );
    setAction( "navigateBack", action );

    action = new QAction( IconLoader::icon( "forward" ), tr( "Navigate Forward" ), this );
    action->setShortcut( QKeySequence( Qt::ALT + Qt::Key_Right ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_actionNavigateForward_triggered() ) );
    setAction( "navigateForward", action );

    action = new QAction( IconLoader::icon( "home" ), tr( "Default Position" ), this );
    action->setShortcut( QKeySequence( Qt::ALT + Qt::Key_Home ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_actionDefaultPosition_triggered() ) );
    setAction( "defaultPosition", action );

    action = new QAction( IconLoader::icon( "load-preset" ), tr( "Load Preset..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_actionLoadPreset_triggered() ) );
    setAction( "loadPreset", action );

    action = new QAction( IconLoader::icon( "save-preset" ), tr( "Save Preset..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_actionSavePreset_triggered() ) );
    setAction( "savePreset", action );

    action = new QAction( IconLoader::icon( "load-bookmark" ), tr( "Load Bookmark..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_actionLoadBookmark_triggered() ) );
    setAction( "loadBookmark", action );

    action = new QAction( IconLoader::icon( "save-bookmark" ), tr( "Save Bookmark..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_actionSaveBookmark_triggered() ) );
    setAction( "saveBookmark", action );

    action = new QAction( IconLoader::icon( "save" ), tr( "Save Image..." ), this );
    action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_S ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_actionSaveImage_triggered() ) );
    setAction( "saveImage", action );

    action = new QAction( IconLoader::icon( "copy" ), tr( "Copy Image" ), this );
    action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_C ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_actionCopyImage_triggered() ) );
    setAction( "copyImage", action );

    action = new QAction( IconLoader::icon( "generate-image" ), tr( "Generate Image..." ), this );
    action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_G ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_actionGenerateImage_triggered() ) );
    setAction( "generateImage", action );

    action = new QAction( IconLoader::icon( "generate-series" ), tr( "Generate Series..." ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_actionGenerateSeries_triggered() ) );
    setAction( "generateSeries", action );

    action = new QAction( IconLoader::icon( "view2d" ), tr( "2D View" ), this );
    action->setShortcut( QKeySequence( Qt::Key_F2 ) );
    action->setCheckable( true );
    action->setChecked( true );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_action2DView_triggered() ) );
    setAction( "view2d", action );

    action = new QAction( IconLoader::icon( "view3d" ), tr( "3D View" ), this );
    action->setShortcut( QKeySequence( Qt::Key_F3 ) );
    action->setCheckable( true );
    connect( action, SIGNAL( triggered() ), this, SLOT( on_action3DView_triggered() ) );
    setAction( "view3d", action );

    action = new QAction( IconLoader::icon( "about" ), tr( "About Fraqtive" ), this );
    action->setShortcut( QKeySequence( Qt::Key_F1 ) );
    connect( action, SIGNAL( triggered() ), fraqtive(), SLOT( about() ) );
    setAction( "aboutFraqtive", action );

    setTitle( "sectionFile", tr( "File" ) );
    setTitle( "sectionEdit", tr( "Edit" ) );
    setTitle( "sectionView", tr( "View" ) );
    setTitle( "sectionPosition", tr( "Position" ) );

    loadXmlUiFile( ":/resources/fraqtivemainwindow.xml" );

    XmlUi::ToolStrip* strip = new XmlUi::ToolStrip( this );
    strip->addAuxiliaryAction( this->action( "aboutFraqtive" ) );
    setMenuWidget( strip );

    XmlUi::Builder* builder = new XmlUi::Builder( this );
    builder->registerToolStrip( "stripMain", strip );
    builder->addClient( this );

    strip->setContextMenuPolicy( Qt::CustomContextMenu );
    connect( strip, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( customContextMenuRequested( const QPoint& ) ) );

    m_ui.setupUi( this );

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

    this->action( "view3d" )->setEnabled( QGLFormat::hasOpenGL() );

    m_model->setViewMode( ImageViewMode );

    view->setFocus();

    ConfigurationData* config = fraqtive()->configuration();

    if ( config->contains( "Geometry" ) )
        restoreGeometry( config->value( "Geometry" ).toByteArray() );
    else
        setWindowState( Qt::WindowMaximized );

    if ( config->contains( "State" ) )
        restoreState( config->value( "State" ).toByteArray(), 2 );
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
    config->setValue( "State", saveState( 2 ) );
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

void FraqtiveMainWindow::customContextMenuRequested( const QPoint& pos )
{
    QMenu* menu = createPopupMenu();
    if ( menu )
        menu->popup( menuWidget()->mapToGlobal( pos ) );
}

void FraqtiveMainWindow::on_actionFractalType_triggered()
{
    FractalTypeDialog dialog( this, m_model );

    if ( dialog.exec() == QDialog::Accepted )
        m_model->setParameters( dialog.fractalType(), dialog.position() );
}

void FraqtiveMainWindow::on_actionEditGradient_triggered()
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

void FraqtiveMainWindow::on_actionFullScreen_triggered()
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
    m_ui.mainContainer->addActions( XmlUi::Client::actions() );
    m_ui.mainContainer->setWindowTitle( windowTitle() );
    hide();
}

void FraqtiveMainWindow::leaveFullScreenMode()
{
    m_ui.mainContainer->setParent( m_ui.centralWidget );
    m_ui.vboxLayout->addWidget( m_ui.mainContainer );
    QList<QAction*> actions = XmlUi::Client::actions();
    for ( int i = 0; i < actions.count(); i++ )
        m_ui.mainContainer->removeAction( actions.at( i ) );
    show();
}

void FraqtiveMainWindow::on_actionDefaultPosition_triggered()
{
    m_model->setDefaultPosition();
}

void FraqtiveMainWindow::positionChanged()
{
    action( "defaultPosition" )->setEnabled( !m_model->hasDefaultPosition() );
}

void FraqtiveMainWindow::on_actionNavigateBack_triggered()
{
    m_model->navigateBackward();
}

void FraqtiveMainWindow::on_actionNavigateForward_triggered()
{
    m_model->navigateForward();
}

void FraqtiveMainWindow::navigationChanged()
{
    action( "navigateBack" )->setEnabled( m_model->canNavigateBackward() );
    action( "navigateForward" )->setEnabled( m_model->canNavigateForward() );
}

void FraqtiveMainWindow::on_actionLoadPreset_triggered()
{
    LoadPresetDialog dialog( this );
    dialog.setModel( m_model );
    dialog.exec();
}

void FraqtiveMainWindow::on_actionSavePreset_triggered()
{
    SavePresetDialog dialog( this );
    dialog.setModel( m_model );
    dialog.exec();
}

void FraqtiveMainWindow::on_actionLoadBookmark_triggered()
{
    LoadBookmarkDialog dialog( this );
    dialog.setModel( m_model );
    dialog.exec();
}

void FraqtiveMainWindow::on_actionSaveBookmark_triggered()
{
    SaveBookmarkDialog dialog( this );
    dialog.setModel( m_model );
    dialog.exec();
}

void FraqtiveMainWindow::on_actionSaveImage_triggered()
{
    QByteArray format;
    QString fileName = getSaveImageName( &format );

    if ( !fileName.isEmpty() ) {
        QImageWriter* writer = createImageWriter( fileName, format );

        QImage image = currentImage();

        if ( !writer->write( image ) ) {
            QMessageBox::warning( this, tr( "Error" ), tr( "The selected file could not be saved." ) );
        }

        delete writer;
    }
}

void FraqtiveMainWindow::on_actionCopyImage_triggered()
{
    QImage image = currentImage();

    QApplication::clipboard()->setImage( image );
}

QString FraqtiveMainWindow::getSaveFileName( const QString& title, const QString& fileName, QByteArray* selectedFormat, QFileDialog::Options options /*= 0*/ )
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

    QByteArray format = *selectedFormat;
    if ( format.isEmpty() || !formats.contains( format ) )
        format = formats.first();

    QString selectedFilter = filters.at( formats.indexOf( format ) );

    QString result = QFileDialog::getSaveFileName( this, title, fileName,
        filters.join( ";;" ), &selectedFilter, options );

    if ( !result.isEmpty() ) {
        int index = filters.indexOf( selectedFilter );
        if ( index >= 0 )
            format = formats.at( index );

        *selectedFormat = format;

        if ( QFileInfo( result ).suffix().isEmpty() )
            result += '.' + format;
    }

    return result;
}

QString FraqtiveMainWindow::getSaveImageName( QByteArray* selectedFormat )
{
    ConfigurationData* config = fraqtive()->configuration();

    QByteArray format = config->value( "SaveFormat" ).toByteArray();

    QString path = config->value( "SavePath", QDir::homePath() ).toString();
    QString fileName = QFileInfo( QDir( path ), tr( "fractal" ) ).absoluteFilePath();

    QString result = getSaveFileName( tr( "Save Image" ), fileName, &format );

    if ( !result.isEmpty() ) {
        config->setValue( "SaveFormat", format );
        config->setValue( "SavePath", QFileInfo( result ).absolutePath() );

        *selectedFormat = format;
    }

    return result;
}

QString FraqtiveMainWindow::getSaveSeriesName( QByteArray* selectedFormat )
{
    ConfigurationData* config = fraqtive()->configuration();

    QByteArray format = config->value( "SeriesFormat" ).toByteArray();

    QString path = config->value( "SeriesPath", QDir::homePath() ).toString();
    QString fileName = QFileInfo( QDir( path ), tr( "series" ) ).absoluteFilePath();

    QString result = getSaveFileName( tr( "Save Series" ), fileName, &format, QFileDialog::DontConfirmOverwrite );

    if ( !result.isEmpty() ) {
        config->setValue( "SeriesFormat", format );
        config->setValue( "SeriesPath", QFileInfo( result ).absolutePath() );

        *selectedFormat = format;
    }

    return result;
}

QImageWriter* FraqtiveMainWindow::createImageWriter( const QString& fileName, const QByteArray& format )
{
    QImageWriter* writer = new QImageWriter( fileName, format );

    if ( format == "tiff" )
        writer->setCompression( 1 );

    return writer;
}

QImage FraqtiveMainWindow::currentImage()
{
    if ( ImageView* imageView = qobject_cast<ImageView*>( m_ui.mainContainer->view() ) )
        return imageView->image();
    if ( MeshView* meshView = qobject_cast<MeshView*>( m_ui.mainContainer->view() ) )
        return meshView->image();
    return QImage();
}

void FraqtiveMainWindow::on_actionGenerateImage_triggered()
{
    GenerateImageDialog dialog( this );

    if ( dialog.exec() == QDialog::Accepted ) {
        QByteArray format;
        QString fileName = getSaveImageName( &format );

        if ( !fileName.isEmpty() ) {
            ImageGenerator generator( this );
            generator.setResolution( dialog.resolution() );
            generator.setParameters( m_model->fractalType(), m_model->position() );
            generator.setColorSettings( m_model->gradient(), m_model->backgroundColor(), m_model->colorMapping() );
            generator.setGeneratorSettings( dialog.generatorSettings() );
            generator.setViewSettings( dialog.viewSettings() );

            QProgressDialog progress( this );
            progress.setWindowModality( Qt::WindowModal );
            progress.setRange( 0, generator.maximumProgress() );
            progress.setWindowTitle( tr( "Generate Image" ) );
            progress.setLabelText( tr( "Calculating..." ) );
            progress.setValue( 0 );

            progress.setFixedHeight( progress.sizeHint().height() );
            progress.resize( 300, progress.height() );

            QEventLoop eventLoop;

            connect( &generator, SIGNAL( progressChanged( int ) ), &progress, SLOT( setValue( int ) ), Qt::QueuedConnection );
            connect( &generator, SIGNAL( completed() ), &eventLoop, SLOT( quit() ), Qt::QueuedConnection );
            connect( &progress, SIGNAL( canceled() ), &eventLoop, SLOT( quit() ) );

            if ( !generator.start() ) {
                QMessageBox::warning( this, tr( "Error" ), tr( "Not enough memory to generate image." ) );
                return;
            }

            eventLoop.exec();

            if ( !progress.wasCanceled() ) {
                QImageWriter* writer = createImageWriter( fileName, format );
                if ( !writer->write( generator.takeImage() ) ) {
                    QMessageBox::warning( this, tr( "Error" ), tr( "The selected file could not be saved." ) );
                }
                delete writer;
            }
        }
    }
}

void FraqtiveMainWindow::on_actionGenerateSeries_triggered()
{
    GenerateSeriesDialog dialog( this, m_model );

    if ( dialog.exec() == QDialog::Accepted ) {
        QByteArray format;
        QString fileName = getSaveSeriesName( &format );

        if ( !fileName.isEmpty() ) {
            QFileInfo info( fileName );

            ImageGenerator generator( this );
            generator.setImageCount( dialog.images() );
            generator.setResolution( dialog.resolution() );

            generator.setColorSettings( m_model->gradient(), m_model->backgroundColor(), m_model->colorMapping() );
            generator.setGeneratorSettings( dialog.generatorSettings() );
            generator.setViewSettings( dialog.viewSettings() );

            QProgressDialog progress( this );
            progress.setWindowModality( Qt::WindowModal );
            progress.setRange( 0, generator.maximumProgress() );
            progress.setWindowTitle( tr( "Generate Series" ) );
            progress.setValue( 0 );

            progress.setFixedHeight( progress.sizeHint().height() );
            progress.resize( 300, progress.height() );

            connect( &generator, SIGNAL( progressChanged( int ) ), &progress, SLOT( setValue( int ) ), Qt::QueuedConnection );

            QImage previous;

            for ( int i = 0; i < dialog.images(); i++ ) {
                progress.setLabelText( tr( "Calculating %1 of %2..." ).arg( i + 1 ).arg( dialog.images() ) );
    
                generator.setCurrentImage( i );

                Position position = m_model->position();

                double zoomTo = position.zoomFactor();
                double zoomFrom = zoomTo - dialog.zoomFactor();
                double angleTo = position.angle();
                double angleFrom = angleTo - dialog.angle();

                double a = (double)i / (double)( dialog.images() - 1 );

                position.setZoomFactor( zoomFrom + a * ( zoomTo - zoomFrom ) );
                position.setAngle( angleFrom + a * ( angleTo - angleFrom ) );

                generator.setParameters( m_model->fractalType(), position );

                QEventLoop eventLoop;

                connect( &generator, SIGNAL( completed() ), &eventLoop, SLOT( quit() ), Qt::QueuedConnection );
                connect( &progress, SIGNAL( canceled() ), &eventLoop, SLOT( quit() ) );

                if ( !generator.start() ) {
                    QMessageBox::warning( this, tr( "Error" ), tr( "Not enough memory to generate image." ) );
                    return;
                }

                eventLoop.exec();

                if ( progress.wasCanceled() )
                    break;

                QString fullName = info.completeBaseName() + QLatin1String( "." ) + QString::number( i ).rightJustified( 4, QLatin1Char( '0' ) ) + QLatin1String( "." ) + info.suffix();
                QString fullPath = info.absoluteDir().absoluteFilePath( fullName );

                QImage current = generator.takeImage();

                if ( !previous.isNull() ) {
                    QPainter painter( &current );

                    double angle = ( angleTo - angleFrom ) / (double)( dialog.images() - 1 );
                    double scale = pow( 10.0, ( zoomTo - zoomFrom ) / (double)( dialog.images() - 1 ) );

                    QTransform transform;
                    transform.translate( current.width() / 2, current.height() / 2 );
                    transform.rotate( angle );
                    transform.scale( scale, scale );
                    transform.translate( -current.width() / 2, -current.height() / 2 );

                    painter.setOpacity( dialog.blending() );
                    painter.setTransform( transform );
                    painter.setRenderHint( QPainter::SmoothPixmapTransform );

                    painter.drawImage( 0, 0, previous );

                    previous = QImage();
                }

                QImageWriter* writer = createImageWriter( fullPath, format );
                if ( !writer->write( current ) ) {
                    QMessageBox::warning( this, tr( "Error" ), tr( "The selected file could not be saved." ) );
                    delete writer;
                    break;
                }
                delete writer;

                if ( dialog.blending() > 0.01 )
                    previous = current;
            }
        }
    }
}

void FraqtiveMainWindow::on_action2DView_triggered()
{
    if ( !qobject_cast<ImageView*>( m_ui.mainContainer->view() ) ) {
        ImageView* view = new ImageView( m_ui.mainContainer, m_model->presenter() );
        view->setInteractive( true );

        m_model->presenter()->setView( view );
        m_ui.mainContainer->setView( view );

        m_model->setViewMode( ImageViewMode );

        view->setFocus();
    }

    action( "view2d" )->setChecked( true );
    action( "view3d" )->setChecked( false );
}

void FraqtiveMainWindow::on_action3DView_triggered()
{
    if ( !qobject_cast<MeshView*>( m_ui.mainContainer->view() ) ) {
        MeshView* view = new MeshView( m_ui.mainContainer, m_model->presenter() );

        m_model->presenter()->setView( view );
        m_ui.mainContainer->setView( view );

        m_model->clearHovering();

        m_model->setViewMode( MeshViewMode );

        view->setFocus();
    }

    action( "view2d" )->setChecked( false );
    action( "view3d" )->setChecked( true );
}

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

#ifndef FRAQTIVEMAINWINDOW_H
#define FRAQTIVEMAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "ui_fraqtivemainwindow.h"
#include "xmlui/client.h"

class FractalModel;
class Gradient;

class FraqtiveMainWindow : public QMainWindow, public XmlUi::Client
{
    Q_OBJECT
public:
    FraqtiveMainWindow();
    ~FraqtiveMainWindow();

public: // overrides
    bool eventFilter( QObject* watched, QEvent* e );

protected: // overrides
    void closeEvent( QCloseEvent* e );

private slots:
    void on_actionFractalType_triggered();
    void on_actionEditGradient_triggered();
    void on_actionFullScreen_triggered();
    void on_actionNavigateBack_triggered();
    void on_actionNavigateForward_triggered();
    void on_actionDefaultPosition_triggered();
    void on_actionLoadPreset_triggered();
    void on_actionSavePreset_triggered();
    void on_actionLoadBookmark_triggered();
    void on_actionSaveBookmark_triggered();
    void on_actionSaveImage_triggered();
    void on_actionCopyImage_triggered();
    void on_actionGenerateImage_triggered();
    void on_actionGenerateSeries_triggered();
    void on_action2DView_triggered();
    void on_action3DView_triggered();

    void customContextMenuRequested( const QPoint& pos );

    void positionChanged();
    void navigationChanged();

    void applyGradient( const Gradient& gradient );

private:
    bool isFullScreenMode() const;
    void enterFullScreenMode();
    void leaveFullScreenMode();

    QString getSaveFileName( const QString& title, const QString& fileName, QByteArray* selectedFormat, QFileDialog::Options options = 0 );
    QString getSaveImageName( QByteArray* selectedFormat );
    QString getSaveSeriesName( QByteArray* selectedFormat );

    QImageWriter* createImageWriter( const QString& fileName, const QByteArray& format );

    QImage currentImage();

private:
    Ui::FraqtiveMainWindow m_ui;

    FractalModel* m_model;
};

#endif

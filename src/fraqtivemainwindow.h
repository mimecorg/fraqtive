/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef FRAQTIVEMAINWINDOW_H
#define FRAQTIVEMAINWINDOW_H

#include <QMainWindow>

#include "ui_fraqtivemainwindow.h"

class FractalModel;
class Gradient;
class TutorialDialog;

class FraqtiveMainWindow : public QMainWindow
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
    void on_actionQuit_triggered();
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
    void on_action2DView_triggered();
    void on_action3DView_triggered();
    void on_actionQuickTutorial_triggered();
    void on_actionAboutFraqtive_triggered();
    void on_actionAboutQt_triggered();

    void positionChanged();
    void navigationChanged();

    void applyGradient( const Gradient& gradient );

private:
    bool isFullScreenMode() const;
    void enterFullScreenMode();
    void leaveFullScreenMode();

    QImage currentImage();

private:
    Ui::FraqtiveMainWindow m_ui;

    FractalModel* m_model;

    TutorialDialog* m_tutorialDialog;
};

#endif

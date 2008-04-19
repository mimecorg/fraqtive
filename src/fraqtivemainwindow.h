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
    void on_actionQuit_activated();
    void on_actionFractalType_activated();
    void on_actionEditGradient_activated();
    void on_actionFullScreen_activated();
    void on_actionNavigateBack_activated();
    void on_actionNavigateForward_activated();
    void on_actionDefaultPosition_activated();
    void on_actionLoadPreset_activated();
    void on_actionSavePreset_activated();
    void on_actionLoadBookmark_activated();
    void on_actionSaveBookmark_activated();
    void on_actionSaveImage_activated();
    void on_actionCopyImage_activated();
    void on_action2DView_activated();
    void on_action3DView_activated();
    void on_actionQuickTutorial_activated();
    void on_actionAboutFraqtive_activated();
    void on_actionAboutQt_activated();

    void positionChanged();
    void navigationChanged();

    void applyGradient( const Gradient& gradient );

private:
    bool isFullScreenMode() const;
    void enterFullScreenMode();
    void leaveFullScreenMode();

private:
    Ui::FraqtiveMainWindow m_ui;

    FractalModel* m_model;

    TutorialDialog* m_tutorialDialog;
};

#endif

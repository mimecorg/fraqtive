/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef ABOUTBOX_H
#define ABOUTBOX_H

#include <QDialog>
#include <QFrame>
#include <QScrollArea>

class AboutBoxSection;

class QBoxLayout;
class QLabel;

class AboutBox : public QDialog
{
    Q_OBJECT
public:
    AboutBox( const QString& title, const QString& message, QWidget* parent );
    ~AboutBox();

public:
    AboutBoxSection* addSection( const QPixmap& pixmap, const QString& message );

private:
    QWidget* m_sectionsWidget;
    QBoxLayout* m_sectionsLayout;
};

class AboutBoxSection : public QFrame
{
    Q_OBJECT
public:
    AboutBoxSection( const QPixmap& pixmap, const QString& message, QWidget* parent );
    ~AboutBoxSection();

protected:
    AboutBoxSection( Qt::WindowFlags flags );

public:
    void setPixmap( const QPixmap& pixmap );
    void setMessage( const QString& message );

    QPushButton* addButton( const QString& text );

    void clearButtons();

private:
    void initialize();

private:
    QBoxLayout* m_mainLayout;
    QBoxLayout* m_buttonsLayout;

    QLabel* m_pixmapLabel;
    QLabel* m_messageLabel;
};

class AboutBoxScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    AboutBoxScrollArea( QWidget* parent );
    ~AboutBoxScrollArea();

public: // overrides
    QSize sizeHint() const;
};

#endif

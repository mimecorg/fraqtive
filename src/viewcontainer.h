/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef VIEWCONTAINER_H
#define VIEWCONTAINER_H

#include <QWidget>

class QBoxLayout;

class ViewContainer : public QWidget
{
    Q_OBJECT
public:
    ViewContainer( QWidget* parent );
    ~ViewContainer();

public:
    void setView( QWidget* view );

public: // overrides
    QSize sizeHint() const;

private:
    QBoxLayout* m_layout;

    QWidget* m_view;
};

#endif

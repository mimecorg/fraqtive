/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef GENERATEIMAGEDIALOG_H
#define GENERATEIMAGEDIALOG_H

#include <QDialog>

#include "ui_generateimagedialog.h"
#include "datastructures.h"

class GenerateImageDialog : public QDialog
{
    Q_OBJECT
public:
    GenerateImageDialog( QWidget* parent );
    ~GenerateImageDialog();

public:
    QSize resolution() const { return m_resolution; }
    GeneratorSettings generatorSettings() const { return m_generatorSettings; }
    ViewSettings viewSettings() const { return m_viewSettings; }

public: // overrides
    void accept();

private:
    Ui::GenerateImageDialog m_ui;

    QSize m_resolution;
    GeneratorSettings m_generatorSettings;
    ViewSettings m_viewSettings;
};

#endif

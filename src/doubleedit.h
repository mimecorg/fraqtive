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

#ifndef DOUBLEEDIT_H
#define DOUBLEEDIT_H

#include <QLineEdit>

class DoubleEdit : public QLineEdit
{
    Q_OBJECT
public:
    DoubleEdit( QWidget* parent );
    ~DoubleEdit();

public:
    void setRange( double min, double max );

    void setValue( double value );
    double value() const { return m_value; }

signals:
    void valueChanged( double value );

protected: // overrides
    void focusOutEvent( QFocusEvent* e );

private slots:
    void on_textChanged( const QString& text );

private:
    void internalSetValue( double value );

private:
    double m_value;

    double m_minimum;
    double m_maximum;
};

#endif

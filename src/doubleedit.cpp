/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "doubleedit.h"

#include <QDoubleValidator>

DoubleEdit::DoubleEdit( QWidget* parent ) : QLineEdit( parent ),
    m_value( 0.0 ),
    m_minimum( -10.0 ),
    m_maximum( 10.0 )
{
    setValidator( new QDoubleValidator( this ) );

    connect( this, SIGNAL( textChanged( const QString& ) ), this, SLOT( on_textChanged( const QString& ) ) );
}

DoubleEdit::~DoubleEdit()
{
}

void DoubleEdit::setRange( double min, double max )
{
    m_minimum = min;
    m_maximum = max;

    setValue( m_value );
}

void DoubleEdit::setValue( double value )
{
    value = qBound( m_minimum, m_maximum, value );

    if ( m_value != value || text().isEmpty() ) {
        internalSetValue( value );
        setText( QString::number( m_value, 'g', 14 ) );
    }
}

void DoubleEdit::focusOutEvent( QFocusEvent* e )
{
    if ( !isReadOnly() ) {
        if ( hasAcceptableInput() ) {
            double value = text().toDouble();
            if ( value < m_minimum )
                internalSetValue( m_minimum );
            else if ( value > m_maximum )
                internalSetValue( m_maximum );
        }
        setText( QString::number( m_value, 'g', 14 ) );
    }

    QLineEdit::focusOutEvent( e );
}

void DoubleEdit::on_textChanged( const QString& text )
{
    if ( hasAcceptableInput() ) {
        double value = text.toDouble();
        if ( value >= m_minimum && value <= m_maximum )
            internalSetValue( value );
    }
}

void DoubleEdit::internalSetValue( double value )
{
    if ( m_value != value ) {
        m_value = value;
        emit valueChanged( value );
    }
}

/****************************************************************************
* Simple XML-based UI builder for Qt4
* Copyright (C) 2007-2012 Michał Męciński
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright
*      notice, this list of conditions and the following disclaimer in the
*      documentation and/or other materials provided with the distribution.
*   3. Neither the name of the copyright holder nor the names of the
*      contributors may be used to endorse or promote products derived from
*      this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
****************************************************************************/

#ifndef XMLUI_MACSTYLE_H
#define XMLUI_MACSTYLE_H

#include <QtGlobal>

#if !defined( Q_WS_MAC ) || defined( QT_NO_STYLE_AQUA )
#define XMLUI_NO_STYLE_MAC
#endif

#if !defined( XMLUI_NO_STYLE_MAC )

#include <QMacStyle>

namespace XmlUi
{

/**
* Better Qt style for Mac.
*
* The style override default QToolButton rendering which are ugly on mac os x.
*/
class MacStyle : public QMacStyle
{
    Q_OBJECT
public:
    /**
    * Constructor.
    */
    MacStyle();

    /**
    * Destructor.
    */
    ~MacStyle();

public: // overrides
    void drawComplexControl( ComplexControl control, const QStyleOptionComplex* option,
        QPainter* painter, const QWidget* widget ) const;
};

}

#endif // !defined( XMLUI_NO_STYLE_MAC )

#endif

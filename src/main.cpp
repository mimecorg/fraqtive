/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "fraqtiveapplication.h"

#ifdef HAVE_STATIC_JPEG
Q_IMPORT_PLUGIN( qjpeg )
#endif
#ifdef HAVE_STATIC_TIFF
Q_IMPORT_PLUGIN( qtiff )
#endif

int main( int argc, char* argv[] )
{
    FraqtiveApplication application( argc, argv );
    return application.exec();
}

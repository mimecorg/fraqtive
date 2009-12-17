/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef ABSTRACTJOBPROVIDER_H
#define ABSTRACTJOBPROVIDER_H

class AbstractJobProvider
{
public:
    AbstractJobProvider() { }
    virtual ~AbstractJobProvider() { }

public:
    virtual int priority() const = 0;

    virtual void executeJob() = 0;
};

#endif

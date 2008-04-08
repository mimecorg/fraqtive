/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Micha³ Mêciñski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H

#include <QAbstractListModel>

#include "abstractjobprovider.h"
#include "datastructures.h"

class BookmarkModel : public QAbstractListModel, public AbstractJobProvider
{
    Q_OBJECT
public:
    BookmarkModel( QObject* parent );
    ~BookmarkModel();

public:
    void setMap( BookmarkMap* map );
    BookmarkMap* map() const { return m_map; }

    void setColorSettings( const Gradient& gradient, const QColor& backgroundColor, const ColorMapping& mapping );

    void abortGeneration();
    void continueGeneration();

    void invalidateBookmark( const QString& name );

    void update();

public: // overrides
    int rowCount( const QModelIndex& parent ) const;

    QVariant data( const QModelIndex& index, int role ) const;

public: // AbstractJobProvider implementation
    int priority() const;

    void executeJob();

private:
    void calculate( const Bookmark& bookmark, double* buffer, const QSize& size );

    void addJobs( int count = -1 );
    void cancelJobs();
    void finishJob();

private:
    BookmarkMap* m_map;

    QRgb* m_gradientCache;
    QColor m_backgroundColor;
    ColorMapping m_colorMapping;

    mutable QMutex m_mutex;

    QStringList m_keys;
    QMap<QString, QPixmap> m_pixmaps;

    bool m_enabled;
    QStringList m_queue;

    int m_activeJobs;
    QWaitCondition m_allJobsDone;
};

#endif

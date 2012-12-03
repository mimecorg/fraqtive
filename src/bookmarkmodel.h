/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Micha³ Mêciñski
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

#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QMutex>
#include <QWaitCondition>

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
    void calculate( const Bookmark& bookmark, double* buffer, const QSize& size, const QSize& resolution );

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
    QMap<QString, QImage> m_images;

    bool m_enabled;
    QStringList m_queue;

    int m_activeJobs;
    QWaitCondition m_allJobsDone;
};

#endif

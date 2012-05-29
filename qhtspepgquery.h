/*
 *  Copyright (C) 2012 Robert Meijers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QHTSPEPGQUERY_H
#define QHTSPEPGQUERY_H

#include <QExplicitlySharedDataPointer>
#include <QObject>
#include <QString>

#include "qhtspchannel.h"
#include "qhtspeventlist.h"
#include "qhtspeventmodel.h"
#include "qhtsptag.h"

class QHtsp;

class QHtspEpgQueryData : public QObject, public QSharedData
{
    Q_OBJECT

public:
    QHtspEpgQueryData(QHtsp *htsp);
    QHtspEpgQueryData(const QHtspEpgQueryData &other);
    ~QHtspEpgQueryData() { }

    QHtspChannel *channel;
    QHtspEventList *events;
    QHtsp *htsp;
    QString query;
    QHtspTag *tag;
};

class QHtspEpgQuery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QHtspChannel *channel READ channel WRITE setChannel)
    Q_PROPERTY(QHtspEventList *events READ events)
    Q_PROPERTY(QHtspEventModel *eventsModel READ eventsModel)
    Q_PROPERTY(QString query READ query WRITE setQuery)
    Q_PROPERTY(QHtspTag *tag READ tag WRITE setTag)

public:
    explicit QHtspEpgQuery(QHtsp *htsp, QObject *parent = 0);
    QHtspEpgQuery(const QHtspEpgQuery &epgQuery, QObject *parent = 0);
    
    QHtspChannel *channel();
    QHtspEventList *events();
    QHtspEventModel *eventsModel();
    QString query();
    QHtspTag *tag();

    void setChannel(QHtspChannel *channel);
    void setQuery(QString query);
    void setTag(QHtspTag *tag);

    Q_INVOKABLE void run();

private:
    QExplicitlySharedDataPointer<QHtspEpgQueryData> d;
    QHtspEventModel *m_eventsModel;
};

#endif // QHTSPEPGQUERY_H

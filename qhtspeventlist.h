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

#ifndef QHTSPEVENTLIST_H
#define QHTSPEVENTLIST_H

#include <QObject>
#include <QList>

#include "qhtspevent.h"
#include "qhtspmessage.h"

class QHtspChannel;

class QHtspEventList : public QObject
{
    Q_OBJECT
public:
    explicit QHtspEventList(QHtspChannel *channel = 0, QObject *parent = 0);
    
    QHtspEvent *at(int i);
    QHtspChannel *channel();
    int count();
    int indexOf(QHtspEvent *event);
    QHtspEvent *find(qint64 id);
    QHtspEvent *findPreviousEvent(qint64 currentEventId);

    void beginGroupAdd();
    void endGroupAdd();

signals:
    void added(QHtspEvent *event);
    void changed(QHtspEvent *event);
    void removing(QHtspEvent *event);
    void groupAddBegun();
    void groupAddEnded();

public slots:
    void add(QHtspEvent *event);
    QHtspEvent *add(QHtspMessage &message);
    bool remove(QHtspEvent *event);
    bool remove(qint64 id);

    void _remove(QObject *event);

private:
    QHtspChannel *m_channel;
    QList<QHtspEvent *> m_events;
};

#endif // QHTSPEVENTLIST_H

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

#include "qhtspeventlist.h"

#include "qhtspchannel.h"

QHtspEventList::QHtspEventList(QHtspChannel *channel, QObject *parent) :
    QObject(parent), m_channel(channel)
{
}

void QHtspEventList::add(QHtspEvent *event)
{
    connect(event, SIGNAL(destroyed(QObject*)), this, SLOT(_remove(QObject*)));
    m_events.append(event);
    emit added(event);
}

QHtspEvent *QHtspEventList::add(QHtspMessage &message)
{
    QHtspEvent *event = new QHtspEvent(message, 0, this);
    add(event);
    return event;
}

QHtspEvent *QHtspEventList::at(int i)
{
    return m_events.at(i);
}

void QHtspEventList::beginGroupAdd()
{
    emit groupAddBegun();
}

QHtspChannel *QHtspEventList::channel()
{
    return m_channel;
}

int QHtspEventList::count()
{
    return m_events.count();
}

void QHtspEventList::endGroupAdd()
{
    emit groupAddEnded();
}

int QHtspEventList::indexOf(QHtspEvent *event)
{
    return m_events.indexOf(event);
}

QHtspEvent *QHtspEventList::find(qint64 id)
{
    int i;

    for(i = 0; i < m_events.count(); i++)
    {
        if(m_events[i]->id() == id)
            return m_events[i];
    }

    return NULL;
}

QHtspEvent *QHtspEventList::findPreviousEvent(qint64 currentEventId)
{
    int i;

    for(i = 0; i < m_events.count(); i++)
    {
        if(m_events[i]->nextEventId() == currentEventId)
            return m_events[i];
        //event list on channel always is sorted, so if current should be previous there isn't a previous
        else if(m_events[i]->id() == currentEventId)
            return 0;
    }

    return 0;
}

bool QHtspEventList::remove(QHtspEvent *event)
{
    disconnect(event, SIGNAL(destroyed(QObject*)), this, SLOT(_remove(QObject*)));
    emit removing(event);
    return m_events.removeOne(event);
}

bool QHtspEventList::remove(qint64 id)
{
    bool result;
    QHtspEvent *event = find(id);
    if(!event)
        return false;

    result = remove(event);
    delete event;

    return result;
}

void QHtspEventList::_remove(QObject *event)
{
    remove(static_cast<QHtspEvent*>(event));
}

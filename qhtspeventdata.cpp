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

#include "qhtspevent.h"

#include "qhtsp.h"

QHtspEventData::QHtspEventData(QHtsp *htsp, int id) :
    id(id), channelId(-1), htsp(htsp), nextEventId(-1), previousEvent(0), m_channel(0), m_loaded(false), m_nextEvent(0)
{
    if(previousEvent)
        connect(previousEvent, SIGNAL(destroyed()), this, SLOT(_previousEventDestroyed()));
}

QHtspEventData::QHtspEventData(const QHtspEventData &other) :
    QObject(0), QSharedData(other), id(other.id), channelId(other.channelId), description(other.description),
    htsp(other.htsp), nextEventId(other.nextEventId), previousEvent(other.previousEvent), start(other.start), stop(other.stop), title(other.title),
    m_channel(other.m_channel), m_loaded(other.m_loaded), m_nextEvent(other.m_nextEvent)
{
}

QHtspChannel *QHtspEventData::channel()
{
    if(!m_channel)
        m_channel = htsp->channels()->find(channelId);

    return m_channel;
}

QHtspEvent *QHtspEventData::nextEvent()
{
    if(!m_nextEvent && nextEventId >= 0)
    {
        m_nextEvent = htsp->events()->find(nextEventId);

        if(!m_nextEvent)
        {
            m_nextEvent = new QHtspEvent(htsp, nextEventId, htsp);

            htsp->events()->add(m_nextEvent);
            htsp->getEvent(nextEventId);
        }
    }

    return m_nextEvent;
}

void QHtspEventData::setId(qint64 id)
{
    if(this->id == id)
        return;

    this->id = id;
    emit idChanged();
}

void QHtspEventData::setChannelId(qint64 channelId)
{
    if(this->channelId == channelId)
        return;

    m_channel = 0;

    this->channelId = channelId;
    emit channelIdChanged();
}

void QHtspEventData::setDescription(QString description)
{
    if(this->description == description)
        return;

    this->description = description;
    emit descriptionChanged();
}

void QHtspEventData::setLongDescription(QString description)
{
    if(longDescription == description)
        return;

    longDescription = description;
    emit longDescriptionChanged();
}

void QHtspEventData::setNextEventId(qint64 nextEventId)
{
    if(this->nextEventId == nextEventId)
        return;

    this->nextEventId = nextEventId;
    m_nextEvent = 0;
    emit nextEventIdChanged();
}

void QHtspEventData::setPreviousEvent(QHtspEvent *event)
{
    previousEvent = event;
    emit previousEventChanged();
}

void QHtspEventData::setStart(QDateTime start)
{
    if(this->start == start)
        return;

    this->start = start;
    emit startChanged();
}

void QHtspEventData::setStop(QDateTime stop)
{
    if(this->stop == stop)
        return;

    this->stop = stop;
    emit stopChanged();
}

void QHtspEventData::setTitle(QString title)
{
    if(this->title == title)
        return;

    this->title = title;
    emit titleChanged();
}



void QHtspEventData::parseMessage(QHtspMessage &message)
{
    qint64 id;
    qint64 channelId;
    QString description;
    QString longDescription;
    qint64 nextEventId;
    qint64 start;
    qint64 stop;
    QString title;
    bool ok;

    id = message.getInt64("eventId", &ok);
    if(ok)
        setId(id);

    channelId = message.getInt64("channelId", &ok);
    if(ok)
        setChannelId(channelId);

    description = message.getString("description", &ok);
    if(ok)
        setDescription(description);

    longDescription = message.getString("ext_text", &ok);
    if(ok)
        setLongDescription(longDescription);

    nextEventId = message.getInt64("nextEventId", &ok);
    if(ok)
        setNextEventId(nextEventId);

    start = message.getInt64("start", &ok);
    if(ok)
        setStart(QDateTime::fromTime_t(start));

    stop = message.getInt64("stop", &ok);
    if(ok)
        setStop(QDateTime::fromTime_t(stop));

    title = message.getString("title", &ok);
    if(ok)
        setTitle(title);

    if(!m_loaded)
    {
        m_loaded = true;
        emit loaded();
    }
}

void QHtspEventData::_previousEventDestroyed()
{
    setPreviousEvent(0);
}

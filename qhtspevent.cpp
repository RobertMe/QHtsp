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
#include "qhtspchannel.h"

QHtspEvent::QHtspEvent(QHtsp *htsp, qint64 id, QObject *parent) :
    QObject(parent)
{
    d = new QHtspEventData(htsp, id);
    _connectSignals();
}

QHtspEvent::QHtspEvent(QHtspMessage &message, QHtsp *htsp, QObject *parent) :
    QObject(parent)
{
    d = new QHtspEventData(htsp, -1);
    d->parseMessage(message);
    _connectSignals();
}

QHtspEvent::QHtspEvent(const QHtspEvent &event, QObject *parent) :
    QObject(parent)
{
    d = event.d;
    _connectSignals();
}

qint64 QHtspEvent::id()
{
    return d->id;
}

QHtspChannel *QHtspEvent::channel()
{
    return d->channel();
}

qint64 QHtspEvent::channelId()
{
    return d->channelId;
}

QString QHtspEvent::description()
{
    return d->description;
}

QString QHtspEvent::longDescription()
{
    return d->longDescription;
}

QHtspEvent *QHtspEvent::nextEvent()
{
    return d->nextEvent();
}

qint64 QHtspEvent::nextEventId()
{
    return d->nextEventId;
}

QHtspEvent *QHtspEvent::previousEvent()
{
    return d->previousEvent();
}

QDateTime QHtspEvent::start()
{
    return d->start;
}

QDateTime QHtspEvent::stop()
{
    return d->stop;
}

QString QHtspEvent::title()
{
    return d->title;
}

void QHtspEvent::setId(qint64 id)
{
    d->setId(id);
}

void QHtspEvent::setChannelId(qint64 channelId)
{
    d->setChannelId(channelId);
}

void QHtspEvent::setDescription(QString description)
{
    d->setDescription(description);
}

void QHtspEvent::setLongDescription(QString description)
{
    d->setLongDescription(description);
}

void QHtspEvent::setNextEventId(qint64 nextEventId)
{
    d->setNextEventId(nextEventId);
}

void QHtspEvent::setStart(QDateTime start)
{
    d->setStart(start);
}

void QHtspEvent::setStop(QDateTime stop)
{
    d->setStop(stop);
}

void QHtspEvent::setTitle(QString title)
{
    d->setTitle(title);
}

bool QHtspEvent::record()
{
    if(!d->htsp)
        return false;

    d->htsp->addDvrEntry(id());
    return true;
}

void QHtspEvent::update(QHtspMessage &message)
{
    d->parseMessage(message);
}

void QHtspEvent::_connectSignals()
{
    connect(d.data(), SIGNAL(channelIdChanged()), this, SIGNAL(channelIdChanged()));
    connect(d.data(), SIGNAL(descriptionChanged()), this, SIGNAL(descriptionChanged()));
    connect(d.data(), SIGNAL(idChanged()), this, SIGNAL(idChanged()));
    connect(d.data(), SIGNAL(loaded()), this, SIGNAL(loaded()));
    connect(d.data(), SIGNAL(longDescriptionChanged()), this, SIGNAL(longDescriptionChanged()));
    connect(d.data(), SIGNAL(nextEventIdChanged()), this, SIGNAL(nextEventIdChanged()));
    connect(d.data(), SIGNAL(previousEventChanged()), this, SIGNAL(previousEventChanged()));
    connect(d.data(), SIGNAL(startChanged()), this, SIGNAL(startChanged()));
    connect(d.data(), SIGNAL(stopChanged()), this, SIGNAL(stopChanged()));
    connect(d.data(), SIGNAL(titleChanged()), this, SIGNAL(titleChanged()));
}

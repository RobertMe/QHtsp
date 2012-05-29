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

#include "qhtspchannel.h"

#include "qhtsp.h"

QHtspChannel::QHtspChannel(QHtsp *htsp, QObject *parent) :
    QObject(parent)
{
    d = new QHtspChannelData(this, htsp);
    _connectSignals();
}

QHtspChannel::QHtspChannel(QHtspMessage &message, QHtsp *htsp, QObject *parent) :
    QObject(parent)
{
    d = new QHtspChannelData(this, htsp);
    d->parseMessage(message);
    _connectSignals();

}

QHtspChannel::QHtspChannel(const QHtspChannel& channel, QObject *parent) :
    QObject(parent)
{
    d = channel.d;
    _connectSignals();
}

QHtspEvent *QHtspChannel::event()
{
    return d->event();
}

QHtspEventList *QHtspChannel::events()
{
    return d->events;
}

QHtspEventModel *QHtspChannel::eventsModel()
{
    return d->eventsModel();
}

qint64 QHtspChannel::eventId()
{
    return d->eventId;
}

QString QHtspChannel::iconUrl()
{
    return d->iconUrl;
}

qint64 QHtspChannel::id()
{
    return d->id;
}

QString QHtspChannel::name()
{
    return d->name;
}

qint64 QHtspChannel::number()
{
    return d->number;
}

QHtspService *QHtspChannel::service()
{
    return d->service;
}

void QHtspChannel::setEventId(qint64 eventId)
{
    d->setEventId(eventId);
}

void QHtspChannel::setIconUrl(QString url)
{
    d->setIconUrl(url);
}

void QHtspChannel::setId(qint64 id)
{
    d->setId(id);
}

void QHtspChannel::setName(QString name)
{
    d->setName(name);
}

void QHtspChannel::setNumber(qint64 number)
{
    d->setNumber(number);
}

void QHtspChannel::setService(QHtspService *service)
{
    d->setService(service);
}

void QHtspChannel::fetchNextEvents(int count)
{
    d->htsp->getEvents(events()->at(events()->count()-1)->nextEventId(), count - 1, events());
}

void QHtspChannel::update(QHtspMessage &message)
{
    d->parseMessage(message);
}

void QHtspChannel::_connectSignals()
{
    connect(d.data(), SIGNAL(eventIdChanged()), this, SIGNAL(eventIdChanged()));
    connect(d.data(), SIGNAL(iconUrlChanged()), this, SIGNAL(iconUrlChanged()));
    connect(d.data(), SIGNAL(idChanged()), this, SIGNAL(idChanged()));
    connect(d.data(), SIGNAL(nameChanged()), this, SIGNAL(nameChanged()));
    connect(d.data(), SIGNAL(numberChanged()), this, SIGNAL(numberChanged()));
    connect(d.data(), SIGNAL(serviceChanged()), this, SIGNAL(serviceChanged()));
}

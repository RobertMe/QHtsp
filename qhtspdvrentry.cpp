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

#include "qhtspdvrentry.h"

#include "qhtsp.h"

QHtspDvrEntry::QHtspDvrEntry(QHtsp *htsp, QObject *parent) :
    QObject(parent)
{
    d = new QHtspDvrEntryData(htsp);
}

QHtspDvrEntry::QHtspDvrEntry(QHtspMessage &message, QHtsp *htsp, QObject *parent) :
    QObject(parent)
{
    d = new QHtspDvrEntryData(htsp);
    d->parseMessage(message);
    _connectSignals();
}

QHtspDvrEntry::QHtspDvrEntry(const QHtspDvrEntry &dvrEntry, QObject *parent) :
    QObject(parent)
{
    d = dvrEntry.d;
    _connectSignals();
}

QHtspChannel *QHtspDvrEntry::channel()
{
    return d->channel();
}

qint64 QHtspDvrEntry::channelId()
{
    return d->channelId;
}

QString QHtspDvrEntry::description()
{
    return d->description;
}

QString QHtspDvrEntry::error()
{
    return d->error;
}

qint64 QHtspDvrEntry::id()
{
    return d->id;
}

QHtspDvrEntry::State QHtspDvrEntry::state()
{
    return (QHtspDvrEntry::State)d->state;
}

QDateTime QHtspDvrEntry::start()
{
    return d->start;
}

QDateTime QHtspDvrEntry::stop()
{
    return d->stop;
}

QString QHtspDvrEntry::title()
{
    return d->title;
}

void QHtspDvrEntry::setChannelId(qint64 channelId)
{
    d->setChannelId(channelId);
}

void QHtspDvrEntry::setDescription(QString description)
{
    d->setDescription(description);
}

void QHtspDvrEntry::setError(QString error)
{
    d->setError(error);
}

void QHtspDvrEntry::setId(qint64 id)
{
    d->setId(id);
}

void QHtspDvrEntry::setState(State state)
{
    d->setState(state);
}

void QHtspDvrEntry::setStart(QDateTime start)
{
    d->setStart(start);
}

void QHtspDvrEntry::setStop(QDateTime stop)
{
    d->setStop(stop);
}

void QHtspDvrEntry::setTitle(QString title)
{
    d->setTitle(title);
}

void QHtspDvrEntry::cancel()
{
    d->htsp->cancelDvrEntry(id());
}

void QHtspDvrEntry::remove()
{
    d->htsp->deleteDvrEntry(id());
}

void QHtspDvrEntry::update(QHtspMessage &message)
{
    d->parseMessage(message);
}

void QHtspDvrEntry::_connectSignals()
{
    connect(d.data(), SIGNAL(channelChanged()), this, SIGNAL(channelChanged()));
    connect(d.data(), SIGNAL(descriptionChanged()), this, SIGNAL(descriptionChanged()));
    connect(d.data(), SIGNAL(errorChanged()), this, SIGNAL(errorChanged()));
    connect(d.data(), SIGNAL(idChanged()), this, SIGNAL(idChanged()));
    connect(d.data(), SIGNAL(startChanged()), this, SIGNAL(startChanged()));
    connect(d.data(), SIGNAL(stopChanged()), this, SIGNAL(stopChanged()));
    connect(d.data(), SIGNAL(titleChanged()), this, SIGNAL(titleChanged()));
}

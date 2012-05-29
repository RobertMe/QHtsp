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

#include "qhtspservice.h"

QHtspService::QHtspService(QHtspChannel *channel, QObject *parent) :
    QObject(parent)
{
    d = new QHtspServiceData(channel);
    _connectSignals();
}

QHtspService::QHtspService(QHtspMessage &message, QHtspChannel *channel, QObject *parent) :
    QObject(parent)
{
    d = new QHtspServiceData(channel);
    d->parseMessage(message);
    _connectSignals();
}

QHtspService::QHtspService(const QHtspService &service, QObject *parent) :
    QObject(parent)
{
    d = service.d;
    _connectSignals();
}

qint64 QHtspService::caid()
{
    return d->caid;
}

QString QHtspService::caname()
{
    return d->caname;
}

QString QHtspService::name()
{
    return d->name;
}

QHtspService::Type QHtspService::type()
{
    return (QHtspService::Type)d->type;
}

void QHtspService::setCaid(qint64 caid)
{
    d->setCaid(caid);
}

void QHtspService::setCaname(QString caname)
{
    return d->setCaname(caname);
}

void QHtspService::setName(QString name)
{
    return d->setName(name);
}

void QHtspService::setType(QHtspService::Type type)
{
    return d->setType(type);
}

void QHtspService::_connectSignals()
{
    connect(d.data(), SIGNAL(caidChanged()), this, SIGNAL(caidChanged()));
    connect(d.data(), SIGNAL(canameChanged()), this, SIGNAL(canameChanged()));
    connect(d.data(), SIGNAL(nameChanged()), this, SIGNAL(typeChanged()));
    connect(d.data(), SIGNAL(typeChanged()), this, SIGNAL(nameChanged()));
}

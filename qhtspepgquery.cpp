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

#include "qhtspepgquery.h"

#include "qhtsp.h"

QHtspEpgQuery::QHtspEpgQuery(QHtsp *htsp, QObject */*parent*/)
    : m_eventsModel(0)
{
    d = new QHtspEpgQueryData(htsp);
}

QHtspEpgQuery::QHtspEpgQuery(const QHtspEpgQuery &epgQuery, QObject */*parent*/)
    : QObject(0), m_eventsModel(0)
{
    d = epgQuery.d;
}

QHtspChannel *QHtspEpgQuery::channel()
{
    return d->channel;
}

QHtspEventList *QHtspEpgQuery::events()
{
    return d->events;
}

QHtspEventModel *QHtspEpgQuery::eventsModel()
{
    if(!m_eventsModel)
        m_eventsModel = new QHtspEventModel(events());

    return m_eventsModel;
}

QString QHtspEpgQuery::query()
{
    return d->query;
}

QHtspTag *QHtspEpgQuery::tag()
{
    return d->tag;
}

void QHtspEpgQuery::setChannel(QHtspChannel *channel)
{
    d->channel = channel;
}

void QHtspEpgQuery::setQuery(QString query)
{
    d->query = query;
}

void QHtspEpgQuery::setTag(QHtspTag *tag)
{
    d->tag = tag;
}

void QHtspEpgQuery::run()
{
    d->htsp->queryEpg(this);
}

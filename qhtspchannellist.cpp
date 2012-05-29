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

#include "qhtspchannellist.h"

QHtspChannelList::QHtspChannelList(QObject *parent) :
    QObject(parent)
{
}

void QHtspChannelList::add(QHtspChannel *channel)
{
    m_channels.append(channel);
    emit added(channel);
}

QHtspChannel *QHtspChannelList::add(QHtsp *htsp, QHtspMessage &message)
{
    QHtspChannel *channel = new QHtspChannel(message, htsp, this);
    add(channel);
    return channel;
}

QHtspChannel *QHtspChannelList::at(int i)
{
    return m_channels.at(i);
}

int QHtspChannelList::count()
{
    return m_channels.count();
}

int QHtspChannelList::indexOf(QHtspChannel *channel)
{
    return m_channels.indexOf(channel);
}

QHtspChannel *QHtspChannelList::find(qint64 id)
{
    int i;

    for(i = 0; i < m_channels.count(); i++)
    {
        if(m_channels[i]->id() == id)
            return m_channels[i];
    }

    return NULL;
}

bool QHtspChannelList::remove(QHtspChannel *channel)
{
    emit removing(channel);
    return m_channels.removeOne(channel);
}

bool QHtspChannelList::remove(qint64 id)
{
    bool result;
    QHtspChannel *channel = find(id);
    if(!channel)
        return false;

    result = remove(channel);
    delete channel;

    return result;
}

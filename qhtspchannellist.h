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

#ifndef QHTSPCHANNELLIST_H
#define QHTSPCHANNELLIST_H

#include <QObject>
#include <QList>

#include "qhtspchannel.h"
#include "qhtspmessage.h"

class QHtspChannelList : public QObject
{
    Q_OBJECT
public:
    explicit QHtspChannelList(QObject *parent = 0);
    
    QHtspChannel *at(int i);
    int count();
    int indexOf(QHtspChannel *channel);
    QHtspChannel *find(qint64 id);

signals:
    void added(QHtspChannel *channel);
    void changed(QHtspChannel *changed);
    void removing(QHtspChannel *channel);
    
public slots:
    void add(QHtspChannel *channel);
    QHtspChannel *add(QHtsp *htsp, QHtspMessage &message);
    bool remove(QHtspChannel *channel);
    bool remove(qint64 id);

private:
    QList<QHtspChannel *> m_channels;
};

#endif // QHTSPCHANNELLIST_H

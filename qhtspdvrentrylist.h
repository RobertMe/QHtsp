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

#ifndef QHTSPDVRENTRYLIST_H
#define QHTSPDVRENTRYLIST_H

#include <QObject>
#include <QList>

#include "qhtspdvrentry.h"
#include "qhtspmessage.h"

class QHtspDvrEntryList : public QObject
{
    Q_OBJECT
public:
    explicit QHtspDvrEntryList(QObject *parent = 0);

    QHtspDvrEntry *at(int i);
    int count();
    int indexOf(QHtspDvrEntry *dvrEntry);
    QHtspDvrEntry *find(qint64 id);

signals:
    void added(QHtspDvrEntry *channel);
    void changed(QHtspDvrEntry *changed);
    void removing(QHtspDvrEntry *channel);

public slots:
    void add(QHtspDvrEntry *dvrEntry);
    QHtspDvrEntry *add(QHtsp *htsp, QHtspMessage &message);
    bool remove(QHtspDvrEntry *dvrEntry);
    bool remove(qint64 id);

private:
    QList<QHtspDvrEntry *> m_dvrEntries;
};

#endif // QHTSPDVRENTRYLIST_H

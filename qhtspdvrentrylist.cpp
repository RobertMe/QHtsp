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

#include "qhtspdvrentrylist.h"

QHtspDvrEntryList::QHtspDvrEntryList(QObject *parent) :
    QObject(parent)
{
}
void QHtspDvrEntryList::add(QHtspDvrEntry *dvrEntry)
{
    m_dvrEntries.append(dvrEntry);
    emit added(dvrEntry);
}

QHtspDvrEntry *QHtspDvrEntryList::add(QHtsp *htsp, QHtspMessage &message)
{
    QHtspDvrEntry *dvrEntry = new QHtspDvrEntry(message, htsp, this);
    add(dvrEntry);
    return dvrEntry;
}

QHtspDvrEntry *QHtspDvrEntryList::at(int i)
{
    return m_dvrEntries.at(i);
}

int QHtspDvrEntryList::count()
{
    return m_dvrEntries.count();
}

int QHtspDvrEntryList::indexOf(QHtspDvrEntry *dvrEntry)
{
    return m_dvrEntries.indexOf(dvrEntry);
}

QHtspDvrEntry *QHtspDvrEntryList::find(qint64 id)
{
    int i;

    for(i = 0; i < m_dvrEntries.count(); i++)
    {
        if(m_dvrEntries[i]->id() == id)
            return m_dvrEntries[i];
    }

    return NULL;
}

bool QHtspDvrEntryList::remove(QHtspDvrEntry *dvrEntry)
{
    emit removing(dvrEntry);
    return m_dvrEntries.removeOne(dvrEntry);
}

bool QHtspDvrEntryList::remove(qint64 id)
{
    bool result;
    QHtspDvrEntry *dvrEntry = find(id);
    if(!dvrEntry)
        return false;

    result = remove(dvrEntry);
    delete dvrEntry;

    return result;
}

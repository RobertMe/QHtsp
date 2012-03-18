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

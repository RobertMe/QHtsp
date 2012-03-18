#include "qhtspeventlist.h"

#include "qhtspchannel.h"

QHtspEventList::QHtspEventList(QHtspChannel *channel, QObject *parent) :
    QObject(parent), m_channel(channel)
{
}

void QHtspEventList::add(QHtspEvent *event)
{
    connect(event, SIGNAL(destroyed(QObject*)), this, SLOT(_remove(QObject*)));
    m_events.append(event);
    emit added(event);
}

QHtspEvent *QHtspEventList::add(QHtspMessage &message)
{
    QHtspEvent *event = new QHtspEvent(message, 0, this);
    add(event);
    return event;
}

QHtspEvent *QHtspEventList::at(int i)
{
    return m_events.at(i);
}

void QHtspEventList::beginGroupAdd()
{
    emit groupAddBegun();
}

QHtspChannel *QHtspEventList::channel()
{
    return m_channel;
}

int QHtspEventList::count()
{
    return m_events.count();
}

void QHtspEventList::endGroupAdd()
{
    emit groupAddEnded();
}

int QHtspEventList::indexOf(QHtspEvent *event)
{
    return m_events.indexOf(event);
}

QHtspEvent *QHtspEventList::find(qint64 id)
{
    int i;

    for(i = 0; i < m_events.count(); i++)
    {
        if(m_events[i]->id() == id)
            return m_events[i];
    }

    return NULL;
}

bool QHtspEventList::remove(QHtspEvent *event)
{
    disconnect(event, SIGNAL(destroyed(QObject*)), this, SLOT(_remove(QObject*)));
    emit removing(event);
    return m_events.removeOne(event);
}

bool QHtspEventList::remove(qint64 id)
{
    bool result;
    QHtspEvent *event = find(id);
    if(!event)
        return false;

    result = remove(event);
    delete event;

    return result;
}

void QHtspEventList::_remove(QObject *event)
{
    remove(static_cast<QHtspEvent*>(event));
}

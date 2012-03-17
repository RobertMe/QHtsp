#include "qhtspevent.h"

#include "qhtsp.h"
#include "qhtspchannel.h"

QHtspEvent::QHtspEvent(QHtsp *htsp, qint64 id, QObject *parent) :
    QObject(parent), m_id(id), m_channel(0), m_channelId(-1), m_htsp(htsp)
{
    m_loaded = false;
    m_nextEventId = -1;
}

QHtspEvent::QHtspEvent(QHtspMessage &message, QHtsp *htsp, QObject *parent) :
    QObject(parent), m_id(-1), m_channel(0), m_channelId(-1), m_htsp(htsp)
{
    m_loaded = false;
    m_nextEventId = -1;
    _parseMessage(message);
}

QHtspEvent::QHtspEvent(const QHtspEvent &event, QObject *parent) :
    QObject(parent)
{
    m_id = event.m_id;
    m_channelId = event.m_channelId;
    m_description = event.m_description;
    m_htsp = event.m_htsp;
    m_nextEventId = event.m_nextEventId;
    m_start = event.m_start;
    m_stop = event.m_stop;
    m_title = event.m_title;
    m_loaded = event.m_loaded;
}

qint64 QHtspEvent::id()
{
    return m_id;
}

QHtspChannel *QHtspEvent::channel()
{
    if(!m_channel)
        m_channel = m_htsp->channels()->find(channelId());

    return m_channel;
}

qint64 QHtspEvent::channelId()
{
    return m_channelId;
}

QString QHtspEvent::description()
{
    return m_description;
}

qint64 QHtspEvent::nextEventId()
{
    return m_nextEventId;
}

QDateTime QHtspEvent::start()
{
    return m_start;
}

QDateTime QHtspEvent::stop()
{
    return m_stop;
}

QString QHtspEvent::title()
{
    return m_title;
}

void QHtspEvent::setId(qint64 id)
{
    if(m_id == id)
        return;

    m_id = id;
    emit idChanged();
}

void QHtspEvent::setChannelId(qint64 channelId)
{
    if(m_channelId == channelId)
        return;

    m_channel = 0;

    m_channelId = channelId;
    emit channelIdChanged();
}

void QHtspEvent::setDescription(QString description)
{
    if(m_description == description)
        return;

    m_description = description;
    emit descriptionChanged();
}

void QHtspEvent::setNextEventId(qint64 nextEventId)
{
    if(m_nextEventId == nextEventId)
        return;

    m_nextEventId = nextEventId;
    emit nextEventIdChanged();
}

void QHtspEvent::setStart(QDateTime start)
{
    if(m_start == start)
        return;

    m_start = start;
    emit startChanged();
}

void QHtspEvent::setStop(QDateTime stop)
{
    if(m_stop == stop)
        return;

    m_stop = stop;
    emit stopChanged();
}

void QHtspEvent::setTitle(QString title)
{
    if(m_title == title)
        return;

    m_title = title;
    emit titleChanged();
}

bool QHtspEvent::record()
{
    if(!m_htsp)
        return false;

    m_htsp->addDvrEntry(id());
    return true;
}

void QHtspEvent::update(QHtspMessage &message)
{
    _parseMessage(message);
}

void QHtspEvent::_parseMessage(QHtspMessage &message)
{
    qint64 id;
    qint64 channelId;
    QString description;
    qint64 nextEventId;
    qint64 start;
    qint64 stop;
    QString title;
    bool ok;

    id = message.getInt64("eventId", &ok);
    if(ok)
        setId(id);

    channelId = message.getInt64("channelId", &ok);
    if(ok)
        setChannelId(channelId);

    description = message.getString("description", &ok);
    if(ok)
        setDescription(description);

    nextEventId = message.getInt64("nextEventId", &ok);
    if(ok)
        setNextEventId(nextEventId);

    start = message.getInt64("start", &ok);
    if(ok)
        setStart(QDateTime::fromTime_t(start));

    stop = message.getInt64("stop", &ok);
    if(ok)
        setStop(QDateTime::fromTime_t(stop));

    title = message.getString("title", &ok);
    if(ok)
        setTitle(title);

    if(!m_loaded)
    {
        m_loaded = true;
        emit loaded();
    }
}

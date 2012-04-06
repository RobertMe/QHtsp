#include "qhtspevent.h"

#include "qhtsp.h"

QHtspEventData::QHtspEventData(QHtsp *htsp, int id) :
    id(id), channelId(-1), htsp(htsp), nextEventId(-1), m_channel(0), m_loaded(false)
{
}

QHtspEventData::QHtspEventData(const QHtspEventData &other) :
    QObject(0), QSharedData(other), id(other.id), channelId(other.channelId), description(other.description),
    htsp(other.htsp), nextEventId(other.nextEventId), start(other.start), stop(other.stop), title(other.title),
    m_channel(other.m_channel), m_loaded(other.m_loaded)
{
}

QHtspChannel *QHtspEventData::channel()
{
    if(!m_channel)
        m_channel = htsp->channels()->find(channelId);

    return m_channel;
}

void QHtspEventData::setId(qint64 id)
{
    if(this->id == id)
        return;

    this->id = id;
    emit idChanged();
}

void QHtspEventData::setChannelId(qint64 channelId)
{
    if(this->channelId == channelId)
        return;

    m_channel = 0;

    this->channelId = channelId;
    emit channelIdChanged();
}

void QHtspEventData::setDescription(QString description)
{
    if(this->description == description)
        return;

    this->description = description;
    emit descriptionChanged();
}

void QHtspEventData::setNextEventId(qint64 nextEventId)
{
    if(this->nextEventId == nextEventId)
        return;

    this->nextEventId = nextEventId;
    emit nextEventIdChanged();
}

void QHtspEventData::setStart(QDateTime start)
{
    if(this->start == start)
        return;

    this->start = start;
    emit startChanged();
}

void QHtspEventData::setStop(QDateTime stop)
{
    if(this->stop == stop)
        return;

    this->stop = stop;
    emit stopChanged();
}

void QHtspEventData::setTitle(QString title)
{
    if(this->title == title)
        return;

    this->title = title;
    emit titleChanged();
}



void QHtspEventData::parseMessage(QHtspMessage &message)
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

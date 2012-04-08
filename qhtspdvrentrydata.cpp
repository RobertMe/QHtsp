#include "qhtspdvrentry.h"

#include "qhtsp.h"

QHtspDvrEntryData::QHtspDvrEntryData(QHtsp *htsp)
    : htsp(htsp), m_channel(0)
{
}

QHtspDvrEntryData::QHtspDvrEntryData(const QHtspDvrEntryData &other)
    : QObject(0), QSharedData(other), channelId(other.channelId), description(other.description), error(other.error), htsp(other.htsp),
      id(other.id), state(other.state), start(other.start), stop(other.stop), title(other.title), m_channel(other.m_channel)
{
}

QHtspChannel *QHtspDvrEntryData::channel()
{
    if(!m_channel)
        m_channel = htsp->channels()->find(channelId);

    return m_channel;
}

void QHtspDvrEntryData::setChannelId(qint64 channelId)
{
    if(this->channelId == channelId)
        return;

    m_channel = 0;
    this->channelId = channelId;
    emit channelChanged();
}

void QHtspDvrEntryData::setDescription(QString description)
{
    if(this->description == description)
        return;

    this->description = description;
    emit descriptionChanged();
}

void QHtspDvrEntryData::setError(QString error)
{
    if(this->error == error)
        return;

    this->error = error;
    emit errorChanged();
}

void QHtspDvrEntryData::setId(qint64 id)
{
    if(this->id == id)
        return;

    this->id = id;
    emit idChanged();
}

void QHtspDvrEntryData::setState(quint16 state)
{
    if(this->state == state)
        return;

    this->state = state;
    emit stateChanged();
}

void QHtspDvrEntryData::setStart(QDateTime start)
{
    if(this->start == start)
        return;

    this->start = start;
    emit startChanged();
}

void QHtspDvrEntryData::setStop(QDateTime stop)
{
    if(this->stop == stop)
        return;

    this->stop = stop;
    emit stopChanged();
}

void QHtspDvrEntryData::setTitle(QString title)
{
    if(this->title == title)
        return;

    this->title = title;
    emit titleChanged();
}

void QHtspDvrEntryData::parseMessage(QHtspMessage &message)
{
    qint64 channelId;
    QString description;
    QString error;
    qint64 id;
    QString state;
    qint64 start;
    qint64 stop;
    QString title;
    bool ok;

    id = message.getInt64("id", &ok);
    if(ok)
        setId(id);

    channelId = message.getInt64("channel", &ok);
    if(ok)
        setChannelId(channelId);

    description = message.getString("description", &ok);
    if(ok)
        setDescription(description);

    error = message.getString("error", &ok);
    if(ok)
        setError(error);

    state = message.getString("state", &ok);
    if(ok)
    {
        if(state == "completed")
            setState(QHtspDvrEntry::Completed);
        else if(state == "invalid")
            setState(QHtspDvrEntry::Invalid);
        else if(state == "recording")
            setState(QHtspDvrEntry::Recording);
        else if(state == "scheduled")
            setState(QHtspDvrEntry::Scheduled);
        else if(state == "missed")
            setState(QHtspDvrEntry::Missed);
    }

    start = message.getInt64("start", &ok);
    if(ok)
        setStart(QDateTime::fromTime_t(start));

    stop = message.getInt64("stop", &ok);
    if(ok)
        setStop(QDateTime::fromTime_t(stop));

    title = message.getString("title", &ok);
    if(ok)
        setTitle(title);
}

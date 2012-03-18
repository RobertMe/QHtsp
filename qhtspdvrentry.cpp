#include "qhtspdvrentry.h"

#include "qhtsp.h"

QHtspDvrEntry::QHtspDvrEntry(QHtsp *htsp, QObject *parent) :
    QObject(parent), m_channel(0), m_htsp(htsp)
{
}

QHtspDvrEntry::QHtspDvrEntry(QHtspMessage &message, QHtsp *htsp, QObject *parent) :
    QObject(parent), m_channel(0), m_htsp(htsp)
{
    _parseMessage(message);
}

QHtspDvrEntry::QHtspDvrEntry(const QHtspDvrEntry &dvrEntry, QObject *parent) :
    QObject(parent), m_channel(0)
{
    m_channelId = dvrEntry.m_channelId;
    m_description = dvrEntry.m_description;
    m_error = dvrEntry.m_error;
    m_htsp = dvrEntry.m_htsp;
    m_id = dvrEntry.m_id;
    m_state = dvrEntry.m_state;
    m_start = dvrEntry.m_start;
    m_stop = dvrEntry.m_stop;
    m_title = dvrEntry.m_title;
}

QHtspChannel *QHtspDvrEntry::channel()
{
    if(!m_channel)
        m_channel = m_htsp->channels()->find(channelId());

    return m_channel;
}

qint64 QHtspDvrEntry::channelId()
{
    return m_channelId;
}

QString QHtspDvrEntry::description()
{
    return m_description;
}

QString QHtspDvrEntry::error()
{
    return m_error;
}

qint64 QHtspDvrEntry::id()
{
    return m_id;
}

QHtspDvrEntry::State QHtspDvrEntry::state()
{
    return m_state;
}

QDateTime QHtspDvrEntry::start()
{
    return m_start;
}

QDateTime QHtspDvrEntry::stop()
{
    return m_stop;
}

QString QHtspDvrEntry::title()
{
    return m_title;
}

void QHtspDvrEntry::setChannelId(qint64 channelId)
{
    if(m_channelId == channelId)
        return;

    m_channel = 0;
    m_channelId = channelId;
    emit channelChanged();
}

void QHtspDvrEntry::setDescription(QString description)
{
    if(m_description == description)
        return;

    m_description = description;
    emit descriptionChanged();
}

void QHtspDvrEntry::setError(QString error)
{
    if(m_error == error)
        return;

    m_error = error;
    emit errorChanged();
}

void QHtspDvrEntry::setId(qint64 id)
{
    if(m_id == id)
        return;

    m_id = id;
    emit idChanged();
}

void QHtspDvrEntry::setState(State state)
{
    if(m_state == state)
        return;

    m_state = state;
    emit stateChanged();
}

void QHtspDvrEntry::setStart(QDateTime start)
{
    if(m_start == start)
        return;

    m_start = start;
    emit startChanged();
}

void QHtspDvrEntry::setStop(QDateTime stop)
{
    if(m_stop == stop)
        return;

    m_stop = stop;
    emit stopChanged();
}

void QHtspDvrEntry::setTitle(QString title)
{
    if(m_title == title)
        return;

    m_title = title;
    emit titleChanged();
}

void QHtspDvrEntry::update(QHtspMessage &message)
{
    _parseMessage(message);
}

void QHtspDvrEntry::_parseMessage(QHtspMessage &message)
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
            setState(Completed);
        else if(state == "invalid")
            setState(Invalid);
        else if(state == "recording")
            setState(Recording);
        else if(state == "scheduled")
            setState(Scheduled);
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

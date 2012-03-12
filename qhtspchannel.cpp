#include "qhtspchannel.h"

#include "qhtsp.h"

QHtspChannel::QHtspChannel(QHtsp *htsp, QObject *parent) :
    QObject(parent), m_events(this), m_eventModel(&m_events)
{
    m_event = NULL;
    m_htsp = htsp;
    m_id = -1;
    m_eventId = -1;
    m_number = 0;
}

QHtspChannel::QHtspChannel(QHtspMessage &message, QHtsp *htsp, QObject *parent) :
    QObject(parent), m_events(this), m_eventModel(&m_events)
{
    m_event = NULL;
    m_htsp = htsp;
    m_id = -1;
    m_eventId = -1;
    m_number = -1;
    _parseMessage(message);
}

QHtspEvent *QHtspChannel::event()
{
    if(m_eventId >= 0 && (!m_event || m_event->id() != m_eventId))
    {
        if(m_htsp)
            m_event = m_htsp->events()->find(m_eventId);

        if(!m_event)
        {
            m_event = new QHtspEvent(m_htsp, m_eventId, this);

            if(m_htsp)
            {
                m_htsp->events()->add(m_event);
                m_htsp->getEvent(m_eventId);
            }
        }

        if(m_events.count() == 0)
            m_events.add(m_event);
    }

    return m_event;
}

QHtspEventList *QHtspChannel::events()
{
    return &m_events;
}

QHtspEventModel *QHtspChannel::eventsModel()
{
    return &m_eventModel;
}

qint64 QHtspChannel::eventId()
{
    return m_eventId;
}

QString QHtspChannel::iconUrl()
{
    return m_iconUrl;
}

qint64 QHtspChannel::id()
{
    return m_id;
}

QString QHtspChannel::name()
{
    return m_name;
}

qint64 QHtspChannel::number()
{
    return m_number;
}

void QHtspChannel::setEventId(qint64 eventId)
{
    if(m_eventId == eventId)
        return;

    m_eventId = eventId;

    if(m_event)
    {
        if(m_htsp)
            m_htsp->events()->remove(m_event);

        m_events.remove(m_event);
        delete m_event;
    }

    emit eventIdChanged();
}

void QHtspChannel::setIconUrl(QString url)
{
    if(m_iconUrl == url)
        return;

    m_iconUrl = url;
    emit iconUrlChanged();
}

void QHtspChannel::setId(qint64 id)
{
    if(m_id == id)
        return;

    m_id = id;
    emit idChanged();
}

void QHtspChannel::setName(QString name)
{
    m_name = name;
    emit nameChanged();
}

void QHtspChannel::setNumber(qint64 number)
{
    if(m_number == number)
        return;

    m_number = number;
    emit numberChanged();
}

void QHtspChannel::fetchNextEvents(int count)
{
    if(!m_htsp)
        return;

    m_htsp->getEvents(events()->at(events()->count()-1)->nextEventId(), count - 1, events());
}

void QHtspChannel::update(QHtspMessage &message)
{
    _parseMessage(message);
}

void QHtspChannel::_parseMessage(QHtspMessage &message)
{
    int eventId;
    QString icon;
    qint64 id;
    QString name;
    qint64 number;
    bool ok;

    id = message.getInt64("channelId", &ok);
    if(ok)
        setId(id);

    name = message.getString("channelName", &ok);
    if(ok)
        setName(name);

    number = message.getInt64("channelNumber", &ok);
    if(ok)
        setNumber(number);

    icon = message.getString("channelIcon", &ok);
    if(ok)
        setIconUrl(icon);

    eventId = message.getInt64("eventId", &ok);
    if(ok)
        setEventId(eventId);
}

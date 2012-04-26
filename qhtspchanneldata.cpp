#include "qhtspchannel.h"

#include "qhtsp.h"

QHtspChannelData::QHtspChannelData(QHtspChannel *channel, QHtsp *htsp)
    : events(new QHtspEventList(channel, this)), htsp(htsp), m_eventModel(0)
{
    m_channel = channel;
    m_event = 0;
    eventId = -1;
}

QHtspChannelData::QHtspChannelData(const QHtspChannelData &other)
    : QObject(), QSharedData(other), events(new QHtspEventList(other.m_channel, this)), m_eventModel(0)
{
    m_channel = other.m_channel;
    m_event = 0;
    events = other.events;
    eventId = other.eventId;
    htsp = other.htsp;
    iconUrl = other.iconUrl;
    id = other.id;
    name = other.name;
    number = other.number;
}

QHtspEvent *QHtspChannelData::event()
{
    if(eventId >= 0 && (!m_event || m_event->id() != eventId))
    {
        m_event = htsp->events()->find(eventId);

        if(!m_event)
        {
            m_event = new QHtspEvent(htsp, eventId, parent());

            htsp->events()->add(m_event);
            htsp->getEvent(eventId);
        }

        if(events->count() == 0)
            events->add(m_event);
    }

    return m_event;
}

QHtspEventModel *QHtspChannelData::eventsModel()
{
    if(!m_eventModel)
        m_eventModel = new QHtspEventModel(events);

    return m_eventModel;
}

void QHtspChannelData::setEventId(qint64 eventId)
{
    if(this->eventId == eventId)
        return;

    this->eventId = eventId;

    if(m_event)
    {
        htsp->events()->remove(m_event);

        events->remove(m_event);
        delete m_event;
        m_event = 0;
    }

    while(events->count() > 0)
    {
        QHtspEvent *event = events->at(0);

        if(event->id() == eventId)
            break;

        htsp->events()->remove(event);
        events->remove(event);
        delete event;
    }

    emit eventIdChanged();
}

void QHtspChannelData::setIconUrl(QString url)
{
    if(iconUrl == url)
        return;

    iconUrl = url;
    emit iconUrlChanged();
}

void QHtspChannelData::setId(qint64 id)
{
    if(this->id == id)
        return;

    this->id = id;
    emit idChanged();
}

void QHtspChannelData::setName(QString name)
{
    this->name = name;
    emit nameChanged();
}

void QHtspChannelData::setNumber(qint64 number)
{
    if(this->number == number)
        return;

    this->number = number;
    emit numberChanged();
}

void QHtspChannelData::parseMessage(QHtspMessage &message)
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

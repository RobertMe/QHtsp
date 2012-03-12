#include "qhtsp.h"

#include <QCryptographicHash>

#include "qhtspevent.h"

QHtsp::QHtsp()
{
    m_connection = NULL;
}

QHtspChannelList *QHtsp::channels()
{
    return &m_channels;
}

QHtspEventList *QHtsp::events()
{
    return &m_events;
}

int QHtsp::htspVersion()
{
    return m_htspVersion;
}

QString QHtsp::serverName()
{
    return m_serverName;
}

QString QHtsp::serverVersion()
{
    return m_serverVersion;
}

QHtspTagList *QHtsp::tags()
{
    return &m_tags;
}

void QHtsp::addDvrEntry(qint64 eventId)
{
    QHtspMessage addEntry;
    addEntry.addString("method", "addDvrEntry");
    addEntry.addInt64("eventId", eventId);
    m_connection->sendMessage(addEntry);
}

void QHtsp::authenticate(QString username, QString password)
{
    QHtspMessage authenticate;
    QCryptographicHash digest(QCryptographicHash::Sha1);
    authenticate.addString("method", "authenticate");
    authenticate.addString("username", username);
    digest.addData(password.toAscii());
    digest.addData(m_challenge);
    authenticate.addBin("digest", digest.result());

    m_connection->sendMessage(authenticate);
}

void QHtsp::connectToServer(QString clientName, QString clientVersion, uint preferredHtspVersion, QString hostName, quint16 port)
{
    if(m_connection)
        delete m_connection;

    m_clientName = clientName;
    m_clientVersion = clientVersion;
    m_preferredHtspVersion = preferredHtspVersion;

    m_connection = new QHtspConnection(this);
    connect(m_connection, SIGNAL(connected()), this, SLOT(_connectionConnected()));
    connect(m_connection, SIGNAL(invoke(QString,QHtspMessage&)), this, SLOT(_invoke(QString,QHtspMessage&)));
    m_connection->setHostName(hostName);
    m_connection->setPort(port);
    m_connection->connectToServer();
}

void QHtsp::enableAsync()
{
    QHtspMessage enableAsync;
    enableAsync.addString("method", "enableAsyncMetadata");
    m_connection->sendMessage(enableAsync);
}

void QHtsp::getEvent(qint64 eventId)
{
    qint64 seq;
    QHtspMessage getEvent;
    getEvent.addString("method", "getEvent");
    getEvent.addInt64("eventId", eventId);
    seq = m_connection->sendMessage(getEvent, this, "_handleGetEvent");
    m_requestedEvents[seq] = eventId;
}

void QHtsp::getEvents(qint64 nextEventId, int numFollowing, QHtspEventList *eventList)
{
    qint64 seq;
    QHtspMessage getEvents;
    getEvents.addString("method", "getEvents");
    getEvents.addInt64("eventId", nextEventId);
    getEvents.addInt64("numFollowing", numFollowing);
    seq = m_connection->sendMessage(getEvents, this, "_handleGetEvents");
    m_requestedEventLists[seq] = eventList;
}

void QHtsp::queryEpg(QHtspEpgQuery *query)
{
    int seq;
    QHtspMessage epgQuery;
    epgQuery.addString("method", "epgQuery");
    epgQuery.addString("query", query->query());
    if(query->channel())
        epgQuery.addInt64("channelId", query->channel()->id());
    if(query->tag())
        epgQuery.addInt64("tagId", query->tag()->id());
    seq = m_connection->sendMessage(epgQuery, this, "_handleEpgQuery");
    m_epgQueries[seq] = query;
}

void QHtsp::_connectionConnected()
{
    QHtspMessage hello;
    hello.addString("method","hello");
    hello.addString("clientname", m_clientName);
    hello.addString("clientversion", m_clientVersion);
    hello.addInt64("htspversion", m_preferredHtspVersion);

    m_connection->sendMessage(hello, this, "_handleHello");
}

void QHtsp::_invoke(QString method, QHtspMessage &message)
{
    if(method == "initialSyncCompleted")
    {
        emit syncCompleted();
    }
    else if(method == "channelAdd")
    {
        m_channels.add(this, message);
    }
    else if(method == "channelUpdate")
    {
        QHtspChannel *channel = m_channels.find(message.getInt64("channelId"));
        if(channel)
            channel->update(message);
        else
            m_channels.add(this, message);
    }
    else if(method == "channelDelete")
    {
        m_channels.remove(message.getInt64("channelId"));
    }
    else if(method == "tagAdd")
    {
        m_tags.add(this, message);
    }
    else if(method == "tagUpdate")
    {
        QHtspTag *tag = m_tags.find(message.getInt64("tagId"));
        if(tag)
            tag->update(message);
        else
            m_tags.add(this, message);
    }
    else if(method == "tagDelete")
    {
        m_tags.remove(message.getInt64("tagId"));
    }
    else
    {
        qDebug() << "Unknown method " << method;
    }
}

void QHtsp::_handleEpgQuery(QHtspMessage &message)
{
    int seq, i;
    QHtspEpgQuery *query;
    QList<qint64> *eventIds;
    bool ok;

    seq = message.getInt64("seq");
    query = m_epgQueries[seq];
    if(!query)
        return;

    eventIds = message.getInt64List("eventIds", &ok);
    if(!ok)
        return;

    for(i = 0; i < eventIds->length(); i++)
    {
        QHtspEvent *event = events()->find(eventIds->at(i));
        if(!event)
        {
            event = new QHtspEvent(this, eventIds->at(i), this);
            events()->add(event);
            getEvent(event->id());
        }

        query->events()->add(event);
    }

    delete eventIds;
}

void QHtsp::_handleGetEvent(QHtspMessage &message)
{
    int seq = message.getInt64("seq");
    QHtspEvent *event = events()->find(m_requestedEvents[seq]);
    if(!event)
        return;

    event->update(message);
}

void QHtsp::_handleGetEvents(QHtspMessage &message)
{
    bool ok;
    QList<QHtspMessage*> *messages;
    QHtspEventList *eventList;

    eventList = m_requestedEventLists[message.getInt64("seq")];
    if(!eventList)
        return;

    messages = message.getMessageList("events", &ok);
    if(!ok)
        return;

    events()->beginGroupAdd();
    eventList->beginGroupAdd();

    while(messages->count())
    {
        QHtspMessage* message = messages->at(0);

        int eventId = message->getInt64("eventId");
        QHtspEvent *event = events()->find(eventId);
        if(event)
            event->update(*message);
        else
            event = new QHtspEvent(*message, this, events());

        events()->add(event);
        eventList->add(event);

        messages->removeFirst();
        delete message;
    }

    eventList->endGroupAdd();
    events()->endGroupAdd();

    delete messages;
}

void QHtsp::_handleHello(QHtspMessage &message)
{
    m_serverName = message.getString("servername");
    m_serverVersion = message.getString("serverversion");
    m_htspVersion = message.getInt64("htspversion");
    m_challenge = message.getBin("challenge");

    emit connected();
}

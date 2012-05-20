#include "qhtsp.h"

#include <QCryptographicHash>

#include "qhtspevent.h"

QHtsp::QHtsp(QObject *parent) :
    QObject(parent), m_connection(0), m_isSyncCompleted(false)
{
    _createLists();
}

QHtspChannelList *QHtsp::channels()
{
    return m_channels;
}

QHtspDvrEntryList *QHtsp::dvrEntries()
{
    return m_dvrEntries;
}

QHtspEventList *QHtsp::events()
{
    return m_events;
}

int QHtsp::htspVersion()
{
    return m_htspVersion;
}

bool QHtsp::isSyncCompleted()
{
    return m_isSyncCompleted;
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
    return m_tags;
}

void QHtsp::addDvrEntry(qint64 eventId)
{
    QHtspMessage addEntry;
    addEntry.addString("method", "addDvrEntry");
    addEntry.addInt64("eventId", eventId);
    m_connection->sendMessage(addEntry, this, "_handleAddDvrEntry");
}

void QHtsp::authenticate(QString username, QString password)
{
    m_username = username;
    m_password = password;

    QHtspMessage authenticate;
    QCryptographicHash digest(QCryptographicHash::Sha1);
    authenticate.addString("method", "authenticate");
    authenticate.addString("username", username);
    digest.addData(password.toAscii());
    digest.addData(m_challenge);
    authenticate.addBin("digest", digest.result());

    m_connection->sendMessage(authenticate);
}

void QHtsp::cancelDvrEntry(qint64 id)
{
    QHtspMessage cancelEntry;
    cancelEntry.addString("method", "cancelDvrEntry");
    cancelEntry.addInt64("id", id);
    m_connection->sendMessage(cancelEntry);
}

void QHtsp::connectToServer(QString clientName, QString clientVersion, uint preferredHtspVersion, QString hostName, quint16 port)
{
    if(m_connection)
        delete m_connection;

    m_clientName = clientName;
    m_clientVersion = clientVersion;
    m_preferredHtspVersion = preferredHtspVersion;
    m_hostName = hostName;
    m_port = port;

    m_connection = new QHtspConnection(this);
    connect(m_connection, SIGNAL(connected()), this, SLOT(_connectionConnected()));
    connect(m_connection, SIGNAL(connectionError(QAbstractSocket::SocketError)), this, SLOT(_connectionError(QAbstractSocket::SocketError)));
    connect(m_connection, SIGNAL(invoke(QString,QHtspMessage&)), this, SLOT(_invoke(QString,QHtspMessage&)));
    connect(m_connection, SIGNAL(accessDenied()), this, SIGNAL(accessDenied()));
    m_connection->setHostName(hostName);
    m_connection->setPort(port);
    m_connection->connectToServer();
}

void QHtsp::disconnectFromServer(bool clear)
{
    if(m_connection)
    {
        m_connection->disconnectFromServer();
        delete m_connection;
        m_connection = 0;
    }

    m_epgQueries.clear();
    m_requestedEvents.clear();
    m_requestedEventLists.clear();

    if(clear)
    {
        _clearLists();
    }
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

QHtspSubscription *QHtsp::subscribe(QHtspChannel *channel)
{
    QHtspSubscription *subscription = new QHtspSubscription(this);
    subscription->setAuthentication(m_username, m_password);
    subscription->setConnectionDetails(m_clientName, m_clientVersion, m_preferredHtspVersion, m_hostName, m_port);
    subscription->start(channel);
    return subscription;
}

void QHtsp::deleteDvrEntry(qint64 id)
{
    QHtspMessage deleteEntry;
    deleteEntry.addString("method", "deleteDvrEntry");
    deleteEntry.addInt64("id", id);
    m_connection->sendMessage(deleteEntry);
}

void QHtsp::_clearLists()
{
    delete m_tags;
    delete m_events;
    delete m_dvrEntries;
    delete m_channels;

    _createLists();
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

void QHtsp::_connectionError(QAbstractSocket::SocketError error)
{
    if(error == QAbstractSocket::RemoteHostClosedError)
        emit connectionLost();
    else if(!m_connection->isConnected())
        emit connectError();
}

void QHtsp::_createLists()
{
    m_channels = new QHtspChannelList(this);
    m_dvrEntries = new QHtspDvrEntryList(this);
    m_events = new QHtspEventList(0, this);
    m_tags = new QHtspTagList(this);
}

void QHtsp::_invoke(QString method, QHtspMessage &message)
{
    if(method == "initialSyncCompleted")
    {
        m_isSyncCompleted = true;
        emit syncCompleted();
    }
    else if(method == "channelAdd" || method == "channelUpdate")
    {
        QHtspChannel *channel = m_channels->find(message.getInt64("channelId"));
        if(channel)
            channel->update(message);
        else
            m_channels->add(this, message);
    }
    else if(method == "channelDelete")
    {
        m_channels->remove(message.getInt64("channelId"));
    }
    else if(method == "tagAdd" || method == "tagUpdate")
    {
        QHtspTag *tag = m_tags->find(message.getInt64("tagId"));
        if(tag)
            tag->update(message);
        else
            m_tags->add(this, message);
    }
    else if(method == "tagDelete")
    {
        m_tags->remove(message.getInt64("tagId"));
    }
    else if(method == "dvrEntryAdd" || method == "dvrEntryUpdate")
    {
        QHtspDvrEntry *dvrEntry = m_dvrEntries->find(message.getInt64("id"));
        if(dvrEntry)
            dvrEntry->update(message);
        else
            m_dvrEntries->add(this, message);
    }
    else if(method == "dvrEntryDelete")
    {
        m_dvrEntries->remove(message.getInt64("id"));
    }
    else
    {
        qDebug() << "Unknown method " << method;
    }
}

void QHtsp::_handleAddDvrEntry(QHtspMessage &message)
{
    int dvrEntryId;
    bool ok;
    QHtspDvrEntry *dvrEntry;

    dvrEntryId = message.getInt64("id", &ok);
    if(!ok)
        return;

    dvrEntry = dvrEntries()->find(dvrEntryId);
    if(!dvrEntry)
        return;

    emit dvrEntryAdded(dvrEntry);
}

void QHtsp::_handleEpgQuery(QHtspMessage &message)
{
    int seq, i;
    QHtspEpgQuery *query;
    QList<qint64> *eventIds;
    bool ok;

    seq = message.getInt64("seq");
    query = m_epgQueries.take(seq);
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
    QHtspEvent *event = events()->find(m_requestedEvents.take(seq));
    if(!event)
        return;

    event->update(message);
}

void QHtsp::_handleGetEvents(QHtspMessage &message)
{
    bool ok;
    QList<QHtspMessage*> *messages;
    QHtspEventList *eventList;

    eventList = m_requestedEventLists.take(message.getInt64("seq"));
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

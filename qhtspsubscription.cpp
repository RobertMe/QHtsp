#include "qhtspsubscription.h"

#include <QCryptographicHash>

QHtspSubscription::QHtspSubscription(QObject *parent) :
    QObject(parent)
{
    d = new QHtspSubscriptionData(this);
}

QString QHtspSubscription::adapter()
{
    return d->adapter;
}

QString QHtspSubscription::mux()
{
    return d->mux;
}

QString QHtspSubscription::network()
{
    return d->network;
}

QString QHtspSubscription::provider()
{
    return d->provider;
}

QString QHtspSubscription::service()
{
    return d->service;
}

void QHtspSubscription::start(QHtspChannel *channel)
{
    m_channel = channel;

    m_connection = new QHtspConnection(this);
    connect(m_connection, SIGNAL(connected()), this, SLOT(_connectionConnected()));
    connect(m_connection, SIGNAL(invoke(QString,QHtspMessage&)), this, SLOT(_invoke(QString,QHtspMessage&)));
    m_connection->setHostName(m_hostName);
    m_connection->setPort(m_port);
    m_connection->connectToServer();
}

void QHtspSubscription::setAuthentication(QString username, QString password)
{
    m_username = username;
    m_password = password;
}

void QHtspSubscription::setConnectionDetails(QString clientName, QString clientVersion, uint preferredHtspVersion, QString hostName, quint16 port)
{
    m_clientName = clientName;
    m_clientVersion = clientVersion;
    m_hostName = hostName;
    m_port = port;
    m_preferredHtspVersion = preferredHtspVersion;
}

void QHtspSubscription::_authenticate(QByteArray challenge)
{
    if(m_username.isEmpty() || m_password.isEmpty())
        return;

    QHtspMessage authenticate;
    QCryptographicHash digest(QCryptographicHash::Sha1);

    digest.addData(m_password.toAscii());
    digest.addData(challenge);

    authenticate.addString("method", "authenticate");
    authenticate.addString("username", m_username);
    authenticate.addBin("digest", digest.result());

    m_connection->sendMessage(authenticate);
}

void QHtspSubscription::_connectionConnected()
{
    QHtspMessage hello;
    hello.addString("method","hello");
    hello.addString("clientname", m_clientName);
    hello.addString("clientversion", m_clientVersion);
    hello.addInt64("htspversion", m_preferredHtspVersion);

    m_connection->sendMessage(hello, this, "_handleHello");
}

void QHtspSubscription::_invoke(QString method, QHtspMessage &message)
{
    if(method == "subscriptionStart")
    {
        bool ok;
        QHtspMessage *sourceinfo = message.getMessage("sourceinfo", &ok);
        if(ok)
        {
            d->parseSourceInfo(*sourceinfo);
            delete sourceinfo;
        }

        QList<QHtspMessage*> *streams = message.getMessageList("streams");
        for(int i = 0; i < streams->length(); i++)
        {
            QHtspMessage *message = streams->at(i);
            QHtspStream *stream = new QHtspStream(*message, this);
            m_streams.insert(message->getInt64("index"), stream);
            delete message;
        }
        delete streams;

        emit started();
    }
}

void QHtspSubscription::_handleHello(QHtspMessage &message)
{
    bool ok;
    QByteArray challenge = message.getBin("challenge", &ok);
    if(ok)
        _authenticate(challenge);

    _start();
}

void QHtspSubscription::_start()
{
    QHtspMessage subscribe;
    subscribe.addString("method", "subscribe");
    subscribe.addInt64("channelId", m_channel->id());
    subscribe.addInt64("subscriptionId", QHtspSubscription::m_id++);

    m_connection->sendMessage(subscribe);
}


quint64 QHtspSubscription::m_id = 0;

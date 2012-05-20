#include "qhtspsubscription.h"

QHtspSubscriptionData::QHtspSubscriptionData(QObject *parent) :
    QObject(parent)
{
}

QHtspSubscriptionData::QHtspSubscriptionData(QHtspSubscriptionData &other, QObject *parent) :
    QObject(parent), QSharedData(other), adapter(other.adapter), mux(other.mux),
    network(other.network), provider(other.provider), service(other.service)
{
}

void QHtspSubscriptionData::parseSourceInfo(QHtspMessage &message)
{
    QString adapter;
    QString mux;
    QString network;
    QString provider;
    QString service;
    bool ok;

    adapter = message.getString("adapter", &ok);
    if(ok)
        this->adapter = adapter;

    mux = message.getString("mux", &ok);
    if(ok)
        this->mux = mux;

    network = message.getString("network", &ok);
    if(ok)
        this->network = network;

    provider = message.getString("provider", &ok);
    if(ok)
        this->provider = provider;

    service = message.getString("service", &ok);
    if(ok)
        this->service = service;
}

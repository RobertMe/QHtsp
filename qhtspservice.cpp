#include "qhtspservice.h"

QHtspService::QHtspService(QHtspChannel *channel, QObject *parent) :
    QObject(parent)
{
    d = new QHtspServiceData(channel);
    _connectSignals();
}

QHtspService::QHtspService(QHtspMessage &message, QHtspChannel *channel, QObject *parent) :
    QObject(parent)
{
    d = new QHtspServiceData(channel);
    d->parseMessage(message);
    _connectSignals();
}

QHtspService::QHtspService(const QHtspService &service, QObject *parent) :
    QObject(parent)
{
    d = service.d;
    _connectSignals();
}

qint64 QHtspService::caid()
{
    return d->caid;
}

QString QHtspService::caname()
{
    return d->caname;
}

QString QHtspService::name()
{
    return d->name;
}

QHtspService::Type QHtspService::type()
{
    return (QHtspService::Type)d->type;
}

void QHtspService::setCaid(qint64 caid)
{
    d->setCaid(caid);
}

void QHtspService::setCaname(QString caname)
{
    return d->setCaname(caname);
}

void QHtspService::setName(QString name)
{
    return d->setName(name);
}

void QHtspService::setType(QHtspService::Type type)
{
    return d->setType(type);
}

void QHtspService::_connectSignals()
{
    connect(d.data(), SIGNAL(caidChanged()), this, SIGNAL(caidChanged()));
    connect(d.data(), SIGNAL(canameChanged()), this, SIGNAL(canameChanged()));
    connect(d.data(), SIGNAL(nameChanged()), this, SIGNAL(typeChanged()));
    connect(d.data(), SIGNAL(typeChanged()), this, SIGNAL(nameChanged()));
}

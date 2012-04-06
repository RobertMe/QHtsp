#include "qhtspchannel.h"

#include "qhtsp.h"

QHtspChannel::QHtspChannel(QHtsp *htsp, QObject *parent) :
    QObject(parent)
{
    d = new QHtspChannelData(this, htsp, parent);
    _connectSignals();
}

QHtspChannel::QHtspChannel(QHtspMessage &message, QHtsp *htsp, QObject *parent) :
    QObject(parent)
{
    d = new QHtspChannelData(this, htsp, parent);
    d->parseMessage(message);
    _connectSignals();

}

QHtspChannel::QHtspChannel(const QHtspChannel& channel, QObject *parent) :
    QObject(parent)
{
    d = channel.d;
    _connectSignals();
}

QHtspEvent *QHtspChannel::event()
{
    return d->event();
}

QHtspEventList *QHtspChannel::events()
{
    return d->events;
}

QHtspEventModel *QHtspChannel::eventsModel()
{
    return d->eventsModel();
}

qint64 QHtspChannel::eventId()
{
    return d->eventId;
}

QString QHtspChannel::iconUrl()
{
    return d->iconUrl;
}

qint64 QHtspChannel::id()
{
    return d->id;
}

QString QHtspChannel::name()
{
    return d->name;
}

qint64 QHtspChannel::number()
{
    return d->number;
}

void QHtspChannel::setEventId(qint64 eventId)
{
    d->setEventId(eventId);
}

void QHtspChannel::setIconUrl(QString url)
{
    d->setIconUrl(url);
}

void QHtspChannel::setId(qint64 id)
{
    d->setId(id);
}

void QHtspChannel::setName(QString name)
{
    d->setName(name);
}

void QHtspChannel::setNumber(qint64 number)
{
    d->setNumber(number);
}

void QHtspChannel::fetchNextEvents(int count)
{
    d->htsp->getEvents(events()->at(events()->count()-1)->nextEventId(), count - 1, events());
}

void QHtspChannel::update(QHtspMessage &message)
{
    d->parseMessage(message);
}

void QHtspChannel::_connectSignals()
{
    connect(d.data(), SIGNAL(eventIdChanged()), this, SIGNAL(eventIdChanged()));
    connect(d.data(), SIGNAL(iconUrlChanged()), this, SIGNAL(iconUrlChanged()));
    connect(d.data(), SIGNAL(idChanged()), this, SIGNAL(idChanged()));
    connect(d.data(), SIGNAL(nameChanged()), this, SIGNAL(nameChanged()));
    connect(d.data(), SIGNAL(numberChanged()), this, SIGNAL(numberChanged()));
}

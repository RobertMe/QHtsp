#include "qhtspevent.h"

#include "qhtsp.h"
#include "qhtspchannel.h"

QHtspEvent::QHtspEvent(QHtsp *htsp, qint64 id, QObject *parent) :
    QObject(parent)
{
    d = new QHtspEventData(htsp, id, parent);
    _connectSignals();
}

QHtspEvent::QHtspEvent(QHtspMessage &message, QHtsp *htsp, QObject *parent) :
    QObject(parent)
{
    d = new QHtspEventData(htsp, -1, parent);
    d->parseMessage(message);
    _connectSignals();
}

QHtspEvent::QHtspEvent(const QHtspEvent &event, QObject *parent) :
    QObject(parent)
{
    d = event.d;
}

qint64 QHtspEvent::id()
{
    return d->id;
}

QHtspChannel *QHtspEvent::channel()
{
    return d->channel();
}

qint64 QHtspEvent::channelId()
{
    return d->channelId;
}

QString QHtspEvent::description()
{
    return d->description;
}

qint64 QHtspEvent::nextEventId()
{
    return d->nextEventId;
}

QDateTime QHtspEvent::start()
{
    return d->start;
}

QDateTime QHtspEvent::stop()
{
    return d->stop;
}

QString QHtspEvent::title()
{
    return d->title;
}

void QHtspEvent::setId(qint64 id)
{
    d->setId(id);
}

void QHtspEvent::setChannelId(qint64 channelId)
{
    d->setChannelId(channelId);
}

void QHtspEvent::setDescription(QString description)
{
    d->setDescription(description);
}

void QHtspEvent::setNextEventId(qint64 nextEventId)
{
    d->setNextEventId(nextEventId);
}

void QHtspEvent::setStart(QDateTime start)
{
    d->setStart(start);
}

void QHtspEvent::setStop(QDateTime stop)
{
    d->setStop(stop);
}

void QHtspEvent::setTitle(QString title)
{
    d->setTitle(title);
}

bool QHtspEvent::record()
{
    if(!d->htsp)
        return false;

    d->htsp->addDvrEntry(id());
    return true;
}

void QHtspEvent::update(QHtspMessage &message)
{
    d->parseMessage(message);
}

void QHtspEvent::_connectSignals()
{
    connect(d.data(), SIGNAL(channelIdChanged()), this, SIGNAL(channelIdChanged()));
    connect(d.data(), SIGNAL(descriptionChanged()), this, SIGNAL(descriptionChanged()));
    connect(d.data(), SIGNAL(idChanged()), this, SIGNAL(idChanged()));
    connect(d.data(), SIGNAL(loaded()), this, SIGNAL(loaded()));
    connect(d.data(), SIGNAL(nextEventIdChanged()), this, SIGNAL(nextEventIdChanged()));
    connect(d.data(), SIGNAL(startChanged()), this, SIGNAL(startChanged()));
    connect(d.data(), SIGNAL(stopChanged()), this, SIGNAL(stopChanged()));
    connect(d.data(), SIGNAL(titleChanged()), this, SIGNAL(titleChanged()));
}

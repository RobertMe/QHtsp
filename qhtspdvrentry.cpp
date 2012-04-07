#include "qhtspdvrentry.h"

#include "qhtsp.h"

QHtspDvrEntry::QHtspDvrEntry(QHtsp *htsp, QObject *parent) :
    QObject(parent)
{
    d = new QHtspDvrEntryData(htsp);
    _connectSignals();
}

QHtspDvrEntry::QHtspDvrEntry(QHtspMessage &message, QHtsp *htsp, QObject *parent) :
    QObject(parent)
{
    d = new QHtspDvrEntryData(htsp);
    d->parseMessage(message);
    _connectSignals();
}

QHtspDvrEntry::QHtspDvrEntry(const QHtspDvrEntry &dvrEntry, QObject *parent) :
    QObject(parent)
{
    d = dvrEntry.d;
    _connectSignals();
}

QHtspChannel *QHtspDvrEntry::channel()
{
    return d->channel();
}

qint64 QHtspDvrEntry::channelId()
{
    return d->channelId;
}

QString QHtspDvrEntry::description()
{
    return d->description;
}

QString QHtspDvrEntry::error()
{
    return d->error;
}

qint64 QHtspDvrEntry::id()
{
    return d->id;
}

QHtspDvrEntry::State QHtspDvrEntry::state()
{
    return (QHtspDvrEntry::State)d->state;
}

QDateTime QHtspDvrEntry::start()
{
    return d->start;
}

QDateTime QHtspDvrEntry::stop()
{
    return d->stop;
}

QString QHtspDvrEntry::title()
{
    return d->title;
}

void QHtspDvrEntry::setChannelId(qint64 channelId)
{
    d->setChannelId(channelId);
}

void QHtspDvrEntry::setDescription(QString description)
{
    d->setDescription(description);
}

void QHtspDvrEntry::setError(QString error)
{
    d->setError(error);
}

void QHtspDvrEntry::setId(qint64 id)
{
    d->setId(id);
}

void QHtspDvrEntry::setState(State state)
{
    d->setState(state);
}

void QHtspDvrEntry::setStart(QDateTime start)
{
    d->setStart(start);
}

void QHtspDvrEntry::setStop(QDateTime stop)
{
    d->setStop(stop);
}

void QHtspDvrEntry::setTitle(QString title)
{
    d->setTitle(title);
}

void QHtspDvrEntry::cancel()
{
    d->htsp->cancelDvrEntry(id());
}

void QHtspDvrEntry::remove()
{
    d->htsp->deleteDvrEntry(id());
}

void QHtspDvrEntry::update(QHtspMessage &message)
{
    d->parseMessage(message);
}

void QHtspDvrEntry::_connectSignals()
{
    connect(d.data(), SIGNAL(channelChanged()), this, SIGNAL(channelChanged()));
    connect(d.data(), SIGNAL(descriptionChanged()), this, SIGNAL(descriptionChanged()));
    connect(d.data(), SIGNAL(errorChanged()), this, SIGNAL(errorChanged()));
    connect(d.data(), SIGNAL(idChanged()), this, SIGNAL(idChanged()));
    connect(d.data(), SIGNAL(startChanged()), this, SIGNAL(startChanged()));
    connect(d.data(), SIGNAL(stopChanged()), this, SIGNAL(stopChanged()));
    connect(d.data(), SIGNAL(titleChanged()), this, SIGNAL(titleChanged()));
}

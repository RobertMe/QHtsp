#include "qhtsptag.h"

#include "qhtsp.h"
#include "qhtspchannel.h"

QHtspTag::QHtspTag(QHtsp *htsp, QObject *parent) :
    QObject(parent), m_channelsModel(0)
{
    d = new QHtspTagData(htsp);
    _connectSignals();
}

QHtspTag::QHtspTag(QHtspMessage &message, QHtsp *htsp, QObject *parent) :
    QObject(parent), m_channelsModel(0)
{
    d = new QHtspTagData(htsp);
    d->parseMessage(message);
    _connectSignals();
}

QHtspTag::QHtspTag(const QHtspTag &tag, QObject *parent) :
    QObject(parent), m_channelsModel(0)
{
    d = tag.d;
}

QHtspChannelList *QHtspTag::channels()
{
    return d->channels;
}

QHtspChannelModel *QHtspTag::channelsModel()
{
    if(!m_channelsModel)
        m_channelsModel = new QHtspChannelModel(d->channels);

    return m_channelsModel;
}

QString QHtspTag::iconUrl()
{
    return d->iconUrl;
}

qint64 QHtspTag::id()
{
    return d->id;
}

QString QHtspTag::name()
{
    return d->name;
}

void QHtspTag::setIconUrl(QString url)
{
    if(d->iconUrl == url)
        return;

    d->iconUrl = url;
    emit iconUrlChanged();
}

void QHtspTag::setId(qint64 id)
{
    if(d->id == id)
        return;

    d->id = id;
    emit idChanged();
}

void QHtspTag::setName(QString name)
{
    if(d->name == name)
        return;

    d->name = name;
    emit nameChanged();
}

void QHtspTag::update(QHtspMessage &message)
{
    d->parseMessage(message);
}

void QHtspTag::_connectSignals()
{
    connect(d.data(), SIGNAL(iconUrlChanged()), this, SIGNAL(iconUrlChanged()));
    connect(d.data(), SIGNAL(idChanged()), this, SIGNAL(idChanged()));
    connect(d.data(), SIGNAL(nameChanged()), this, SIGNAL(nameChanged()));
}

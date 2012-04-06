#include "qhtsptag.h"

#include "qhtsp.h"

QHtspTagData::QHtspTagData(QHtsp *htsp, QObject *parent) :
    QObject(parent), channels(new QHtspChannelList(this)), htsp(htsp)
{
}

QHtspTagData::QHtspTagData(const QHtspTagData &other, QObject *parent) :
    QObject(parent), QSharedData(other), channels(new QHtspChannelList(other.channels)), htsp(other.htsp),
    iconUrl(other.iconUrl), id(other.id), name(other.name)
{
}

void QHtspTagData::setIconUrl(QString url)
{
    if(this->iconUrl == url)
        return;

    this->iconUrl = url;
    emit iconUrlChanged();
}

void QHtspTagData::setId(qint64 id)
{
    if(this->id == id)
        return;

    this->id = id;
    emit idChanged();
}

void QHtspTagData::setName(QString name)
{
    if(this->name == name)
        return;

    this->name = name;
    emit nameChanged();
}

void QHtspTagData::parseMessage(QHtspMessage &message)
{
    QString iconUrl;
    qint64 id;
    QString name;
    QList<qint64> *channelIds;
    bool ok;

    id = message.getInt64("tagId", &ok);
    if(ok)
        setId(id);

    name = message.getString("tagName", &ok);
    if(ok)
        setName(name);

    iconUrl = message.getString("tagIcon", &ok);
    if(ok)
        setIconUrl(iconUrl);

    channelIds = message.getInt64List("members", &ok);
    if(ok)
    {
        _updateChannels(channelIds);
        delete channelIds;
    }
}

void QHtspTagData::_updateChannels(QList<qint64> *ids)
{
    int i;
    for(i = 0; i < ids->length(); i++)
    {
        if(channels->find(ids->at(i)))
        {
            continue;
        }

        QHtspChannel *channel = htsp->channels()->find(ids->at(i));
        if(channel)
        {
            channels->add(channel);
        }
    }

    for(i = 0; i < channels->count();)
    {
        QHtspChannel *channel = channels->at(i);
        if(ids->contains(channel->id()))
        {
            i++;
        }
        else
        {
            channels->remove(channel);
        }
    }
}

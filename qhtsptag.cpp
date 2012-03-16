#include "qhtsptag.h"

#include "qhtsp.h"
#include "qhtspchannel.h"

QHtspTag::QHtspTag(QHtsp *htsp, QObject *parent) :
    QObject(parent), m_channelsModel(0), m_htsp(htsp), m_id(0)
{
    m_channels = new QHtspChannelList(this);
}

QHtspTag::QHtspTag(QHtspMessage &message, QHtsp *htsp, QObject *parent) :
    QObject(parent), m_channelsModel(0), m_htsp(htsp), m_id(0)
{
    m_channels = new QHtspChannelList(this);
    _parseMessage(message);
}

QHtspTag::QHtspTag(const QHtspTag &tag, QObject *parent) :
    QObject(parent), m_channelsModel(0)
{
    m_channels = tag.m_channels;
    m_htsp = tag.m_htsp;
    m_iconUrl = tag.m_iconUrl;
    m_id = tag.m_id;
    m_name = tag.m_name;
}

QHtspChannelList *QHtspTag::channels()
{
    return m_channels;
}

QHtspChannelModel *QHtspTag::channelsModel()
{
    if(!m_channelsModel)
        m_channelsModel = new QHtspChannelModel(channels());

    return m_channelsModel;
}

QString QHtspTag::iconUrl()
{
    return m_iconUrl;
}

qint64 QHtspTag::id()
{
    return m_id;
}

QString QHtspTag::name()
{
    return m_name;
}

void QHtspTag::setIconUrl(QString url)
{
    if(m_iconUrl == url)
        return;

    m_iconUrl = url;
    emit iconUrlChanged();
}

void QHtspTag::setId(qint64 id)
{
    if(m_id == id)
        return;

    m_id = id;
    emit idChanged();
}

void QHtspTag::setName(QString name)
{
    if(m_name == name)
        return;

    m_name = name;
    emit nameChanged();
}

void QHtspTag::update(QHtspMessage &message)
{
    _parseMessage(message);
}

void QHtspTag::_parseMessage(QHtspMessage &message)
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

void QHtspTag::_updateChannels(QList<qint64> *ids)
{
    int i;
    for(i = 0; i < ids->length(); i++)
    {
        if(channels()->find(ids->at(i)))
        {
            continue;
        }

        QHtspChannel *channel = m_htsp->channels()->find(ids->at(i));
        if(channel)
        {
            channels()->add(channel);
        }
    }

    for(i = 0; i < channels()->count();)
    {
        QHtspChannel *channel = channels()->at(i);
        if(ids->contains(channel->id()))
        {
            i++;
        }
        else
        {
            channels()->remove(channel);
        }
    }
}

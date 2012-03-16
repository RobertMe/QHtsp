#include "qhtspepgquery.h"

#include "qhtsp.h"

QHtspEpgQuery::QHtspEpgQuery(QHtsp *htsp, QObject *parent) :
    QObject(parent), m_channel(0), m_eventsModel(0), m_htsp(htsp), m_tag(0)
{
    m_events = new QHtspEventList(0, this);
}

QHtspEpgQuery::QHtspEpgQuery(const QHtspEpgQuery &epgQuery, QObject *parent) :
    QObject(parent), m_eventsModel(0)
{
    m_channel = epgQuery.m_channel;
    m_events = new QHtspEventList(m_channel, this);
    m_htsp = epgQuery.m_htsp;
    m_query = epgQuery.m_query;
    m_tag = epgQuery.m_tag;
}

QHtspChannel *QHtspEpgQuery::channel()
{
    return m_channel;
}

QHtspEventList *QHtspEpgQuery::events()
{
    return m_events;
}

QHtspEventModel *QHtspEpgQuery::eventsModel()
{
    if(!m_eventsModel)
        m_eventsModel = new QHtspEventModel(events());

    return m_eventsModel;
}

QString QHtspEpgQuery::query()
{
    return m_query;
}

QHtspTag *QHtspEpgQuery::tag()
{
    return m_tag;
}

void QHtspEpgQuery::setChannel(QHtspChannel *channel)
{
    m_channel = channel;
}

void QHtspEpgQuery::setQuery(QString query)
{
    m_query = query;
}

void QHtspEpgQuery::setTag(QHtspTag *tag)
{
    m_tag = tag;
}

void QHtspEpgQuery::run()
{
    m_htsp->queryEpg(this);
}

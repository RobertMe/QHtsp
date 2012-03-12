#include "qhtspepgquery.h"

#include "qhtsp.h"

QHtspEpgQuery::QHtspEpgQuery(QHtsp *htsp, QObject *parent) :
    QObject(parent), m_channel(0), m_eventsModel(&m_events), m_htsp(htsp), m_tag(0)
{
}

QHtspChannel *QHtspEpgQuery::channel()
{
    return m_channel;
}

QHtspEventList *QHtspEpgQuery::events()
{
    return &m_events;
}

QHtspEventModel *QHtspEpgQuery::eventsModel()
{
    return &m_eventsModel;
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

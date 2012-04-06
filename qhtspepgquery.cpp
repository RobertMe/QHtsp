#include "qhtspepgquery.h"

#include "qhtsp.h"

QHtspEpgQuery::QHtspEpgQuery(QHtsp *htsp, QObject */*parent*/)
    : m_eventsModel(0)
{
    d = new QHtspEpgQueryData(htsp);
}

QHtspEpgQuery::QHtspEpgQuery(const QHtspEpgQuery &epgQuery, QObject */*parent*/)
    : QObject(0), m_eventsModel(0)
{
    d = epgQuery.d;
}

QHtspChannel *QHtspEpgQuery::channel()
{
    return d->channel;
}

QHtspEventList *QHtspEpgQuery::events()
{
    return d->events;
}

QHtspEventModel *QHtspEpgQuery::eventsModel()
{
    if(!m_eventsModel)
        m_eventsModel = new QHtspEventModel(events());

    return m_eventsModel;
}

QString QHtspEpgQuery::query()
{
    return d->query;
}

QHtspTag *QHtspEpgQuery::tag()
{
    return d->tag;
}

void QHtspEpgQuery::setChannel(QHtspChannel *channel)
{
    d->channel = channel;
}

void QHtspEpgQuery::setQuery(QString query)
{
    d->query = query;
}

void QHtspEpgQuery::setTag(QHtspTag *tag)
{
    d->tag = tag;
}

void QHtspEpgQuery::run()
{
    d->htsp->queryEpg(this);
}

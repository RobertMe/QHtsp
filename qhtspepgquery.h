#ifndef QHTSPEPGQUERY_H
#define QHTSPEPGQUERY_H

#include <QObject>
#include <QString>

#include "qhtspchannel.h"
#include "qhtspeventlist.h"
#include "qhtspeventmodel.h"
#include "qhtsptag.h"

class QHtsp;

class QHtspEpgQuery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QHtspChannel *channel READ channel WRITE setChannel)
    Q_PROPERTY(QHtspEventList *events READ events)
    Q_PROPERTY(QHtspEventModel *eventsModel READ eventsModel)
    Q_PROPERTY(QString query READ query WRITE setQuery)
    Q_PROPERTY(QHtspTag *tag READ tag WRITE setTag)

public:
    explicit QHtspEpgQuery(QHtsp *htsp = 0, QObject *parent = 0);
    
    QHtspChannel *channel();
    QHtspEventList *events();
    QHtspEventModel *eventsModel();
    QString query();
    QHtspTag *tag();

    void setChannel(QHtspChannel *channel);
    void setQuery(QString query);
    void setTag(QHtspTag *tag);

    void run();

private:
    QHtspChannel *m_channel;
    QHtspEventList m_events;
    QHtspEventModel m_eventsModel;
    QHtsp *m_htsp;
    QString m_query;
    QHtspTag *m_tag;
};

#endif // QHTSPEPGQUERY_H

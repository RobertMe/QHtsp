#ifndef QHTSP_H
#define QHTSP_H

#include <QHash>
#include <QList>
#include <QObject>
#include <QString>

#include "qhtsp_global.h"
#include "qhtspchannellist.h"
#include "qhtspconnection.h"
#include "qhtspepgquery.h"
#include "qhtspeventlist.h"
#include "qhtspmessage.h"
#include "qhtsptaglist.h"

class QHTSPSHARED_EXPORT QHtsp : public QObject
{
    Q_OBJECT
public:
    QHtsp();

    QHtspChannelList *channels();
    QHtspEventList *events();
    int htspVersion();
    QString serverName();
    QString serverVersion();
    QHtspTagList *tags();

    void addDvrEntry(qint64 eventId);
    void authenticate(QString username, QString password);
    void connectToServer(QString clientName, QString clientVersion, uint preferredHtspVersion, QString hostName, quint16 port = 9982);
    void enableAsync();
    void getEvent(qint64 eventId);
    void getEvents(qint64 nextEventId, int numFollowing, QHtspEventList *eventList);
    void queryEpg(QHtspEpgQuery *query);

signals:
    void connected();
    void syncCompleted();

private:
    QHtspChannelList m_channels;
    QHtspConnection *m_connection;
    QHtspEventList m_events;
    QHtspTagList m_tags;

    QByteArray m_challenge;
    QString m_clientName;
    QString m_clientVersion;
    QHash<qint64, QHtspEpgQuery*> m_epgQueries;
    int m_htspVersion;
    uint m_preferredHtspVersion;
    QHash<qint64, qint64> m_requestedEvents;
    QHash<qint64, QHtspEventList*> m_requestedEventLists;
    QString m_serverName;
    QString m_serverVersion;

private slots:
    void _connectionConnected();
    void _invoke(QString method, QHtspMessage &message);

    void _handleEpgQuery(QHtspMessage &message);
    void _handleGetEvent(QHtspMessage &message);
    void _handleGetEvents(QHtspMessage &message);
    void _handleHello(QHtspMessage &message);
};

#endif // QHTSP_H

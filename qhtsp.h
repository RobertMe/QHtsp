#ifndef QHTSP_H
#define QHTSP_H

#include <QHash>
#include <QList>
#include <QObject>
#include <QString>

#include "qhtsp_global.h"
#include "qhtspchannellist.h"
#include "qhtspconnection.h"
#include "qhtspdvrentrylist.h"
#include "qhtspepgquery.h"
#include "qhtspeventlist.h"
#include "qhtspmessage.h"
#include "qhtspsubscription.h"
#include "qhtsptaglist.h"

class QHTSPSHARED_EXPORT QHtsp : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int htspVersion READ htspVersion NOTIFY connected)
    Q_PROPERTY(QString serverName READ serverName NOTIFY connected)
    Q_PROPERTY(QString serverVersion READ serverVersion NOTIFY connected)

public:
    QHtsp(QObject *parent = 0);

    QHtspChannelList *channels();
    QHtspDvrEntryList *dvrEntries();
    QHtspEventList *events();
    int htspVersion();
    bool isSyncCompleted();
    QString serverName();
    QString serverVersion();
    QHtspTagList *tags();

    void addDvrEntry(qint64 eventId);
    virtual void authenticate(QString username, QString password);
    void cancelDvrEntry(qint64 id);
    virtual void connectToServer(QString clientName, QString clientVersion, uint preferredHtspVersion, QString hostName, quint16 port = 9982);
    virtual void disconnectFromServer(bool clear = true);
    void deleteDvrEntry(qint64 id);
    virtual void enableAsync();
    void getEvent(qint64 eventId);
    void getEvents(qint64 nextEventId, int numFollowing, QHtspEventList *eventList);
    void queryEpg(QHtspEpgQuery *query);
    void subscribe(QHtspChannel *channel);

signals:
    void accessDenied();
    void connected();
    void connectError();
    void connectionLost();
    void dvrEntryAdded(QHtspDvrEntry *dvrEntry);
    void syncCompleted();

protected:
    QHtspConnection *m_connection;

    QString m_clientName;
    QString m_clientVersion;
    QString m_hostName;
    QString m_password;
    quint16 m_port;
    QString m_username;
    uint m_preferredHtspVersion;

private:
    QHtspChannelList *m_channels;
    QHtspDvrEntryList *m_dvrEntries;
    QHtspEventList *m_events;
    QHtspTagList *m_tags;

    QByteArray m_challenge;
    QHash<qint64, QHtspEpgQuery*> m_epgQueries;
    int m_htspVersion;
    bool m_isSyncCompleted;
    QHash<qint64, qint64> m_requestedEvents;
    QHash<qint64, QHtspEventList*> m_requestedEventLists;
    QString m_serverName;
    QString m_serverVersion;

private slots:
    void _clearLists();
    void _connectionConnected();
    void _connectionError(QAbstractSocket::SocketError error);
    void _createLists();
    void _invoke(QString method, QHtspMessage &message);

    void _handleAddDvrEntry(QHtspMessage &message);
    void _handleEpgQuery(QHtspMessage &message);
    void _handleGetEvent(QHtspMessage &message);
    void _handleGetEvents(QHtspMessage &message);
    void _handleHello(QHtspMessage &message);
};

#endif // QHTSP_H

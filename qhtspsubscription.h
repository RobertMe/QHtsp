#ifndef QHTSPSUBSCRIPTION_H
#define QHTSPSUBSCRIPTION_H

#include <QByteArray>
#include <QExplicitlySharedDataPointer>
#include <QObject>
#include <QSharedData>
#include <QString>

#include "qhtspchannel.h"
#include "qhtspconnection.h"
#include "qhtspmessage.h"

class QHtspSubscriptionData : public QObject, public QSharedData
{
    Q_OBJECT

public:
    QHtspSubscriptionData(QObject *parent = 0);
    QHtspSubscriptionData(QHtspSubscriptionData &other, QObject *parent);
    ~QHtspSubscriptionData() { }

    QString adapter;
    QString mux;
    QString network;
    QString provider;
    QString service;

    void parseSourceInfo(QHtspMessage &message);

signals:
    void sourceInfoChanged();
};

class QHtspSubscription : public QObject
{
    Q_OBJECT

public:
    explicit QHtspSubscription(QObject *parent = 0);

    QString adapter();
    QString mux();
    QString network();
    QString provider();
    QString service();

    void start(QHtspChannel *channel);
    void setAuthentication(QString username, QString password);
    void setConnectionDetails(QString clientName, QString clientVersion, uint preferredHtspVersion, QString hostName, quint16 port = 9982);

signals:
    void started();

protected:
    QHtspConnection *m_connection;
    
private:
    QString m_clientName;
    QString m_clientVersion;
    QString m_hostName;
    QString m_username;
    QString m_password;
    quint16 m_port;
    uint m_preferredHtspVersion;

    QHtspChannel *m_channel;

    static quint64 m_id;

    QExplicitlySharedDataPointer<QHtspSubscriptionData> d;

    void _authenticate(QByteArray challenge);
    void _start();

private slots:
    void _connectionConnected();
    void _invoke(QString method, QHtspMessage &message);
    void _handleHello(QHtspMessage &message);
};

#endif // QHTSPSUBSCRIPTION_H

#ifndef QHTSPSUBSCRIPTION_H
#define QHTSPSUBSCRIPTION_H

#include <QByteArray>
#include <QObject>
#include <QString>

#include "qhtspchannel.h"
#include "qhtspconnection.h"
#include "qhtspmessage.h"

class QHtspSubscription : public QObject
{
    Q_OBJECT

public:
    explicit QHtspSubscription(QObject *parent = 0);

    void start(QHtspChannel *channel);
    void setAuthentication(QString username, QString password);
    void setConnectionDetails(QString clientName, QString clientVersion, uint preferredHtspVersion, QString hostName, quint16 port = 9982);
    
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

    void _authenticate(QByteArray challenge);
    void _start();

private slots:
    void _connectionConnected();
    void _invoke(QString method, QHtspMessage &message);
    void _handleHello(QHtspMessage &message);
};

#endif // QHTSPSUBSCRIPTION_H

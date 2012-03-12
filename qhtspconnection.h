#ifndef QHTSPCONNECTION_H
#define QHTSPCONNECTION_H

#include <QByteArray>
#include <QHash>
#include <QObject>
#include <QString>
#include <QTcpSocket>

#include "qhtsp_global.h"
#include "qhtspmessage.h"

class QHtspConnectionCallback
{
public:
    QObject *receiver;
    QString member;
};

class QHTSPSHARED_EXPORT QHtspConnection : public QObject{
    Q_OBJECT

public:
    explicit QHtspConnection(QObject *parent = 0);
    ~QHtspConnection();

    QString hostName();
    quint16 port();

    void setHostName(QString hostName);
    void setHtspVersion(int version);
    void setPort(quint16 port);


    void connectToServer();
    int sendMessage(QHtspMessage &message);
    int sendMessage(QHtspMessage &message, QObject *receiver, QString member);

signals:
    void connected();
    void invoke(QString method, QHtspMessage &message);

private:
    QString m_hostName;
    quint16 m_port;

    QHash<quint64, QHtspConnectionCallback> m_callbacks;
    QByteArray *m_currentMessage;
    qint64 m_currentMessageRead;
    qint64 m_currentMessageTotal;
    quint64 m_seq;
    QTcpSocket *m_socket;

private slots:
    void _handleMessage(QByteArray payload);
    void _readMessage();
    void _socketConnected();
};

#endif // QHTSPCONNECTION_H

/*
 *  Copyright (C) 2012 Robert Meijers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    bool isConnected();
    quint16 port();

    void setHostName(QString hostName);
    void setHtspVersion(int version);
    void setPort(quint16 port);


    void connectToServer();
    void disconnectFromServer();
    int sendMessage(QHtspMessage &message);
    int sendMessage(QHtspMessage &message, QObject *receiver, QString member);

signals:
    void accessDenied();
    void connected();
    void connectionError(QAbstractSocket::SocketError error);
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
    void _error(QAbstractSocket::SocketError error);
    void _handleMessage(QByteArray payload);
    void _readMessage();
    void _socketConnected();
};

#endif // QHTSPCONNECTION_H

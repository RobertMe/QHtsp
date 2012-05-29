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

#include "qhtspconnection.h"


QHtspConnection::QHtspConnection(QObject *parent)
    : QObject(parent)
{
    m_seq = 1;
    setPort(9982);
    m_currentMessage = 0;
    m_currentMessageRead = 0;
    m_currentMessageTotal = 0;
    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(connected()), this, SLOT(_socketConnected()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(_readMessage()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(_error(QAbstractSocket::SocketError)));
}

QHtspConnection::~QHtspConnection()
{
    if(m_currentMessage)
        delete m_currentMessage;

    delete m_socket;
}

QString QHtspConnection::hostName()
{
    return m_hostName;
}

bool QHtspConnection::isConnected()
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

quint16 QHtspConnection::port()
{
    return m_port;
}

void QHtspConnection::setHostName(QString hostName)
{
    m_hostName = hostName;
}

void QHtspConnection::setPort(quint16 port)
{
    m_port = port;
}

void QHtspConnection::connectToServer()
{
    if(m_socket->isOpen())
        return;

    m_socket->connectToHost(hostName(), port());
}

void QHtspConnection::disconnectFromServer()
{
    if(!m_socket->isOpen())
        return;

    m_socket->disconnectFromHost();
}

int QHtspConnection::sendMessage(QHtspMessage &message)
{
    if(!this)
        return -1;

    quint64 seq = m_seq++;
    message.addInt64("seq", seq);
    QByteArray payload = message.getMessage();
    m_socket->write(payload);

    return seq;
}

int QHtspConnection::sendMessage(QHtspMessage &message, QObject *receiver, QString member)
{
    qint64 seq = sendMessage(message);

    if(seq < 0)
        return seq;

    QHtspConnectionCallback callback;
    callback.receiver = receiver;
    callback.member = member;
    m_callbacks[seq] = callback;

    return seq;
}

void QHtspConnection::_error(QAbstractSocket::SocketError error)
{
    emit connectionError(error);
}

void QHtspConnection::_handleMessage(QByteArray payload)
{
    quint64 seq;
    QHtspMessage message(payload);
    seq = message.getInt64("seq");
    if(seq)
    {
        qint64 noAccess = message.getInt64("noaccess");
        QHtspConnectionCallback callback;
        if(noAccess)
        {
            emit accessDenied();
        }
        else if((callback = m_callbacks.take(seq)).receiver)
        {
            QByteArray member = callback.member.toAscii();
            QMetaObject::invokeMethod(callback.receiver, member, Qt::DirectConnection, Q_ARG(QHtspMessage&, message));
        }
    }
    else
    {
        QString method = message.getString("method");
        if(method.length())
        {
            emit invoke(method, message);
        }
    }
}

void QHtspConnection::_readMessage()
{
    qint64 available;
    while((available = m_socket->bytesAvailable()))
    {
        if(m_currentMessageTotal == m_currentMessageRead)
        {
            quint32 length = 0;
            m_currentMessage = new QByteArray(m_socket->read(4));
            int i;

            for(i = 0; i < m_currentMessage->length(); i++)
            {
                length = length << 8;
                length |= static_cast<quint8>(m_currentMessage->at(i));
            }

            m_currentMessageTotal = length;
            m_currentMessageRead = 0;
            available -= 4;
        }

        int read = qMin(m_currentMessageTotal - m_currentMessageRead, available);
        m_currentMessage->append(m_socket->read(read));
        m_currentMessageRead += read;

        if(m_currentMessageTotal == m_currentMessageRead)
        {
            _handleMessage(*m_currentMessage);
            delete m_currentMessage;
            m_currentMessage = 0;
        }
    }
}

void QHtspConnection::_socketConnected()
{
    emit connected();
}

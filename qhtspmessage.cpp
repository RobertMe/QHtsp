#include "qhtspmessage.h"

#include <QDebug>

QHtspMessage::QHtspMessage()
{
}

QHtspMessage::QHtspMessage(const QByteArray message)
{
    parseMessageData(message);
}

void QHtspMessage::addBin(QString key, QByteArray value)
{
    QHtspMessageData data;
    data.type = 4;
    data.key = key;
    data.value = value;
    m_data.append(data);
}

void QHtspMessage::addInt64(QString key, qint64 value)
{
    QHtspMessageData data;
    data.type = 2;
    data.key = key;
    data.value = QByteArray();
    while(value != 0)
    {
        data.value.append(value);
        value >>= 8;
    }
    m_data.append(data);
}

void QHtspMessage::addString(QString key, QString value)
{
    QHtspMessageData data;
    data.type = 3;
    data.key = key;
    data.value = value.toUtf8();
    m_data.append(data);
}

QByteArray QHtspMessage::getBin(QString key, bool *ok)
{
    QHtspMessageData *data = _searchKey(key);

    if(ok)
        *ok = false;

    if(!data || data->type != 4)
        return QByteArray();

    if(ok)
        *ok = true;
    return data->value;
}

qint64 QHtspMessage::getInt64(QString key, bool *ok)
{
    QHtspMessageData *data = _searchKey(key);

    if(ok)
        *ok = false;

    if(!data || data->type != 2 || data->value.length() == 0)
        return 0;

    qint64 value = 0;
    int i;

    for(i = data->value.length() -1; i >= 0; i--)
    {
        value = value << 8;
        value |= static_cast<quint8>(data->value.at(i));
    }

    if(ok)
        *ok = true;

    return value;
}

QList<qint64> *QHtspMessage::getInt64List(QString key, bool *ok)
{
    QHtspMessageData *data = _searchKey(key);
    QList<qint64> *ints = new QList<qint64>();

    if(ok)
        *ok = false;

    if(!data || data->type != 5 || (data->value.count() > 1 && data->value.at(0) != 2))
        return 0;

    int length = data->value.count();
    int currentPos = 0;
    while(length != currentPos)
    {
        int intLength = _getLength(data->value.mid(currentPos + 2, 4));
        qint64 value = 0;

        int i;
        for(i = intLength - 1; i >= 0; i--)
        {
            value = value << 8;
            value |= static_cast<quint8>(data->value.at(currentPos + 2 + 4 + i));
        }
        ints ->append(value);

        currentPos += 2 + 4 + intLength;
    }

    if(ok)
        *ok = true;

    return ints;
}

QList<QHtspMessage*> *QHtspMessage::getMessageList(QString key, bool *ok)
{
    QHtspMessageData *data = _searchKey(key);

    if(ok)
        *ok = false;

    if(!data || data->type != 5 || (data->value.count() > 1 && data->value.at(0) != 1))
        return 0;

    QList<QHtspMessage*> *messages = new QList<QHtspMessage*>();
    int length = data->value.count();
    int currentPos = 0;
    while(length != currentPos)
    {
        int messageLength = _getLength(data->value.mid(currentPos + 2, 4));
        QHtspMessage *message = new QHtspMessage(data->value.mid(currentPos + 2, messageLength + 4));
        messages->append(message);

        currentPos += 2 + 4 + messageLength;
    }

    if(ok)
        *ok = true;

    return messages;
}

QString QHtspMessage::getString(QString key, bool *ok)
{
    QHtspMessageData *data = _searchKey(key);

    if(ok)
        *ok = false;

    if(!data || data->type != 3)
        return QString();

    if(ok)
        *ok = true;

    return QString::fromUtf8(data->value);
}

QByteArray QHtspMessage::getMessage()
{
    QByteArray message;
    int i;

    for(i = 0; i < m_data.size(); i++)
    {
        message.append(m_data[i].type);
        message.append(m_data[i].key.length());
        message.append(_getLength(m_data[i].value.length()));
        message.append(m_data[i].key);
        message.append(m_data[i].value);
    }

    return message.prepend(_getLength(message.length()));
}

bool QHtspMessage::parseMessageData(const QByteArray data)
{
    int currentPos = -1;
    int length;
    length = _getLength(data.mid(0, 4));
    currentPos = 4;

    if(data.length() != length + 4)
    {

        qDebug() << "Message is corrupt! Expected length is " << length << " + 4. Real length is " << data.length();
        return false;
    }

    while(currentPos+1 < data.length())
    {
        QHtspMessageData messageData;
        int keyLength;
        int valueLength;
        messageData.type = data.mid(currentPos++, 1)[0];

        keyLength = data.at(currentPos++);
        valueLength = _getLength(data.mid(currentPos, 4));
        currentPos += 4;

        messageData.key = QString(data.mid(currentPos, keyLength));
        currentPos += keyLength;

        messageData.value = data.mid(currentPos, valueLength);
        currentPos += valueLength;

        m_data.append(messageData);
    }

    return true;
}

QByteArray QHtspMessage::_getLength(quint32 length)
{
    return QByteArray().append(length).rightJustified(4, 0);
}

int QHtspMessage::_getLength(QByteArray bytes)
{
    int length = 0;
    int i;

    for(i = 0; i < bytes.length(); i++)
    {
        if(i != 0)
            length = length << 8;
        length |= static_cast<quint8>(bytes.at(i));
    }

    return length;
}

QHtspMessageData *QHtspMessage::_searchKey(QString key)
{
    int i;
    QHtspMessageData *data;

    for(i = 0; i < m_data.size(); i++)
    {
        data = &m_data[i];
        if(data->key == key)
            return data;
    }

    return NULL;
}

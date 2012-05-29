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

#ifndef QHTSPMESSAGE_H
#define QHTSPMESSAGE_H

#include <QByteArray>
#include <QList>
#include <QString>

class QHtspMessageData
{
public:
    quint8 type;
    QString key;
    QByteArray value;
};

class QHtspMessage
{
public:
    QHtspMessage();
    QHtspMessage(const QByteArray message);

    void addBin(QString key, QByteArray value);
    void addInt64(QString key, qint64 value);
    void addString(QString key, QString value);

    QByteArray getBin(QString key, bool *ok = 0);
    qint64 getInt64(QString key, bool *ok = 0);
    QList<qint64> *getInt64List(QString key, bool *ok = 0);
    QList<QHtspMessage*> *getMessageList(QString key, bool *ok = 0);
    QString getString(QString key, bool *ok = 0);

    QByteArray getMessage();
    bool parseMessageData(const QByteArray data);

private:
    QByteArray _getLength(quint32 length);
    int _getLength(QByteArray bytes);
    QHtspMessageData *_searchKey(QString key);

    QList<QHtspMessageData> m_data;
};

#endif // QHTSPMESSAGE_H

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

#ifndef QHTSPTAG_H
#define QHTSPTAG_H

#include <QExplicitlySharedDataPointer>
#include <QObject>
#include <QSharedData>
#include <QString>

#include "qhtspchannellist.h"
#include "qhtspchannelmodel.h"
#include "qhtspmessage.h"

class QHtsp;
class QHtspTagData : public QObject, public QSharedData
{
    Q_OBJECT

public:
    QHtspTagData(QHtsp *htsp);
    QHtspTagData(const QHtspTagData &other);
    ~QHtspTagData() { }

    QHtspChannelList *channels;
    QHtsp *htsp;
    QString iconUrl;
    qint64 id;
    QString name;

    void setIconUrl(QString url);
    void setId(qint64 id);
    void setName(QString name);

    void parseMessage(QHtspMessage &message);

signals:
    void iconUrlChanged();
    void idChanged();
    void nameChanged();

private:
    void _updateChannels(QList<qint64> *ids);
};

class QHtspTag : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QHtspChannelList *channels READ channels)
    Q_PROPERTY(QHtspChannelModel *channelsModel READ channelsModel)
    Q_PROPERTY(QString iconUrl READ iconUrl WRITE setIconUrl NOTIFY iconUrlChanged)
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit QHtspTag(QHtsp *htsp, QObject *parent = 0);
    QHtspTag(QHtspMessage &message, QHtsp *htsp, QObject *parent = 0);
    QHtspTag(const QHtspTag &tag, QObject *parent = 0);
    
    QHtspChannelList *channels();
    QHtspChannelModel *channelsModel();
    QString iconUrl();
    qint64 id();
    QString name();

    void setIconUrl(QString url);
    void setId(qint64 id);
    void setName(QString name);

    void update(QHtspMessage &message);

signals:
    void iconUrlChanged();
    void idChanged();
    void nameChanged();

private:
    QExplicitlySharedDataPointer<QHtspTagData> d;
    QHtspChannelModel *m_channelsModel;

    void _connectSignals();
};

#endif // QHTSPTAG_H

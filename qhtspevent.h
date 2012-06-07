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

#ifndef QHTSPEVENT_H
#define QHTSPEVENT_H

#include <QDateTime>
#include <QExplicitlySharedDataPointer>
#include <QMetaType>
#include <QObject>
#include <QSharedData>
#include <QString>

#include "qhtspmessage.h"

class QHtsp;
class QHtspChannel;
class QHtspEvent;

class QHtspEventData : public QObject, public QSharedData
{
    Q_OBJECT

public:
    QHtspEventData(QHtsp *htsp, int id = -1);
    QHtspEventData(const QHtspEventData &other);
    ~QHtspEventData() { }

    qint64 id;
    qint64 channelId;
    QString description;
    QHtsp *htsp;
    QString longDescription;
    qint64 nextEventId;
    QDateTime start;
    QDateTime stop;
    QString title;

    void setId(qint64 id);
    void setChannelId(qint64 channelId);
    void setDescription(QString description);
    void setLongDescription(QString description);
    void setNextEventId(qint64 nextEventId);
    void setStart(QDateTime start);
    void setStop(QDateTime stop);
    void setTitle(QString title);

    QHtspChannel *channel();
    QHtspEvent *nextEvent();
    QHtspEvent *previousEvent();

    void parseMessage(QHtspMessage &message);

signals:
    void idChanged();
    void channelIdChanged();
    void descriptionChanged();
    void longDescriptionChanged();
    void nextEventIdChanged();
    void previousEventChanged();
    void startChanged();
    void stopChanged();
    void titleChanged();

    void loaded();

private:
    QHtspChannel *m_channel;
    bool m_loaded;
    QHtspEvent *m_nextEvent;
    QHtspEvent *m_previousEvent;
    bool m_previousEventSearched;

private slots:
    void _previousEventDestroyed();
};

class QHtspEvent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QHtspChannel *channel READ channel)
    Q_PROPERTY(qint64 channelId READ channelId WRITE setChannelId NOTIFY channelIdChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString longDescription READ longDescription WRITE setLongDescription NOTIFY longDescriptionChanged)
    Q_PROPERTY(QHtspEvent *nextEvent READ nextEvent NOTIFY nextEventIdChanged)
    Q_PROPERTY(qint64 nextEventId READ nextEventId WRITE setNextEventId NOTIFY nextEventIdChanged)
    Q_PROPERTY(QHtspEvent *previousEvent READ previousEvent NOTIFY previousEventChanged)
    Q_PROPERTY(QDateTime start READ start WRITE setStart NOTIFY startChanged)
    Q_PROPERTY(QDateTime stop READ stop WRITE setStop NOTIFY stopChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

public:
    QHtspEvent(QHtsp *htsp, qint64 id = -1, QObject *parent = 0);
    QHtspEvent(QHtspMessage &message, QHtsp *htsp, QObject *parent = 0);
    QHtspEvent(const QHtspEvent &event, QObject *parent = 0);

    qint64 id();
    QHtspChannel *channel();
    qint64 channelId();
    QString description();
    QString longDescription();
    QHtspEvent *nextEvent();
    qint64 nextEventId();
    QHtspEvent *previousEvent();
    QDateTime start();
    QDateTime stop();
    QString title();

    void setId(qint64 id);
    void setChannelId(qint64 channelId);
    void setDescription(QString description);
    void setLongDescription(QString description);
    void setNextEventId(qint64 nextEventId);
    void setStart(QDateTime start);
    void setStop(QDateTime stop);
    void setTitle(QString title);

    Q_INVOKABLE bool record();
    void update(QHtspMessage &message);

signals:
    void idChanged();
    void channelIdChanged();
    void descriptionChanged();
    void longDescriptionChanged();
    void nextEventIdChanged();
    void previousEventChanged();
    void startChanged();
    void stopChanged();
    void titleChanged();

    void loaded();

private:
    QExplicitlySharedDataPointer<QHtspEventData> d;

    void _connectSignals();
};

#endif // QHTSPEVENT_H

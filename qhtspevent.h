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

class QHtspEventData : public QObject, public QSharedData
{
    Q_OBJECT

public:
    QHtspEventData(QHtsp *htsp, int id = -1, QObject *parent = 0);
    QHtspEventData(const QHtspEventData &other, QObject *parent = 0);
    ~QHtspEventData() { }

    qint64 id;
    qint64 channelId;
    QString description;
    QHtsp *htsp;
    qint64 nextEventId;
    QDateTime start;
    QDateTime stop;
    QString title;

    void setId(qint64 id);
    void setChannelId(qint64 channelId);
    void setDescription(QString description);
    void setNextEventId(qint64 nextEventId);
    void setStart(QDateTime start);
    void setStop(QDateTime stop);
    void setTitle(QString title);

    QHtspChannel *channel();

    void parseMessage(QHtspMessage &message);

signals:
    void idChanged();
    void channelIdChanged();
    void descriptionChanged();
    void nextEventIdChanged();
    void startChanged();
    void stopChanged();
    void titleChanged();

    void loaded();

private:
    QHtspChannel *m_channel;
    bool m_loaded;
};

class QHtspEvent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QHtspChannel *channel READ channel)
    Q_PROPERTY(qint64 channelId READ channelId WRITE setChannelId NOTIFY channelIdChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
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
    qint64 nextEventId();
    QDateTime start();
    QDateTime stop();
    QString title();

    void setId(qint64 id);
    void setChannelId(qint64 channelId);
    void setDescription(QString description);
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
    void nextEventIdChanged();
    void startChanged();
    void stopChanged();
    void titleChanged();

    void loaded();

private:
    QExplicitlySharedDataPointer<QHtspEventData> d;

    void _connectSignals();
};

#endif // QHTSPEVENT_H

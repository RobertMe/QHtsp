#ifndef QHTSPEVENT_H
#define QHTSPEVENT_H

#include <QMetaType>
#include <QObject>
#include <QDateTime>
#include <QString>

#include "qhtspmessage.h"

class QHtsp;
class QHtspChannel;

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
    QHtspEvent(QHtsp *htsp = 0, qint64 id = -1, QObject *parent = 0);
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
    void changed(QHtspEvent *event);
    void idChanged();
    void channelIdChanged();
    void descriptionChanged();
    void nextEventIdChanged();
    void startChanged();
    void stopChanged();
    void titleChanged();

    void loaded();

private:
    qint64 m_id;
    QHtspChannel *m_channel;
    qint64 m_channelId;
    QString m_description;
    QHtsp *m_htsp;
    qint64 m_nextEventId;
    QDateTime m_start;
    QDateTime m_stop;
    QString m_title;

    bool m_loaded;

    void _parseMessage(QHtspMessage &message);
    
};

#endif // QHTSPEVENT_H

#ifndef QHTSPDVRENTRY_H
#define QHTSPDVRENTRY_H

#include <QDateTime>
#include <QExplicitlySharedDataPointer>
#include <QObject>
#include <QSharedData>
#include <QString>

#include "qhtspchannel.h"
#include "qhtspmessage.h"

class QHtsp;

class QHtspDvrEntryData : public QObject, public QSharedData
{
    Q_OBJECT

public:
    explicit QHtspDvrEntryData(QHtsp *htsp);
    QHtspDvrEntryData(const QHtspDvrEntryData &other);
    ~QHtspDvrEntryData() { }

    qint64 channelId;
    QString description;
    QString error;
    QHtsp *htsp;
    qint64 id;
    quint16 state;
    QDateTime start;
    QDateTime stop;
    QString title;

    void setChannelId(qint64 channelId);
    void setDescription(QString description);
    void setError(QString error);
    void setId(qint64 id);
    void setState(quint16 state);
    void setStart(QDateTime start);
    void setStop(QDateTime stop);
    void setTitle(QString title);

    QHtspChannel *channel();

    void parseMessage(QHtspMessage &message);

signals:
    void channelChanged();
    void descriptionChanged();
    void errorChanged();
    void idChanged();
    void stateChanged();
    void startChanged();
    void stopChanged();
    void titleChanged();

private:
    QHtspChannel *m_channel;
};

class QHtspDvrEntry : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(QHtspChannel *channel READ channel NOTIFY channelChanged)
    Q_PROPERTY(qint64 channelId READ channelId WRITE setChannelId NOTIFY channelChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString error READ error WRITE setError NOTIFY errorChanged)
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QDateTime start READ start WRITE setStart NOTIFY startChanged)
    Q_PROPERTY(QDateTime stop READ stop WRITE setStop NOTIFY stopChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

public:
    enum State {
        Completed = 2,
        Invalid = 3,
        Missed = 4,
        Recording = 1,
        Scheduled = 0
    };

    explicit QHtspDvrEntry(QHtsp *htsp, QObject *parent = 0);
    QHtspDvrEntry(QHtspMessage &message, QHtsp *htsp, QObject *parent = 0);
    QHtspDvrEntry(const QHtspDvrEntry &dvrEntry, QObject *parent = 0);
    
    QHtspChannel *channel();
    qint64 channelId();
    QString description();
    QString error();
    qint64 id();
    State state();
    QDateTime start();
    QDateTime stop();
    QString title();

    void setChannelId(qint64 channelId);
    void setDescription(QString description);
    void setError(QString error);
    void setId(qint64 id);
    void setState(State state);
    void setStart(QDateTime start);
    void setStop(QDateTime stop);
    void setTitle(QString title);

    Q_INVOKABLE void cancel();
    Q_INVOKABLE void remove();
    void update(QHtspMessage &message);

signals:
    void channelChanged();
    void descriptionChanged();
    void errorChanged();
    void idChanged();
    void stateChanged();
    void startChanged();
    void stopChanged();
    void titleChanged();
    
private:
    QExplicitlySharedDataPointer<QHtspDvrEntryData> d;

    void _connectSignals();
};

#endif // QHTSPDVRENTRY_H

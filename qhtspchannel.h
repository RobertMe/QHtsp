#ifndef QHTSPCHANNEL_H
#define QHTSPCHANNEL_H

#include <QObject>
#include <QString>

#include "qhtspmessage.h"
#include "qhtspevent.h"
#include "qhtspeventlist.h"
#include "qhtspeventmodel.h"

class QHtsp;

class QHtspChannel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QHtspEvent *event READ event)
    Q_PROPERTY(qint64 eventId READ eventId WRITE setEventId NOTIFY eventIdChanged)
    Q_PROPERTY(QHtspEventModel *eventsModel READ eventsModel)
    Q_PROPERTY(QString iconUrl READ iconUrl WRITE setIconUrl NOTIFY iconUrlChanged)
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(qint64 number READ number WRITE setNumber NOTIFY numberChanged)

public:
    explicit QHtspChannel(QHtsp *htsp = 0, QObject *parent = 0);
    QHtspChannel(QHtspMessage &message, QHtsp *htsp = 0, QObject *parent = 0);
    QHtspChannel(const QHtspChannel &channel, QObject *parent = 0);

    QHtspEvent *event();
    QHtspEventList *events();
    QHtspEventModel *eventsModel();
    qint64 eventId();
    QString iconUrl();
    qint64 id();
    QString name();
    qint64 number();

    void setEventId(qint64 eventId);
    void setIconUrl(QString url);
    void setId(qint64 id);
    void setName(QString name);
    void setNumber(qint64 number);

    void fetchNextEvents(int count);
    void update(QHtspMessage &message);

signals:
    void eventIdChanged();
    void iconUrlChanged();
    void idChanged();
    void nameChanged();
    void numberChanged();

private:
    QHtspEvent *m_event;
    QHtspEventList m_events;
    QHtspEventModel m_eventModel;
    qint64 m_eventId;
    QHtsp *m_htsp;
    QString m_iconUrl;
    qint64 m_id;
    QString m_name;
    qint64 m_number;

    bool m_blockChanged;

    void _parseMessage(QHtspMessage &message);
    void _emitChanged();
};

#endif // QHTSPCHANNEL_H

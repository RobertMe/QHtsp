#ifndef QHTSPEVENTLIST_H
#define QHTSPEVENTLIST_H

#include <QObject>
#include <QList>

#include "qhtspevent.h"
#include "qhtspmessage.h"

class QHtspChannel;

class QHtspEventList : public QObject
{
    Q_OBJECT
public:
    explicit QHtspEventList(QHtspChannel *channel = 0, QObject *parent = 0);
    
    QHtspEvent *at(int i);
    QHtspChannel *channel();
    int count();
    int indexOf(QHtspEvent *event);
    QHtspEvent *find(qint64 id);

    void beginGroupAdd();
    void endGroupAdd();

signals:
    void added(QHtspEvent *event);
    void changed(QHtspEvent *event);
    void removing(QHtspEvent *event);
    void groupAddBegun();
    void groupAddEnded();

public slots:
    void add(QHtspEvent *event);
    QHtspEvent *add(QHtspMessage &message);
    bool remove(QHtspEvent *event);
    bool remove(qint64 id);

    void _remove(QObject *event);

private:
    QHtspChannel *m_channel;
    QList<QHtspEvent *> m_events;
};

#endif // QHTSPEVENTLIST_H

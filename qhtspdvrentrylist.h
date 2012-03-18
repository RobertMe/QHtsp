#ifndef QHTSPDVRENTRYLIST_H
#define QHTSPDVRENTRYLIST_H

#include <QObject>
#include <QList>

#include "qhtspdvrentry.h"
#include "qhtspmessage.h"

class QHtspDvrEntryList : public QObject
{
    Q_OBJECT
public:
    explicit QHtspDvrEntryList(QObject *parent = 0);

    QHtspDvrEntry *at(int i);
    int count();
    int indexOf(QHtspDvrEntry *dvrEntry);
    QHtspDvrEntry *find(qint64 id);

signals:
    void added(QHtspDvrEntry *channel);
    void changed(QHtspDvrEntry *changed);
    void removing(QHtspDvrEntry *channel);

public slots:
    void add(QHtspDvrEntry *dvrEntry);
    QHtspDvrEntry *add(QHtsp *htsp, QHtspMessage &message);
    bool remove(QHtspDvrEntry *dvrEntry);
    bool remove(qint64 id);

private:
    QList<QHtspDvrEntry *> m_dvrEntries;
};

#endif // QHTSPDVRENTRYLIST_H

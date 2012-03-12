#ifndef QHTSPCHANNELLIST_H
#define QHTSPCHANNELLIST_H

#include <QObject>
#include <QList>

#include "qhtspchannel.h"
#include "qhtspmessage.h"

class QHtspChannelList : public QObject
{
    Q_OBJECT
public:
    explicit QHtspChannelList(QObject *parent = 0);
    
    QHtspChannel *at(int i);
    int count();
    int indexOf(QHtspChannel *channel);
    QHtspChannel *find(qint64 id);

signals:
    void added(QHtspChannel *channel);
    void changed(QHtspChannel *changed);
    void removing(QHtspChannel *channel);
    
public slots:
    void add(QHtspChannel *channel);
    QHtspChannel *add(QHtsp *htsp, QHtspMessage &message);
    bool remove(QHtspChannel *channel);
    bool remove(qint64 id);

private:
    QList<QHtspChannel *> m_channels;
};

#endif // QHTSPCHANNELLIST_H

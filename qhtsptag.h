#ifndef QHTSPTAG_H
#define QHTSPTAG_H

#include <QObject>
#include <QString>

#include "qhtspchannellist.h"
#include "qhtspchannelmodel.h"
#include "qhtspmessage.h"

class QHtsp;

class QHtspTag : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QHtspChannelList *channels READ channels)
    Q_PROPERTY(QHtspChannelModel *channelsModel READ channelsModel)
    Q_PROPERTY(QString iconUrl READ iconUrl WRITE setIconUrl NOTIFY iconUrlChanged)
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit QHtspTag(QHtsp *htsp = 0, QObject *parent = 0);
    QHtspTag(QHtspMessage &message, QHtsp *htsp = 0, QObject *parent = 0);
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
    QHtspChannelList *m_channels;
    QHtspChannelModel *m_channelsModel;
    QHtsp *m_htsp;
    QString m_iconUrl;
    qint64 m_id;
    QString m_name;

    void _parseMessage(QHtspMessage &message);
    void _updateChannels(QList<qint64> *ids);
};

#endif // QHTSPTAG_H

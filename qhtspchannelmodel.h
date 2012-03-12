#ifndef QHTSPCHANNELMODEL_H
#define QHTSPCHANNELMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>

#include "qhtspchannel.h"
#include "qhtspchannellist.h"
#include "qhtspevent.h"

class QHtspChannelModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum ChannelRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        NumberRole,
        IconUrlRole,
        EventRole
    };

    explicit QHtspChannelModel(QHtspChannelList *channels = 0);
    
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Q_INVOKABLE QHtspChannel *getChannelById(int id);

private:
    QHtspChannelList *m_channels;

private slots:
    void _addRow(QHtspChannel *channel);
    void _updateId();
    void _updateName();
    void _updateNumber();
    void _updateIconUrl();
    void _updateEvent();
    void _updateEventId();
    void _removeRow(QHtspChannel *channel);
};

#endif // QHTSPCHANNELMODEL_H

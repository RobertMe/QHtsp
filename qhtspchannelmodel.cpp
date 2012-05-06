#include "qhtspchannelmodel.h"

QHtspChannelModel::QHtspChannelModel(QHtspChannelList *channels) :
    QAbstractTableModel(channels), m_channels(channels)
{
    connect(m_channels, SIGNAL(added(QHtspChannel*)), this, SLOT(_addRow(QHtspChannel*)));
    connect(m_channels, SIGNAL(removing(QHtspChannel*)), this, SLOT(_removeRow(QHtspChannel*)));

    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[IconUrlRole] = "icon";
    roles[EventRole] = "event";
    setRoleNames(roles);

    for(int i = 0; i < m_channels->count(); i++)
    {
        _connectSignals(m_channels->at(i));
    }
}

int QHtspChannelModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 5;
}

QVariant QHtspChannelModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    QHtspChannel *channel;

    if(index.row() < 0 || index.row() >= m_channels->count())
        return result;

    channel = m_channels->at(index.row());

    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 0:
            result = channel->id();
            break;
        case 1:
            result = channel->name();
            break;
        case 2:
            result = channel->number();
            break;
        case 3:
            result = channel->iconUrl();
            break;
        case 4:
            result = channel->event() ? channel->event()->title() : "";
            break;
        }
    }
    else if(role == IdRole)
    {
        result = channel->id();
    }
    else if(role == NameRole)
    {
        result = channel->name();
    }
    else if(role == NumberRole)
    {
        result = channel->number();
    }
    else if(role == IconUrlRole)
    {
        result = channel->iconUrl();
    }
    else if(role == EventRole)
    {
        result = channel->event() ? channel->event()->title() : "";
    }

    return result;
}

QVariant QHtspChannelModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch(section)
    {
    case 0:
        return QString("Id");
        break;
    case 1:
        return QString("Name");
        break;
    case 2:
        return QString("Number");
        break;
    case 3:
        return QString("Icon url");
        break;
    case 4:
        return QString("Event");
        break;
    }

    return QVariant();
}

int QHtspChannelModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_channels->count();
}

QHtspChannel *QHtspChannelModel::getChannelById(int id)
{
    return m_channels->find(id);
}

QHtspChannel *QHtspChannelModel::getChannelByIndex(int i)
{
    return m_channels->at(i);
}

void QHtspChannelModel::_addRow(QHtspChannel *channel)
{
    int rows = rowCount() - 1;
    beginInsertRows(QModelIndex(), rows, rows);
    endInsertRows();

    _connectSignals(channel);
}

void QHtspChannelModel::_connectSignals(QHtspChannel *channel)
{
    connect(channel, SIGNAL(idChanged()), this, SLOT(_updateId()));
    connect(channel, SIGNAL(nameChanged()), this, SLOT(_updateName()));
    connect(channel, SIGNAL(numberChanged()), this, SLOT(_updateNumber()));
    connect(channel, SIGNAL(iconUrlChanged()), this, SLOT(_updateIconUrl()));
    connect(channel, SIGNAL(eventIdChanged()), this, SLOT(_updateEventId()));
    if(channel->event())
        connect(channel->event(), SIGNAL(titleChanged()), this, SLOT(_updateEvent()));
}

void QHtspChannelModel::_updateId()
{
    int row = m_channels->indexOf((QHtspChannel*)QObject::sender());
    QModelIndex index = createIndex(row, 0);
    emit dataChanged(index, index);
}

void QHtspChannelModel::_updateName()
{
    int row = m_channels->indexOf((QHtspChannel*)QObject::sender());
    QModelIndex index = createIndex(row, 1);
    emit dataChanged(index, index);
}

void QHtspChannelModel::_updateNumber()
{
    int row = m_channels->indexOf((QHtspChannel*)QObject::sender());
    QModelIndex index = createIndex(row, 2);
    emit dataChanged(index, index);
}

void QHtspChannelModel::_updateIconUrl()
{
    int row = m_channels->indexOf((QHtspChannel*)QObject::sender());
    QModelIndex index = createIndex(row, 3);
    emit dataChanged(index, index);
}

void QHtspChannelModel::_updateEvent()
{
    QHtspChannel *channel = (QHtspChannel*)QObject::sender();
    int row = m_channels->indexOf(channel);
    QModelIndex index = createIndex(row, 4);
    emit dataChanged(index, index);
}

void QHtspChannelModel::_updateEventId()
{
    QHtspChannel *channel = (QHtspChannel*)QObject::sender();
    int row = m_channels->indexOf(channel);
    QModelIndex index = createIndex(row, 4);
    emit dataChanged(index, index);

    connect(channel->event(), SIGNAL(titleChanged()), this, SLOT(_updateEvent()));
}

void QHtspChannelModel::_removeRow(QHtspChannel *channel)
{
    disconnect(channel, SIGNAL(idChanged()), this, SLOT(_updateId()));
    disconnect(channel, SIGNAL(nameChanged()), this, SLOT(_updateName()));
    disconnect(channel, SIGNAL(numberChanged()), this, SLOT(_updateNumber()));
    disconnect(channel, SIGNAL(iconUrlChanged()), this, SLOT(_updateIconUrl()));
    disconnect(channel, SIGNAL(eventIdChanged()), this, SLOT(_updateEvent()));

    int row = m_channels->indexOf(channel);
    beginRemoveRows(QModelIndex(), row, row);
    endRemoveRows();
}

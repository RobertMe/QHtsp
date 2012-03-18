#include "qhtspdvrentrymodel.h"

QHtspDvrEntryModel::QHtspDvrEntryModel(QHtspDvrEntryList *dvrEnties) :
    QAbstractTableModel(dvrEnties), m_dvrEntries(dvrEnties)
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[ChannelRole] = "channel";
    roles[StateRole] = "state";
    roles[StartRole] = "start";
    roles[StopRole] = "stop";
    roles[TitleRole] = "title";
    setRoleNames(roles);
}

int QHtspDvrEntryModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 6;
}

QVariant QHtspDvrEntryModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if(index.row() < 0 || index.row() >= m_dvrEntries->count())
        return result;

    QHtspDvrEntry *dvrEntry = m_dvrEntries->at(index.row());

    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 0:
            result = dvrEntry->id();
            break;
        case 1:
            result = dvrEntry->channel() ? dvrEntry->channel()->name() : "";
            break;
        case 2:
            result = dvrEntry->state();
            break;
        case 3:
            result = dvrEntry->start();
            break;
        case 4:
            result = dvrEntry->stop();
            break;
        case 5:
            result = dvrEntry->title();
            break;
        }
    }
    else if(role == IdRole)
    {
        result = dvrEntry->id();
    }
    else if(role == ChannelRole)
    {
        result = dvrEntry->channel() ? dvrEntry->channel()->name() : "";
    }
    else if(role == StateRole)
    {
        result = dvrEntry->state();
    }
    else if(role == StartRole)
    {
        result = dvrEntry->start();
    }
    else if(role == StopRole)
    {
        result = dvrEntry->stop();
    }
    else if(role == TitleRole)
    {
        result = dvrEntry->title();
    }

    return result;
}

QVariant QHtspDvrEntryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch(section)
    {
    case 0:
        return QString("Id");
        break;
    case 1:
        return QString("Channel");
        break;
    case 2:
        return QString("State");
        break;
    case 3:
        return QString("Start");
        break;
    case 4:
        return QString("Stop");
        break;
    case 5:
        return QString("Title");
        break;
    }

    return QVariant();
}

int QHtspDvrEntryModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_dvrEntries->count();
}

QHtspDvrEntry *QHtspDvrEntryModel::getDvrEntryById(int id)
{
    return m_dvrEntries->find(id);
}

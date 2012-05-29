/*
 *  Copyright (C) 2012 Robert Meijers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "qhtspdvrentrymodel.h"

QHtspDvrEntryModel::QHtspDvrEntryModel(QHtspDvrEntryList *dvrEnties, QObject *parent) :
    QAbstractTableModel(parent ? parent : dvrEnties), m_dvrEntries(dvrEnties)
{
    connect(m_dvrEntries, SIGNAL(added(QHtspDvrEntry*)), this, SLOT(_addRow(QHtspDvrEntry*)));
    connect(m_dvrEntries, SIGNAL(removing(QHtspDvrEntry*)), this, SLOT(_removeRow(QHtspDvrEntry*)));

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

QHtspDvrEntry *QHtspDvrEntryModel::getDvrEntryByIndex(int index)
{
    return m_dvrEntries->at(index);
}

void QHtspDvrEntryModel::_addRow(QHtspDvrEntry *dvrEntry)
{
    int rows = rowCount() - 1;
    beginInsertRows(QModelIndex(), rows, rows);
    endInsertRows();

    connect(dvrEntry, SIGNAL(idChanged()), this, SLOT(_updateId()));
    connect(dvrEntry, SIGNAL(channelChanged()), this, SLOT(_updateChannel()));
    connect(dvrEntry, SIGNAL(stateChanged()), this, SLOT(_updateState()));
    connect(dvrEntry, SIGNAL(startChanged()), this, SLOT(_updateStart()));
    connect(dvrEntry, SIGNAL(stopChanged()), this, SLOT(_updateStop()));
    connect(dvrEntry, SIGNAL(titleChanged()), this, SLOT(_updateTitle()));
    if(dvrEntry->channel())
        connect(dvrEntry->channel(), SIGNAL(nameChanged()), this, SLOT(_updateChannel()));
}

void QHtspDvrEntryModel::_updateId()
{
    int row = m_dvrEntries->indexOf((QHtspDvrEntry*)QObject::sender());
    QModelIndex index = createIndex(row, 0);
    emit dataChanged(index, index);
}

void QHtspDvrEntryModel::_updateChannel()
{
    int row = m_dvrEntries->indexOf((QHtspDvrEntry*)QObject::sender());
    QModelIndex index = createIndex(row, 1);
    emit dataChanged(index, index);
}

void QHtspDvrEntryModel::_updateState()
{
    int row = m_dvrEntries->indexOf((QHtspDvrEntry*)QObject::sender());
    QModelIndex index = createIndex(row, 2);
    emit dataChanged(index, index);
}

void QHtspDvrEntryModel::_updateStart()
{
    int row = m_dvrEntries->indexOf((QHtspDvrEntry*)QObject::sender());
    QModelIndex index = createIndex(row, 3);
    emit dataChanged(index, index);
}

void QHtspDvrEntryModel::_updateStop()
{
    int row = m_dvrEntries->indexOf((QHtspDvrEntry*)QObject::sender());
    QModelIndex index = createIndex(row, 4);
    emit dataChanged(index, index);
}

void QHtspDvrEntryModel::_updateTitle()
{
    int row = m_dvrEntries->indexOf((QHtspDvrEntry*)QObject::sender());
    QModelIndex index = createIndex(row, 5);
    emit dataChanged(index, index);
}

void QHtspDvrEntryModel::_removeRow(QHtspDvrEntry *dvrEntry)
{
    disconnect(dvrEntry, SIGNAL(idChanged()), this, SLOT(_updateId()));
    disconnect(dvrEntry, SIGNAL(channelChanged()), this, SLOT(_updateChannel()));
    disconnect(dvrEntry, SIGNAL(stateChanged()), this, SLOT(_updateState()));
    disconnect(dvrEntry, SIGNAL(startChanged()), this, SLOT(_updateStart()));
    disconnect(dvrEntry, SIGNAL(stopChanged()), this, SLOT(_updateStop()));
    disconnect(dvrEntry, SIGNAL(titleChanged()), this, SLOT(_updateTitle()));
    if(dvrEntry->channel())
        disconnect(dvrEntry->channel(), SIGNAL(nameChanged()), this, SLOT(_updateChannel()));

    int row = m_dvrEntries->indexOf(dvrEntry);
    beginRemoveRows(QModelIndex(), row, row);
    endRemoveRows();
}

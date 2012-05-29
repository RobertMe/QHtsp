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

#ifndef QHTSPDVRENTRYMODEL_H
#define QHTSPDVRENTRYMODEL_H

#include <QAbstractTableModel>

#include "qhtspdvrentry.h"
#include "qhtspdvrentrylist.h"

class QHtspDvrEntryModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)
public:
    enum DvrEntryRoles {
        IdRole = Qt::UserRole + 1,
        ChannelRole,
        StateRole,
        StartRole,
        StopRole,
        TitleRole
    };

    explicit QHtspDvrEntryModel(QHtspDvrEntryList *dvrEnties, QObject *parent = 0);

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Q_INVOKABLE QHtspDvrEntry *getDvrEntryById(int id);
    Q_INVOKABLE QHtspDvrEntry *getDvrEntryByIndex(int index);
    
private:
    QHtspDvrEntryList *m_dvrEntries;

private slots:
    void _addRow(QHtspDvrEntry *dvrEntry);
    void _updateId();
    void _updateChannel();
    void _updateState();
    void _updateStart();
    void _updateStop();
    void _updateTitle();
    void _removeRow(QHtspDvrEntry *dvrEntry);
};

#endif // QHTSPDVRENTRYMODEL_H

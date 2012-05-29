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

#ifndef QHTSPTAGMODEL_H
#define QHTSPTAGMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>

#include "qhtsptag.h"
#include "qhtsptaglist.h"

class QHtspTagModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)

public:
    enum TagRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        IconRole
    };

    explicit QHtspTagModel(QHtspTagList *tags, QObject *parent = 0);

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Q_INVOKABLE QHtspTag *getTagById(qint64 id);
    Q_INVOKABLE QHtspTag *getTagByIndex(int i);

private:
    QHtspTagList *m_tags;

private slots:
    void _addRow(QHtspTag *tag);
    void _updateIcon();
    void _updateId();
    void _updateName();
    void _removeRow(QHtspTag *tag);
};

#endif // QHTSPTAGMODEL_H

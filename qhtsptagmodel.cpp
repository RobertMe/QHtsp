#include "qhtsptagmodel.h"

QHtspTagModel::QHtspTagModel(QHtspTagList *tags) :
    QAbstractTableModel(tags), m_tags(tags)
{
    connect(m_tags, SIGNAL(added(QHtspTag*)), this, SLOT(_addRow(QHtspTag*)));
    connect(m_tags, SIGNAL(removing(QHtspTag*)), this, SLOT(_removeRow(QHtspTag*)));

    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[IconRole] = "icon";
    setRoleNames(roles);
}

int QHtspTagModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 3;
}

QVariant QHtspTagModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    QHtspTag *tag = m_tags->at(index.row());

    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 0:
            result = tag->id();
            break;
        case 1:
            result = tag->name();
            break;
        case 2:
            result = tag->iconUrl();
        }
    }
    else if(role == IdRole)
    {
        result = tag->id();
    }
    else if(role == NameRole)
    {
        result = tag->name();
    }
    else if(role == IconRole)
    {
        result = tag->iconUrl();
    }

    return result;
}

QVariant QHtspTagModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return QString("Icon url");
    }

    return QVariant();
}

int QHtspTagModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_tags->count();
}

QHtspTag *QHtspTagModel::getTagById(qint64 id)
{
    return m_tags->find(id);
}

QHtspTag *QHtspTagModel::getTagByIndex(int i)
{
    return m_tags->at(i);
}

void QHtspTagModel::_addRow(QHtspTag *tag)
{
    int rows = rowCount() - 1;
    beginInsertRows(QModelIndex(), rows, rows);
    endInsertRows();
    connect(tag, SIGNAL(iconUrlChanged()), this, SLOT(_updateIcon()));
    connect(tag, SIGNAL(idChanged()), this, SLOT(_updateId()));
    connect(tag, SIGNAL(nameChanged()), this, SLOT(_updateName()));
}

void QHtspTagModel::_updateIcon()
{
    int row = m_tags->indexOf((QHtspTag*)QObject::sender());
    QModelIndex index = createIndex(row, 2);
    emit dataChanged(index, index);
}

void QHtspTagModel::_updateId()
{
    int row = m_tags->indexOf((QHtspTag*)QObject::sender());
    QModelIndex index = createIndex(row, 0);
    emit dataChanged(index, index);
}

void QHtspTagModel::_updateName()
{
    int row = m_tags->indexOf((QHtspTag*)QObject::sender());
    QModelIndex index = createIndex(row, 1);
    emit dataChanged(index, index);
}

void QHtspTagModel::_removeRow(QHtspTag *tag)
{
    disconnect(tag, SIGNAL(iconUrlChanged()), this, SLOT(_updateIcon()));
    disconnect(tag, SIGNAL(idChanged()), this, SLOT(_updateId()));
    disconnect(tag, SIGNAL(nameChanged()), this, SLOT(_updateName()));
    int row = m_tags->indexOf(tag);
    beginRemoveRows(QModelIndex(), row, row);
    endRemoveRows();
}

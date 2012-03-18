#ifndef QHTSPEVENTMODEL_H
#define QHTSPEVENTMODEL_H

#include <QAbstractTableModel>

#include "qhtspevent.h"
#include "qhtspeventlist.h"

class QHtspChannel;

class QHtspEventModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)
public:
    enum ChannelRoles {
        IdRole = Qt::UserRole + 1,
        DescriptionRole,
        StartRole,
        StopRole,
        TitleRole,
        ChannelRole
    };

    explicit QHtspEventModel(QHtspEventList *events);
    
    bool canFetchMore(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void fetchMore(const QModelIndex &parent);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Q_INVOKABLE QHtspEvent *getEventById(int id);

signals:
    
private:
    QHtspEventList *m_events;
    int m_fakeRowCount;

private slots:
    void _addRow(QHtspEvent *event);
    void _fetchMore();
    void _updateId();
    void _updateChannel();
    void _updateDescription();
    void _updateStart();
    void _updateStop();
    void _updateTitle();
    void _removeRow(QHtspEvent *event);
};

#endif // QHTSPEVENTMODEL_H

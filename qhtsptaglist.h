#ifndef QHTSPTAGLIST_H
#define QHTSPTAGLIST_H

#include <QObject>
#include <QList>

#include "qhtspmessage.h"
#include "qhtsptag.h"

class QHtspTagList : public QObject
{
    Q_OBJECT
public:
    explicit QHtspTagList(QObject *parent = 0);

    QHtspTag *at(int i);
    int count();
    int indexOf(QHtspTag *tag);
    QHtspTag *find(qint64 id);

signals:
    void added(QHtspTag *tag);
    void removing(QHtspTag *tag);

public slots:
    void add(QHtspTag *tag);
    QHtspTag *add(QHtsp *htsp, QHtspMessage &message);
    bool remove(QHtspTag *tag);
    bool remove(qint64 id);

private:
    QList<QHtspTag *> m_tags;
};

#endif // QHTSPTAGLIST_H

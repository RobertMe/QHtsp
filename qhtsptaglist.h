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

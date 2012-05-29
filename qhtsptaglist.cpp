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

#include "qhtsptaglist.h"

QHtspTagList::QHtspTagList(QObject *parent) :
    QObject(parent)
{
}

void QHtspTagList::add(QHtspTag *tag)
{
    m_tags.append(tag);
    emit added(tag);
}

QHtspTag *QHtspTagList::add(QHtsp *htsp, QHtspMessage &message)
{
    QHtspTag *tag = new QHtspTag(message, htsp, this);
    add(tag);
    return tag;
}

QHtspTag *QHtspTagList::at(int i)
{
    return m_tags.at(i);
}

int QHtspTagList::count()
{
    return m_tags.count();
}

int QHtspTagList::indexOf(QHtspTag *tag)
{
    return m_tags.indexOf(tag);
}

QHtspTag *QHtspTagList::find(qint64 id)
{
    int i;

    for(i = 0; i < m_tags.count(); i++)
    {
        if(m_tags[i]->id() == id)
            return m_tags[i];
    }

    return NULL;
}

bool QHtspTagList::remove(QHtspTag *tag)
{
    emit removing(tag);
    return m_tags.removeOne(tag);
}

bool QHtspTagList::remove(qint64 id)
{
    bool result;
    QHtspTag *tag = find(id);
    if(!tag)
        return false;


    result = remove(tag);
    delete tag;

    return result;
}

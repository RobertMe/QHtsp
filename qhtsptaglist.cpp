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

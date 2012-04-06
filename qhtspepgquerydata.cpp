#include "qhtspepgquery.h"

QHtspEpgQueryData::QHtspEpgQueryData(QHtsp *htsp) :
    channel(0), events(new QHtspEventList(0, this)), htsp(htsp), tag(0)
{
}

QHtspEpgQueryData::QHtspEpgQueryData(const QHtspEpgQueryData &other) :
    QObject(0), QSharedData(other), channel(other.channel), events(new QHtspEventList(0, this)),
    htsp(other.htsp), query(other.query), tag(other.tag)
{
}

#include "qhtspservice.h"

QHtspServiceData::QHtspServiceData(QHtspChannel *channel)
    : channel(channel)
{
}

QHtspServiceData::QHtspServiceData(const QHtspServiceData &other)
    : QObject(0), QSharedData(other), caid(other.caid), caname(other.caname), channel(other.channel), name(other.name), type(other.type)
{
}

void QHtspServiceData::setCaid(qint64 caid)
{
    if(this->caid == caid)
        return;

    this->caid = caid;
    emit caidChanged();
}

void QHtspServiceData::setCaname(QString caname)
{
    if(this->caname == caname)
        return;

    this->caname = caname;
    emit canameChanged();
}

void QHtspServiceData::setName(QString name)
{
    if(this->name == name)
        return;

    this->name = name;
    emit nameChanged();
}

void QHtspServiceData::setType(qint64 type)
{
    if(this->type == type)
        return;

    this->type = type;
    emit typeChanged();
}

void QHtspServiceData::parseMessage(QHtspMessage &message)
{
    qint64 caid;
    QString caname;
    QString name;
    QString type;
    bool ok;

    caid = message.getInt64("caid", &ok);
    if(ok)
        setCaid(caid);

    caname = message.getString("caname", &ok);
    if(ok)
        setCaname(caname);

    name = message.getString("name", &ok);
    if(ok)
        setName(name);

    type = message.getString("type", &ok);
    if(ok)
    {
        if(type == "Radio")
        {
            setType(QHtspService::RadioType);
        }
        else if(type == "SDTV" || type == "SDTV-AC")
        {
            setType(QHtspService::SdTvType);
        }
        else if(type == "HDTV" || type == "HDTV-AC")
        {
            setType(QHtspService::HdTvType);
        }
    }
}

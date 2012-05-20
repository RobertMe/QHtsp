#include "qhtspstream.h"

QHtspStream::QHtspStream(QObject *parent) :
    QObject(parent)
{
}

QHtspStream::QHtspStream(QHtspMessage &message, QObject *parent) :
    QObject(parent), m_height(-1), m_width(-1)
{
    parseMessage(message);
}

void QHtspStream::parseMessage(QHtspMessage &message)
{
    quint16 height;
    QString language;
    QString type;
    quint16 width;
    bool ok;

    height = message.getInt64("height", &ok);
    if(ok)
        m_height = height;

    language = message.getString("language", &ok);
    if(ok)
        m_language = language;

    type = message.getString("type", &ok);
    if(ok)
    {
        if(type == "AAC")
            m_type = AAC;
        else if(type == "AC3")
            m_type = AC3;
        else if(type == "CA")
            m_type = CA;
        else if(type == "DVBSUB")
            m_type = DVBSUB;
        else if(type == "EAC3")
            m_type = EAC3;
        else if(type == "H264")
            m_type = H264;
        else if(type == "MPEG2AUDIO")
            m_type = MPEG2AUDIO;
        else if(type == "MPEG2VIDEO")
            m_type = MPEG2VIDEO;
        else if(type == "MPEGTS")
            m_type = MPEGTS;
        else if(type == "PAT")
            m_type = PAT;
        else if(type == "PMT")
            m_type = PMT;
        else if(type == "TELETEXT")
            m_type = TELETEXT;
        else if(type == "TEXTSUB")
            m_type = TEXTSUB;
    }

    width = message.getInt64("width", &ok);
    if(ok)
        m_width = width;
}

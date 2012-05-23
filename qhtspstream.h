#ifndef QHTSPSTREAM_H
#define QHTSPSTREAM_H

#include <QObject>

#include "qhtspmessage.h"

class QHtspStream : public QObject
{
    Q_OBJECT
    Q_ENUMS(Type)

public:
    enum Type {
        AAC,
        AC3,
        CA,
        DVBSUB,
        EAC3,
        H264,
        MPEG2AUDIO,
        MPEG2VIDEO,
        MPEGTS,
        PAT,
        PMT,
        TELETEXT,
        TEXTSUB
    };

    explicit QHtspStream(QObject *parent = 0);
    QHtspStream(QHtspMessage &message, QObject *parent = 0);

    quint16 height();
    QString language();
    Type type();
    quint16 width();

    void parseInfo(QHtspMessage &message);

private:
    quint16 m_height;
    QString m_language;
    Type m_type;
    quint16 m_width;
};

#endif // QHTSPSTREAM_H

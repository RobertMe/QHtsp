#ifndef QHTSPSTREAM_H
#define QHTSPSTREAM_H

#include <QChar>
#include <QObject>
#include <QQueue>

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

    struct Frame {
        quint32 decodeTimestamp;
        quint32 duration;
        QByteArray payload;
        quint32 presentationTimestamp;
        QChar type;
    };

    explicit QHtspStream(QObject *parent = 0);
    QHtspStream(QHtspMessage &message, QObject *parent = 0);

    quint16 height();
    quint8 id();
    bool isOpen();
    QString language();
    Type type();
    quint16 width();

    void handleMux(QHtspMessage &message);
    void open(bool open = true);
    void parseInfo(QHtspMessage &message);
    bool readFrame(Frame *frame);

signals:
    void frameReceived(QHtspStream *stream);

private:
    quint16 m_height;
    quint8 m_id;
    QString m_language;
    Type m_type;
    quint16 m_width;

    bool m_isOpen;
    QQueue<Frame> m_frames;
};

#endif // QHTSPSTREAM_H

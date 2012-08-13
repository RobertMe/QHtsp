#ifndef QHTSPGSTSUBSCRIPTIONHELPER_H
#define QHTSPGSTSUBSCRIPTIONHELPER_H

#include <QObject>
#include <QHash>
#include "gst/gstelement.h"

#include "gsthtspsrc.h"
#include "qhtspsubscription.h"
#include "qhtspstream.h"

class QHtspGstSubscriptionHelper : public QObject
{
    Q_OBJECT
public:
    explicit QHtspGstSubscriptionHelper(QHtspSubscription *subscription);
    ~QHtspGstSubscriptionHelper();

    GstHtspSrc *gstHtspSrc();

    void start();

private:
    GstHtspSrc *m_gstHtspSrc;
    QHash<quint8, GstPad*> m_pads;
    QHtspSubscription *m_subscription;
    GstElement *m_pipeline;
    GstElement *m_audioDecoder;
    GstElement *m_videoDecoder;

    void _initAudioPad(GstPad *pad);
    void _initVideoPad(GstPad *pad);

private slots:
    void _initPads();
    void _pushData(QHtspStream *stream);

};

#endif // QHTSPGSTSUBSCRIPTIONHELPER_H

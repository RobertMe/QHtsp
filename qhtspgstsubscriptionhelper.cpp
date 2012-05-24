#include "qhtspgstsubscriptionhelper.h"

QHtspGstSubscriptionHelper::QHtspGstSubscriptionHelper(QHtspSubscription *subscription) :
    QObject(subscription)
{
    m_subscription = subscription;
    m_gstHtspSrc = GST_HTSP_SRC(gst_element_factory_make("htspsrc", "htsp"));
}

GstHtspSrc *QHtspGstSubscriptionHelper::gstHtspSrc()
{
    return m_gstHtspSrc;
}

void QHtspGstSubscriptionHelper::start()
{
    if(m_subscription->isStarted())
        _initPads();
    else
        connect(m_subscription, SIGNAL(started()), this, SLOT(_initPads()));
}

void QHtspGstSubscriptionHelper::_initPads()
{
    int i;
    GstElementClass *klass;
    klass = G_TYPE_INSTANCE_GET_CLASS(m_gstHtspSrc, GST_ELEMENT_CLASS, GstElementClass);

    for(i = 0; i < m_subscription->streams().count(); i++)
    {
        QHtspStream *stream = m_subscription->streams().at(i);
        GstCaps *caps = 0;
        GstPadTemplate *templ = NULL;
        QString type;
        switch(stream->type())
        {
        case QHtspStream::AAC:
            caps = gst_caps_new_simple("audio/mpeg",
                                       "mpegversion", G_TYPE_INT, 4,
                                       "framed", G_TYPE_BOOLEAN, TRUE, NULL);
            templ = gst_element_class_get_pad_template(klass, "audio_%02d");
            type = "audio";
            break;
        case QHtspStream::AC3:
            caps = gst_caps_new_simple("audio/x-ac3",
                                       "framed", G_TYPE_BOOLEAN, TRUE, NULL);
            type = "audio";
            templ = gst_element_class_get_pad_template(klass, "audio_%02d");
            break;
        case QHtspStream::MPEG2AUDIO:
            caps = gst_caps_new_simple("audio/mpeg",
                                       "mpegversion", G_TYPE_INT, 1, "layer", G_TYPE_INT, 2, NULL);
            type = "audio";
            templ = gst_element_class_get_pad_template(klass, "audio_%02d");
            break;
        default:
            break;
        }

        if(!caps)
            continue;

        GstPad *pad = gst_pad_new_from_template(templ, type.append("_%1").arg(i, -2, 10, QChar('0')).toAscii());
        gst_pad_use_fixed_caps(pad);
        gst_pad_set_caps(pad, caps);
        gst_pad_set_active(pad, TRUE);
        gst_element_add_pad(GST_ELEMENT(m_gstHtspSrc), pad);

        m_pads.insert(stream->id(), pad);
        connect(stream, SIGNAL(frameReceived(QHtspStream*)), this, SLOT(_pushData(QHtspStream*)));
        stream->open();
        emit padAdded(pad);
    }
}

void QHtspGstSubscriptionHelper::_pushData(QHtspStream *stream)
{
    QHtspStream::Frame frame;
    while(stream->readFrame(&frame))
    {
        int size = frame.payload.size();
        GstBuffer *buf = gst_buffer_new_and_alloc(size);
        guint8 *data = (guint8*)g_malloc(size);
        memcpy(data, frame.payload.data(), size);
        gst_buffer_set_data(buf, data, size);
        gst_pad_push(m_pads[stream->id()], buf);
    }
}

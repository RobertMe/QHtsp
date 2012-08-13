#include "qhtspgstsubscriptionhelper.h"

QHtspGstSubscriptionHelper::QHtspGstSubscriptionHelper(QHtspSubscription *subscription) :
    QObject(subscription), m_pipeline(0), m_audioDecoder(0), m_videoDecoder(0)
{
    m_subscription = subscription;
    m_gstHtspSrc = GST_HTSP_SRC(gst_element_factory_make("htspsrc", "htsp"));
}

QHtspGstSubscriptionHelper::~QHtspGstSubscriptionHelper()
{
    if(m_pipeline)
    {
        gst_element_set_state(m_pipeline, GST_STATE_NULL);
        gst_object_unref(m_pipeline);
    }
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

    m_pipeline = gst_pipeline_new("player");
    gst_bin_add(GST_BIN(m_pipeline), GST_ELEMENT(gstHtspSrc()));
}

void QHtspGstSubscriptionHelper::_initAudioPad(GstPad *pad)
{
    if(m_audioDecoder)
        return;
#if defined(MEEGO_EDITION_HARMATTAN)
    m_audioDecoder = gst_element_factory_make("nokiamp3dec", "audio-decoder");
#else
    m_audioDecoder = gst_element_factory_make("mad", "audio-decoder");
#endif
    GstElement *queue, *convertor, *sink;
    GstPad *sinkpad;

    queue = gst_element_factory_make("queue", "audio-queue");
    convertor = gst_element_factory_make("audioconvert", "convertor");
    sink = gst_element_factory_make("autoaudiosink", "audio-output");

    gst_bin_add_many(GST_BIN(m_pipeline),
                     queue, m_audioDecoder, convertor, sink, NULL);

    gst_element_link_many(queue, m_audioDecoder, convertor, sink, NULL);

    sinkpad = gst_element_get_static_pad(queue, "sink");
    gst_pad_link(pad, sinkpad);
    gst_object_unref(sinkpad);
}

void QHtspGstSubscriptionHelper::_initVideoPad(GstPad *pad)
{
    if(m_videoDecoder)
        return;

    m_videoDecoder = gst_element_factory_make("mpeg2dec", "video-decoder");

    GstElement *queue, *rate, *sink;
    GstPad *sinkpad;

    queue = gst_element_factory_make("queue", "video-queue");
    rate = gst_element_factory_make("videorate", "video-rate");
    sink = gst_element_factory_make("autovideosink", "video-output");

    gst_bin_add_many(GST_BIN(m_pipeline),
                     queue, m_videoDecoder, rate, sink, NULL);

    gst_element_link_many(queue, m_videoDecoder, rate, sink, NULL);

    sinkpad = gst_element_get_static_pad(queue, "sink");
    gst_pad_link(pad, sinkpad);
    gst_object_unref(sinkpad);
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
        case QHtspStream::MPEG2VIDEO:
            caps = gst_caps_new_simple("video/mpeg",
                                       "mpegversion", G_TYPE_INT, 1, "layer", G_TYPE_INT, 2, NULL);
            type = "video";
            templ = gst_element_class_get_pad_template(klass, "video_%02d");
            break;
        default:
            break;
        }

        if(!caps)
            continue;

        GstPad *pad = gst_pad_new_from_template(templ, QString(type).append("_%1").arg(i, -2, 10, QChar('0')).toAscii());
        gst_pad_use_fixed_caps(pad);
        gst_pad_set_caps(pad, caps);
        gst_pad_set_active(pad, TRUE);
        gst_element_add_pad(GST_ELEMENT(m_gstHtspSrc), pad);

        m_pads.insert(stream->id(), pad);
        connect(stream, SIGNAL(frameReceived(QHtspStream*)), this, SLOT(_pushData(QHtspStream*)));
        stream->open();

        if(type == "audio")
            _initAudioPad(pad);
        else if(type == "video")
            _initVideoPad(pad);
    }

    gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
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

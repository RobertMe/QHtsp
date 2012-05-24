#include "gsthtspsrc.h"

GST_DEBUG_CATEGORY_STATIC(htspsrc_debug);

GST_BOILERPLATE(GstHtspSrc, gst_htsp_src, GstElement, GST_TYPE_ELEMENT)

#if GST_VERSION_MAJOR == 0 && GST_VERSION_MINOR <= 10 && GST_VERSION_MICRO < 35
void
gst_element_class_add_static_pad_template (GstElementClass * klass,
    GstStaticPadTemplate * templ)
{
  GstPadTemplate *pt;

  g_return_if_fail (GST_IS_ELEMENT_CLASS (klass));

  pt = gst_static_pad_template_get (templ);
  gst_element_class_add_pad_template (klass, pt);
  gst_object_unref (pt);
}
#endif

static GstStaticPadTemplate audio_src_templ =
        GST_STATIC_PAD_TEMPLATE("audio_%02d",
                                GST_PAD_SRC,
                                GST_PAD_SOMETIMES,
                                GST_STATIC_CAPS("audio/"));

static GstStaticPadTemplate video_src_templ =
        GST_STATIC_PAD_TEMPLATE("video_%02d",
                                GST_PAD_SRC,
                                GST_PAD_SOMETIMES,
                                GST_STATIC_CAPS("video/"));

static void gst_htsp_src_class_init(GstHtspSrcClass *klass)
{
    GObjectClass *gobject_class;
    GstElementClass *gstelement_class;
    GstHtspSrcClass *gsthtsp_src_class;

    GST_DEBUG_CATEGORY_INIT(htspsrc_debug, "htspsrc", 0, "Htsp source");

    gobject_class = G_OBJECT_CLASS(klass);
    gstelement_class = GST_ELEMENT_CLASS(klass);
    gsthtsp_src_class = GST_HTSP_SRC_CLASS(klass);
}

static void gst_htsp_src_init(GstHtspSrc *htspsrc, GstHtspSrcClass *klass)
{
    GstPad *pad = gst_pad_new_from_template(gst_element_class_get_pad_template(klass, "audio_%02d"), "audio");

    gst_pad_set_active(pad, TRUE);
    gst_element_add_pad(GST_ELEMENT(htspsrc), pad);
}

static void gst_htsp_src_base_init(gpointer klass)
{
    GstElementClass *element_class = GST_ELEMENT_CLASS (klass);

    gst_element_class_add_static_pad_template(element_class, &audio_src_templ);
    gst_element_class_add_static_pad_template(element_class, &video_src_templ);

    gst_element_class_set_details_simple(element_class, "Htsp srouce",
                                         "Source",
                                         "Source for multiple Htsp streams",
                                         "Robert Meijers <robert@r-meijers.nl>");
}

static gboolean
register_elements (GstPlugin *plugin)
{
    if(!gst_element_register (plugin, "htspsrc",
                               GST_RANK_PRIMARY, gst_htsp_src_get_type()))
    {
        return FALSE;
    }
    return TRUE;
}
#define PACKAGE "QHtsp"
GST_PLUGIN_DEFINE_STATIC (
  GST_VERSION_MAJOR,
  GST_VERSION_MINOR,
  "htsp-plugin",
  "Htsp element for QHtsp",
  register_elements,
  "0.1",
  "LGPL",
  "QHtsp",
  "http://meetv.r-meijers.nl"
)

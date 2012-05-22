#ifndef GST_HTSP_SRC_H
#define GST_HTSP_SRC_H

#include <gst/gst.h>
#include <gst/gstelement.h>

G_BEGIN_DECLS


#define GST_TYPE_HTSP_SRC \
    (gst_htsp_src_get_type())
#define GST_HTSP_SRC(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_HTSP_SRC,GstHtspSrc))
#define GST_HTSP_SRC_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_HTSP_SRC,GstHtspSrcClass))
#define GST_IS_HTSP_SRC(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_HTSP_SRC))
#define GST_IS_HTSP_SRC_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_HTSP_SRC))

typedef struct _GstHtspSrc GstHtspSrc;
typedef struct _GstHtspSrcClass GstHtspSrcClass;

struct _GstHtspSrc {
    GstElement element;
};

struct _GstHtspSrcClass {
    GstElementClass parent_class;
};

GType gst_htsp_src_get_type(void);

G_END_DECLS

#endif /* GST_HTSP_SRC_H */

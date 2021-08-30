/*
* Copyright (C) 2020 – 2021 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software
* is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
* KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
* EVENT SHALL XILINX BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
* OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE. Except as contained in this notice, the name of the Xilinx shall
* not be used in advertising or otherwise to promote the sale, use or other
* dealings in this Software without prior written authorization from Xilinx.
*/

#ifndef __GST_IVAS_XMETAAFFIXER_H__
#define __GST_IVAS_XMETAAFFIXER_H__

#include <gst/gst.h>
#include <gst/base/gstcollectpads.h>
#include <gst/video/video.h>
#include <gst/base/gstflowcombiner.h>
#include <gst/ivas/gstivasmeta.h>

G_BEGIN_DECLS
/* #defines don't like whitespacey bits */
#define GST_TYPE_IVAS_XMETAAFFIXER \
  (gst_ivas_xmetaaffixer_get_type())
#define GST_IVAS_XMETAAFFIXER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_IVAS_XMETAAFFIXER,GstIvas_XMetaAffixer))
#define GST_IVAS_XMETAAFFIXER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_IVAS_XMETAAFFIXER,GstIvas_XMetaAffixerClass))
#define GST_IS_IVAS_XMETAAFFIXER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_IVAS_XMETAAFFIXER))
#define GST_IS_IVAS_XMETAAFFIXER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_IVAS_XMETAAFFIXER))
#define GST_IVAS_XMETAAFFIXER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_IVAS_XMETAAFFIXER, GstIvas_XMetaAffixerClass))
#define GST_TYPE_IVAS_XMETAAFFIXER_PAD (gst_ivas_xmetaaffixer_pad_get_type())
#define GST_IVAS_XMETAAFFIXER_PAD(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_IVAS_XMETAAFFIXER_PAD, GstIvas_XMetaAffixerPad))
#define GST_IVAS_XMETAAFFIXER_PAD_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_VIDEO_MIXER_PAD, GstIvas_XMetaAffixerPadClass))
#define GST_IS_IVAS_XMETAAFFIXER_PAD(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_IVAS_XMETAAFFIXER_PAD))
#define GST_IS_IVAS_XMETAAFFIXER_PAD_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_IVAS_XMETAAFFIXER_PAD))
typedef struct _GstIvas_XMetaAffixer GstIvas_XMetaAffixer;
typedef struct _GstIvas_XMetaAffixerClass GstIvas_XMetaAffixerClass;
typedef struct _GstIvas_XMetaAffixerPad GstIvas_XMetaAffixerPad;
typedef struct _GstIvas_XMetaAffixerPadClass GstIvas_XMetaAffixerPadClass;
typedef struct _GstIvas_XMetaAffixerCollectData GstIvas_XMetaAffixerCollectData;

typedef enum {
  IVAS_XMETAAFFIXER_STATE_FIRST_BUFFER,
  IVAS_XMETAAFFIXER_STATE_DROP_BUFFER,
  IVAS_XMETAAFFIXER_STATE_PROCESS_BUFFER,
} IVAS_XMETAAFFIXER_STREAM_STATE;

struct _GstIvas_XMetaAffixerCollectData
{
  GstCollectData collectdata;   /* we extend the CollectData */
  GstIvas_XMetaAffixerPad *sinkpad;
};

struct _GstIvas_XMetaAffixerPad
{
  GstPad parent;
  GstIvas_XMetaAffixerCollectData *collect;
  GstPad *srcpad;
  guint width;
  guint height;
  GstClockTime duration;
  GstClockTime curr_pts;
  gboolean eos_received;
  GstVideoInfo vinfo;
  GstFlowReturn fret;
  gboolean sent_eos;
  IVAS_XMETAAFFIXER_STREAM_STATE stream_state;
};

#define MAX_SLAVE_SOURCES 16

struct _GstIvas_XMetaAffixer
{
  GstElement element;
  GstCollectPads *collect;
  GstIvas_XMetaAffixerPad *sink_master;
  GstIvas_XMetaAffixerPad *sink_slave[MAX_SLAVE_SOURCES];
  GstFlowCombiner *flowcombiner;
  guint num_slaves;
  gboolean sync;
  GstClockTime prev_m_end_ts;
  GstBuffer *prev_meta_buf; /* buffer holds metadata only but not data */
};

struct _GstIvas_XMetaAffixerPadClass
{
  GstPadClass parent_class;
};

struct _GstIvas_XMetaAffixerClass
{
  GstElementClass parent_class;
};

GType gst_ivas_xmetaaffixer_get_type (void);
GType gst_ivas_xmetaaffixer_pad_get_type (void);

G_END_DECLS
#endif /* __GST_IVAS_XMETAAFFIXER_H__ */

/*
* Copyright (C) 2020 - 2021 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software
* is furnished to do so, subject to the following conditions:
*
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

#ifndef __GST_IVAS_BUFFER_POOL_H__
#define __GST_IVAS_BUFFER_POOL_H__

#include <gst/gst.h>
#include <gst/video/video.h>

G_BEGIN_DECLS

typedef struct _GstIvasBufferPool GstIvasBufferPool;
typedef struct _GstIvasBufferPoolClass GstIvasBufferPoolClass;
typedef struct _GstIvasBufferPoolPrivate GstIvasBufferPoolPrivate;

#define GST_TYPE_IVAS_BUFFER_POOL \
  (gst_ivas_buffer_pool_get_type())
#define GST_IS_IVAS_BUFFER_POOL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_IVAS_BUFFER_POOL))
#define GST_IVAS_BUFFER_POOL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_IVAS_BUFFER_POOL, GstIvasBufferPool))
#define GST_IVAS_BUFFER_POOL_CAST(obj) \
  ((GstIvasBufferPool*)(obj))

typedef void (*ReleaseBufferCallback)(GstBuffer *buf, gpointer user_data);

struct _GstIvasBufferPool
{
  GstVideoBufferPool parent;
  GstIvasBufferPoolPrivate *priv;
  ReleaseBufferCallback release_buf_cb;
  gpointer cb_user_data; /* release callback user data */
};

struct _GstIvasBufferPoolClass
{
  GstVideoBufferPoolClass parent_class;
};

GST_EXPORT
GType gst_ivas_buffer_pool_get_type (void) G_GNUC_CONST;

GST_EXPORT
GstBufferPool *gst_ivas_buffer_pool_new (guint stride_align, guint elevation_align);

GST_EXPORT
void gst_ivas_buffer_pool_set_release_buffer_cb (GstIvasBufferPool *xpool, ReleaseBufferCallback release_buf_cb, gpointer user_data);

G_END_DECLS

#endif /* __GST_IVAS_BUFFER_POOL_H__ */

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/video/gstvideometa.h>
#include <gst/ivas/gstivasallocator.h>
#include "gstivasbufferpool.h"

GST_DEBUG_CATEGORY_STATIC (gst_ivas_buffer_pool_debug);
#define GST_CAT_DEFAULT gst_ivas_buffer_pool_debug

#define ALIGN(size,align) (((size) + (align) - 1) & ~((align) - 1))

enum
{
  PROP_0,
  PROP_STRIDE_ALIGN,
  PROP_ELEVATION_ALIGN,
};

struct _GstIvasBufferPoolPrivate
{
  GstVideoInfo vinfo;
  GstAllocator *allocator;
  gboolean add_videometa;
  GstAllocationParams params;
  guint stride_align;
  guint elevation_align;
};

#define parent_class gst_ivas_buffer_pool_parent_class
G_DEFINE_TYPE_WITH_CODE (GstIvasBufferPool, gst_ivas_buffer_pool,
    GST_TYPE_VIDEO_BUFFER_POOL, G_ADD_PRIVATE (GstIvasBufferPool);
    GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, "ivasbufferpool", 0,
        "IVAS buffer pool"));

static gboolean
gst_ivas_buffer_pool_set_config (GstBufferPool * pool, GstStructure * config)
{
  GstIvasBufferPool *xpool;
  GstIvasBufferPoolPrivate *priv;
  GstCaps *caps;
  GstVideoInfo vinfo;
  GstAllocator *allocator;
  GstAllocationParams params;
  guint size, min_buffers, max_buffers;
  gint align_stride, align_elevation, align_size;
  gsize offset[GST_VIDEO_MAX_PLANES] = { 0, };
  gint stride[GST_VIDEO_MAX_PLANES] = { 0, };

  xpool = GST_IVAS_BUFFER_POOL_CAST (pool);
  priv = xpool->priv;

  if (!gst_buffer_pool_config_get_params (config, &caps, &size, &min_buffers,
          &max_buffers))
    goto wrong_config;

  if (!caps)
    goto no_caps;

  /* now parse the caps from the config */
  if (!gst_video_info_from_caps (&vinfo, caps))
    goto wrong_caps;

  if (GST_VIDEO_INFO_FORMAT (&vinfo) != GST_VIDEO_FORMAT_NV12) {
    GST_ERROR_OBJECT (pool, "not yet supporting format other than NV12");
    return FALSE;
  }

  allocator = NULL;
  gst_buffer_pool_config_get_allocator (config, &allocator, &params);

  priv->params = params;

  /* not our allocator, not our buffers */
  if (allocator && GST_IS_IVAS_ALLOCATOR (allocator)) {
    if (priv->allocator)
      gst_object_unref (priv->allocator);
    priv->allocator = gst_object_ref (allocator);
  }
  if (!priv->allocator)
    goto no_allocator;

  priv->vinfo = vinfo;

  /* enable metadata based on config of the pool */
  priv->add_videometa = gst_buffer_pool_config_has_option (config,
      GST_BUFFER_POOL_OPTION_VIDEO_META);

  vinfo.size = MAX (size, vinfo.size);

  switch (GST_VIDEO_INFO_FORMAT (&vinfo)) {
    case GST_VIDEO_FORMAT_NV12:
      align_stride =
          ALIGN (GST_VIDEO_INFO_PLANE_STRIDE (&vinfo, 0),
          xpool->priv->stride_align);
      align_elevation =
          ALIGN (GST_VIDEO_INFO_HEIGHT (&vinfo), xpool->priv->elevation_align);
      stride[0] = stride[1] = align_stride;
      offset[0] = 0;
      offset[1] = offset[0] + stride[0] * align_elevation;
      align_size = (align_stride * align_elevation * 3) >> 1;
      break;
    default:
      g_assert_not_reached ();
      break;
  }

  vinfo.size = MAX (align_size, vinfo.size);

  GST_LOG_OBJECT (pool, "max buffer size %lu", vinfo.size);

  gst_buffer_pool_config_set_params (config, caps, vinfo.size, min_buffers,
      max_buffers);

  return GST_BUFFER_POOL_CLASS (parent_class)->set_config (pool, config);

  /* ERRORS */
wrong_config:
  {
    GST_WARNING_OBJECT (pool, "invalid config");
    return FALSE;
  }
no_caps:
  {
    GST_WARNING_OBJECT (pool, "no caps in config");
    return FALSE;
  }
wrong_caps:
  {
    GST_WARNING_OBJECT (pool,
        "failed getting geometry from caps %" GST_PTR_FORMAT, caps);
    return FALSE;
  }
no_allocator:
  {
    GST_WARNING_OBJECT (pool, "no valid allocator in pool");
    return FALSE;
  }
}

static GstFlowReturn
gst_ivas_buffer_pool_alloc_buffer (GstBufferPool * pool, GstBuffer ** buffer,
    GstBufferPoolAcquireParams * params)
{
  GstIvasBufferPool *xpool;
  GstIvasBufferPoolPrivate *priv;
  GstVideoInfo *info;
  gsize offset[GST_VIDEO_MAX_PLANES] = { 0, };
  gint stride[GST_VIDEO_MAX_PLANES] = { 0, };
  gint align_stride, align_elevation, align_size;

  xpool = GST_IVAS_BUFFER_POOL_CAST (pool);
  priv = xpool->priv;
  info = &priv->vinfo;

  switch (GST_VIDEO_INFO_FORMAT (info)) {
    case GST_VIDEO_FORMAT_NV12:
      align_stride =
          ALIGN (GST_VIDEO_INFO_PLANE_STRIDE (info, 0),
          xpool->priv->stride_align);
      align_elevation =
          ALIGN (GST_VIDEO_INFO_HEIGHT (info), xpool->priv->elevation_align);
      stride[0] = stride[1] = align_stride;
      offset[0] = 0;
      offset[1] = offset[0] + stride[0] * align_elevation;
      align_size = (align_stride * align_elevation * 3) >> 1;
      break;
    default:
      g_assert_not_reached ();
      break;
  }

  GST_DEBUG_OBJECT (pool, "alloc %d", align_size);

  *buffer =
      gst_buffer_new_allocate (priv->allocator, align_size, &priv->params);
  if (*buffer == NULL)
    goto no_memory;

  if (priv->add_videometa) {
    GST_DEBUG_OBJECT (pool, "adding GstVideoMeta");

    gst_buffer_add_video_meta_full (*buffer, GST_VIDEO_FRAME_FLAG_NONE,
        GST_VIDEO_INFO_FORMAT (info),
        GST_VIDEO_INFO_WIDTH (info), GST_VIDEO_INFO_HEIGHT (info),
        GST_VIDEO_INFO_N_PLANES (info), offset, stride);
  }
  return GST_FLOW_OK;

  /* ERROR */
no_memory:
  {
    GST_WARNING_OBJECT (pool, "can't create memory");
    return GST_FLOW_ERROR;
  }
}

static void
gst_ivas_buffer_pool_release_buffer (GstBufferPool * pool, GstBuffer * buffer)
{
  GstIvasBufferPool *xpool = GST_IVAS_BUFFER_POOL_CAST (pool);
  if (xpool->release_buf_cb)
    xpool->release_buf_cb (buffer, xpool->cb_user_data);
  GST_BUFFER_POOL_CLASS (parent_class)->release_buffer (pool, buffer);
}

static void
gst_ivas_buffer_pool_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstIvasBufferPool *xpool = GST_IVAS_BUFFER_POOL (object);

  switch (prop_id) {
    case PROP_STRIDE_ALIGN:
      xpool->priv->stride_align = g_value_get_uint (value);
      break;
    case PROP_ELEVATION_ALIGN:
      xpool->priv->elevation_align = g_value_get_uint (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_ivas_buffer_pool_init (GstIvasBufferPool * pool)
{
  pool->priv = gst_ivas_buffer_pool_get_instance_private (pool);
}

static void
gst_ivas_buffer_pool_class_init (GstIvasBufferPoolClass * klass)
{
  GObjectClass *gobject_class;
  GstBufferPoolClass *gstbufferpool_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->set_property = gst_ivas_buffer_pool_set_property;

  gstbufferpool_class = (GstBufferPoolClass *) klass;
  gstbufferpool_class->set_config = gst_ivas_buffer_pool_set_config;
  gstbufferpool_class->alloc_buffer = gst_ivas_buffer_pool_alloc_buffer;
  gstbufferpool_class->release_buffer = gst_ivas_buffer_pool_release_buffer;

  g_object_class_install_property (gobject_class, PROP_STRIDE_ALIGN,
      g_param_spec_uint ("stride-align", "Stride alignment of buffer",
          "Stride alignment of buffer", 0, G_MAXUINT,
          0, G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_ELEVATION_ALIGN,
      g_param_spec_uint ("elevation-align", "Elevation alignment of buffer",
          "Elevation alignment of buffer", 0, G_MAXUINT,
          0, G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));
}

GstBufferPool *
gst_ivas_buffer_pool_new (guint stride_align, guint elevation_align)
{
  GstBufferPool *pool;

  pool =
      g_object_new (GST_TYPE_IVAS_BUFFER_POOL, "stride-align", stride_align,
      "elevation-align", elevation_align, NULL);
  gst_object_ref_sink (pool);

  return pool;
}

void
gst_ivas_buffer_pool_set_release_buffer_cb (GstIvasBufferPool * xpool,
    ReleaseBufferCallback release_buf_cb, gpointer user_data)
{
  xpool->release_buf_cb = release_buf_cb;
  xpool->cb_user_data = user_data;
}

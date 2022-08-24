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

#include <gstivasmeta.h>
#include <stdio.h>

GType
gst_ivas_meta_api_get_type (void)
{
  static volatile GType type = 0;
  static const gchar *tags[] =
      { GST_META_TAG_VIDEO_STR, GST_META_TAG_VIDEO_SIZE_STR,
    GST_META_TAG_VIDEO_ORIENTATION_STR, NULL
  };

  if (g_once_init_enter (&type)) {
    GType _type = gst_meta_api_type_register ("GstIvasMetaAPI", tags);
    g_once_init_leave (&type, _type);
  }

  return type;
}

static gboolean
gst_ivas_meta_init (GstMeta * meta, gpointer params, GstBuffer * buffer)
{
  GstIvasMeta *ivasmeta = (GstIvasMeta *) meta;

  ivasmeta->xmeta.objects = NULL;
  ivasmeta->xmeta.display_meta = NULL;
  ivasmeta->xmeta.pts = -1;

  return TRUE;
}

static void
ivas_free_object (gpointer data)
{
  IvasObjectMetadata *obj = data;

  if (obj->obj_list) {
    g_list_free_full (g_steal_pointer (&obj->obj_list),
        (GDestroyNotify) gst_buffer_unref);
  }

  g_free (data);
}

static gboolean
gst_ivas_meta_free (GstMeta * meta, GstBuffer * buffer)
{
  GstIvasMeta *ivasmeta = (GstIvasMeta *) meta;

  GST_LOG ("freeing ivas meta %p with buffer %p", ivasmeta, buffer);

  if (ivasmeta->xmeta.objects) {
    g_list_free_full (g_steal_pointer (&ivasmeta->xmeta.objects),
        ivas_free_object);
    ivasmeta->xmeta.objects = NULL;
  }

  if (ivasmeta->xmeta.display_meta) {
    g_list_free_full (ivasmeta->xmeta.display_meta, g_free);
    ivasmeta->xmeta.display_meta = NULL;
  }

  return TRUE;
}

static gboolean
gst_ivas_meta_transform (GstBuffer * dest, GstMeta * meta,
    GstBuffer * buffer, GQuark type, gpointer data)
{
  GstIvasMeta *dmeta, *smeta;
  GList *node;

  if (GST_META_TRANSFORM_IS_COPY (type)) {
    IvasObjectMetadata *sobj = NULL;

    smeta = (GstIvasMeta *) meta;
    dmeta = gst_buffer_add_ivas_meta (dest);

    if (!dmeta)
      return FALSE;

    GST_LOG ("copy metadata from %p -> %p buffer %p -> %p", smeta, dmeta,
        buffer, dest);
    node = smeta->xmeta.objects;

    while (node) {
      GList *src_lbuf = NULL;

      IvasObjectMetadata *dobj =
          (IvasObjectMetadata *) calloc (1, sizeof (IvasObjectMetadata));

      if (dobj == NULL) {
        GST_ERROR ("Failed to allocate meta obj data buffer");
        return FALSE;
      }

      sobj = (IvasObjectMetadata *) node->data;
      GST_LOG ("copying metaobject %p->%p", sobj, dobj);

      memcpy (dobj, sobj, sizeof (IvasObjectMetadata));
      dobj->obj_list = NULL;

      src_lbuf = sobj->obj_list;
      while (src_lbuf) {
        GstBuffer *obj_buf;

        obj_buf = gst_buffer_ref (GST_BUFFER (src_lbuf->data));
        dobj->obj_list = g_list_append (dobj->obj_list, obj_buf);
        src_lbuf = g_list_next (src_lbuf);
      }
      dmeta->xmeta.objects = g_list_append (dmeta->xmeta.objects, dobj);
      node = g_list_next (node);
    }
  } else if (GST_VIDEO_META_TRANSFORM_IS_SCALE (type)) {
    GstVideoMetaTransform *trans = data;
    IvasObjectMetadata *sobj = NULL;
    float x_scale_f;
    float y_scale_f;

    smeta = (GstIvasMeta *) meta;
    dmeta = gst_buffer_add_ivas_meta (dest);

    if (!dmeta)
      return FALSE;

    node = smeta->xmeta.objects;

    x_scale_f =
        (float) GST_VIDEO_INFO_WIDTH (trans->out_info) /
        GST_VIDEO_INFO_WIDTH (trans->in_info);
    y_scale_f =
        (float) GST_VIDEO_INFO_HEIGHT (trans->out_info) /
        GST_VIDEO_INFO_HEIGHT (trans->in_info);

    while (node) {
      IvasObjectMetadata *dobj =
          (IvasObjectMetadata *) calloc (1, sizeof (IvasObjectMetadata));

      if (dobj == NULL) {
        GST_ERROR ("Failed to allocate meta obj data buffer");
        return FALSE;
      }

      sobj = (IvasObjectMetadata *) node->data;

      dobj->bbox_meta.xmin = sobj->bbox_meta.xmin * x_scale_f;
      dobj->bbox_meta.ymin = sobj->bbox_meta.ymin * y_scale_f;
      dobj->bbox_meta.xmax = sobj->bbox_meta.xmax * x_scale_f;
      dobj->bbox_meta.ymax = sobj->bbox_meta.ymax * y_scale_f;

      GST_LOG ("source metadata : (%f, %f) -> (%f, %f)", sobj->bbox_meta.xmin,
          sobj->bbox_meta.ymin, sobj->bbox_meta.xmax, sobj->bbox_meta.ymax);
      GST_LOG ("destination metadata : (%f, %f) -> (%f, %f)",
          dobj->bbox_meta.xmin, dobj->bbox_meta.ymin, dobj->bbox_meta.xmax,
          dobj->bbox_meta.ymax);

      dmeta->xmeta.objects = g_list_append (dmeta->xmeta.objects, dobj);
      node = g_list_next (node);
    }
  } else {
    return FALSE;
  }

  return TRUE;
}

const GstMetaInfo *
gst_ivas_meta_get_info (void)
{
  static const GstMetaInfo *ivas_meta_info = NULL;

  if (g_once_init_enter ((GstMetaInfo **) & ivas_meta_info)) {
    const GstMetaInfo *meta =
        gst_meta_register (GST_IVAS_META_API_TYPE, "GstIvasMeta",
        sizeof (GstIvasMeta), (GstMetaInitFunction) gst_ivas_meta_init,
        (GstMetaFreeFunction) gst_ivas_meta_free, gst_ivas_meta_transform);
    g_once_init_leave ((GstMetaInfo **) & ivas_meta_info, (GstMetaInfo *) meta);
  }
  return ivas_meta_info;
}

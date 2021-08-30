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


#include <gstivasinpinfer.h>


GType
gst_ivas_inp_infer_meta_api_get_type (void)
{
  static volatile GType type = 0;
  static const gchar *tags[] =
      { GST_META_TAG_VIDEO_STR, GST_META_TAG_VIDEO_SIZE_STR,
    GST_META_TAG_VIDEO_ORIENTATION_STR, NULL
  };

  if (g_once_init_enter (&type)) {
    GType _type = gst_meta_api_type_register ("GstIvasInpInferMetaAPI", tags);
    g_once_init_leave (&type, _type);
  }

  return type;
}

static gboolean
gst_ivas_inp_infer_meta_init (GstMeta * meta, gpointer params, GstBuffer * buffer)
{
  GstIvasInpInferMeta *ivasmeta = (GstIvasInpInferMeta *) meta;

  ivasmeta->ml_class = IVAS_XCLASS_NOTFOUND;
  ivasmeta->model_name = NULL;

  return TRUE;
}


static gboolean
gst_ivas_inp_infer_meta_free (GstMeta * meta, GstBuffer * buffer)
{
  GstIvasInpInferMeta *ivasmeta = (GstIvasInpInferMeta *) meta;

  if(ivasmeta->model_name) {
    g_free (ivasmeta->model_name);
    ivasmeta->model_name = NULL;
  }
  
  return TRUE;
}


static gboolean
gst_ivas_inp_infer_meta_transform (GstBuffer * dest, GstMeta * meta,
    GstBuffer * buffer, GQuark type, gpointer data)
{
  GstIvasInpInferMeta *smeta;

  smeta = (GstIvasInpInferMeta *) meta;
  gst_buffer_add_ivas_inp_infer_meta (dest, smeta->ml_class, smeta->model_name);
  
  return TRUE;
}

const GstMetaInfo *
gst_ivas_inp_infer_meta_get_info (void)
{
  static const GstMetaInfo *ivas_inp_infer_meta_info = NULL;

  if (g_once_init_enter ((GstMetaInfo **) & ivas_inp_infer_meta_info)) {
    const GstMetaInfo *meta =
        gst_meta_register (GST_IVAS_INP_INFER_META_API_TYPE, "GstIvasInpInferMeta",
        sizeof (GstIvasInpInferMeta), 
        (GstMetaInitFunction) gst_ivas_inp_infer_meta_init,
        (GstMetaFreeFunction) gst_ivas_inp_infer_meta_free,
        gst_ivas_inp_infer_meta_transform);
    g_once_init_leave ((GstMetaInfo **) & ivas_inp_infer_meta_info, (GstMetaInfo *) meta);
  }
  return ivas_inp_infer_meta_info;
}

GstIvasInpInferMeta *
gst_buffer_add_ivas_inp_infer_meta (GstBuffer *buffer, IvasClass ml_class, gchar *model_name)
{
  GstIvasInpInferMeta *meta;
  g_return_val_if_fail (GST_IS_BUFFER (buffer), NULL);

  meta = (GstIvasInpInferMeta *) gst_buffer_add_meta (buffer,
  GST_IVAS_INP_INFER_EXAMPLE_META_INFO, NULL);

  meta->ml_class = ml_class;
  meta->model_name = g_strdup (model_name);

  return meta;
}

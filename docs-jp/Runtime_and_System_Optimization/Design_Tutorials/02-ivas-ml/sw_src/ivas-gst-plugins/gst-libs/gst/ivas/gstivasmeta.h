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

#ifndef __GST_IVAS_META_H__
#define __GST_IVAS_META_H__

#include <gst/gst.h>
#include <gst/video/video.h>
#include <string.h>
#include <ivas/ivasmeta.h>

G_BEGIN_DECLS

#define GST_IVAS_META_API_TYPE  (gst_ivas_meta_api_get_type())
#define GST_IVAS_META_INFO  (gst_ivas_meta_get_info())

typedef struct _ivas_meta {
  /* List of IvasObjectMetadata objects */
  GList *objects;
  GList *display_meta;
  uint64_t pts;
} IvasMeta;

typedef struct _GstIvasMeta GstIvasMeta;

struct _GstIvasMeta {
  GstMeta meta;
  IvasMeta xmeta;
};

GST_EXPORT
GType gst_ivas_meta_api_get_type (void);

GST_EXPORT
const GstMetaInfo * gst_ivas_meta_get_info (void);

#define gst_buffer_get_ivas_meta(b) ((GstIvasMeta*)gst_buffer_get_meta((b),GST_IVAS_META_API_TYPE))
#define gst_buffer_add_ivas_meta(b) ((GstIvasMeta*)gst_buffer_add_meta((b),GST_IVAS_META_INFO, NULL))

G_END_DECLS

#endif /* __GST_IVAS_META_H__ */

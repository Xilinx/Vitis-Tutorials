/*
 * Copyright (C) 2019 - 2020 Xilinx, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * XILINX BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the Xilinx shall not be used
 * in advertising or otherwise to promote the sale, use or other dealings in
 * this Software without prior written authorization from Xilinx.
 */

#ifndef __GST_IVAS_ALLOCATOR_H__
#define __GST_IVAS_ALLOCATOR_H__

#include <gst/gst.h>
#include <gst/video/video.h>
#ifdef XLNX_PCIe_PLATFORM
#include <xrt.h>
#else
#include <xrt/xrt.h>
#endif

G_BEGIN_DECLS

#define GST_TYPE_IVAS_ALLOCATOR  \
   (gst_ivas_allocator_get_type())
#define GST_IS_IVAS_ALLOCATOR(obj)       \
   (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_IVAS_ALLOCATOR))
#define GST_IS_IVAS_ALLOCATOR_CLASS(klass)     \
   (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_IVAS_ALLOCATOR))
#define GST_IVAS_ALLOCATOR_GET_CLASS(obj)      \
   (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_IVAS_ALLOCATOR, GstIvasAllocatorClass))
#define GST_IVAS_ALLOCATOR(obj)        \
   (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_IVAS_ALLOCATOR, GstIvasAllocator))
#define GST_IVAS_ALLOCATOR_CLASS(klass)      \
   (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_IVAS_ALLOCATOR, GstIvasAllocatorClass))

typedef struct _GstIvasAllocator GstIvasAllocator;
typedef struct _GstIvasAllocatorClass GstIvasAllocatorClass;
typedef struct _GstIvasAllocatorPrivate GstIvasAllocatorPrivate;

struct _GstIvasAllocator
{
  GstAllocator parent;
  GstIvasAllocatorPrivate *priv;
};

struct _GstIvasAllocatorClass {
  GstAllocatorClass parent_class;
};

GST_EXPORT
GType gst_ivas_allocator_get_type (void) G_GNUC_CONST;
GST_EXPORT
GstAllocator* gst_ivas_allocator_new (guint dev_idx, gboolean need_dma);
GST_EXPORT
gboolean gst_is_ivas_memory (GstMemory *mem);
GST_EXPORT
guint64  gst_ivas_allocator_get_paddr (GstMemory *mem);
GST_EXPORT
guint  gst_ivas_allocator_get_bo (GstMemory *mem);
GST_EXPORT
gboolean gst_ivas_memory_can_avoid_copy (GstMemory *mem, guint cur_devid);
GST_EXPORT
guint gst_ivas_allocator_get_device_idx (GstAllocator * allocator);

#ifdef XLNX_PCIe_PLATFORM
typedef enum {
  IVAS_SYNC_NONE = 0,
  IVAS_SYNC_TO_DEVICE = 1 << 0, /* sync data to device using DMA transfer */
  IVAS_SYNC_FROM_DEVICE = 1 << 1, /* sync data to device using DMA transfer */
} IvasSyncFlags;

GST_EXPORT
void gst_ivas_memory_set_sync_flag (GstMemory *mem, IvasSyncFlags flag);
GST_EXPORT
gboolean gst_ivas_memory_sync_bo (GstMemory *mem);
GST_EXPORT
gboolean gst_ivas_memory_sync_with_flags (GstMemory *mem, GstMapFlags flags);
#endif

G_END_DECLS

#endif /* __GST_IVAS_ALLOCATOR_H__ */

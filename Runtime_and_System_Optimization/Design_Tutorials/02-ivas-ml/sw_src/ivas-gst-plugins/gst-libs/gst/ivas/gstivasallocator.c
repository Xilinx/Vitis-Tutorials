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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gstivasallocator.h"
#include <sys/mman.h>
#include <string.h>
#include <gst/allocators/gstdmabuf.h>

#define GST_CAT_DEFAULT ivasallocator_debug
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);

#define GST_IVAS_MEMORY_TYPE "IVASMemory"
#define DEFAULT_DEVICE_INDEX 0
#define DEFAULT_NEED_DMA FALSE

enum
{
  PROP_0,
  PROP_DEVICE_INDEX,
  PROP_NEED_DMA,
};

struct _GstIvasAllocatorPrivate
{
  guint dev_idx;
  gboolean need_dma;
  xclDeviceHandle handle;
  GstAllocator *dmabuf_alloc;
};

typedef struct _GstIvasMemory
{
  GstMemory parent;
  gpointer data;                /* used in non-dma mode */
  unsigned int bo;
  gsize size;
  GstIvasAllocator *alloc;
  GstMapFlags mmapping_flags;
  gint mmap_count;
  GMutex lock;
#ifdef XLNX_PCIe_PLATFORM
  IvasSyncFlags sync_flags;
#endif
} GstIvasMemory;

#define parent_class gst_ivas_allocator_parent_class
G_DEFINE_TYPE_WITH_CODE (GstIvasAllocator, gst_ivas_allocator,
    GST_TYPE_ALLOCATOR, G_ADD_PRIVATE (GstIvasAllocator);
    GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, "ivasallocator", 0,
        "IVAS allocator"));
GST_DEBUG_CATEGORY_STATIC (GST_CAT_PERFORMANCE);

static GstIvasMemory *
get_ivas_mem (GstMemory * mem)
{
  g_return_val_if_fail (mem != NULL, NULL);

  if (GST_IS_IVAS_ALLOCATOR (mem->allocator)) {
    return (GstIvasMemory *) mem;
  } else if (gst_is_dmabuf_memory (mem)) {
    return (GstIvasMemory *) gst_mini_object_get_qdata (GST_MINI_OBJECT (mem),
        g_quark_from_static_string ("ivasmem"));
  } else {
    return NULL;
  }
}

static xclDeviceHandle
ivas_open_device (guint dev_idx)
{
  xclDeviceHandle handle = NULL;

  if (dev_idx >= xclProbe ()) {
    GST_ERROR ("Cannot find device index specified\n");
    return NULL;
  }

  handle = xclOpen (dev_idx, NULL, XCL_INFO);
  if (handle == NULL) {
    GST_ERROR ("failed to open device with idx %d. reason : %s", dev_idx,
        strerror (errno));
    return NULL;
  }

  return handle;
}

static GstMemory *
gst_ivas_allocator_alloc (GstAllocator * allocator, gsize size,
    GstAllocationParams * params)
{
  GstIvasAllocator *alloc = GST_IVAS_ALLOCATOR (allocator);
  GstIvasMemory *ivasmem;
  GstMemory *mem;
  gint prime_fd = 0;

  if (alloc->priv->handle == NULL) {
    GST_ERROR_OBJECT (alloc, "failed get handle from IVAS");
    return NULL;
  }

  ivasmem = g_slice_new0 (GstIvasMemory);
  gst_memory_init (GST_MEMORY_CAST (ivasmem), params->flags,
      GST_ALLOCATOR_CAST (alloc), NULL, size, params->align, params->prefix,
      size);

  GST_DEBUG_OBJECT (alloc, "flags %d, align %lu, prefix %lu", params->flags,
      params->align, params->prefix);
  ivasmem->alloc = alloc;
  g_mutex_init (&ivasmem->lock);

#ifdef XLNX_PCIe_PLATFORM
  ivasmem->sync_flags = IVAS_SYNC_NONE;
#endif

  ivasmem->bo = xclAllocBO (alloc->priv->handle, size, XCL_BO_DEVICE_RAM, 0);
  if (ivasmem->bo == NULLBO) {
    GST_ERROR_OBJECT (alloc, "failed to allocate Device BO. reason %s(%d)",
        strerror (errno), errno);
    return NULL;
  }
  ivasmem->size = size;

  if (alloc->priv->need_dma) {
    prime_fd = xclExportBO (alloc->priv->handle, ivasmem->bo);
    if (prime_fd < 0) {
      GST_ERROR_OBJECT (alloc, "failed to get dmafd...");
      xclFreeBO (alloc->priv->handle, ivasmem->bo);
      ivasmem->bo = 0;
      return NULL;
    }

    GST_DEBUG_OBJECT (alloc,
        "memory allocated of size %lu, bo = %u and dmafd = %d", size,
        ivasmem->bo, prime_fd);

    mem =
        gst_dmabuf_allocator_alloc (alloc->priv->dmabuf_alloc, prime_fd, size);

    gst_mini_object_set_qdata (GST_MINI_OBJECT (mem),
        g_quark_from_static_string ("ivasmem"), ivasmem,
        (GDestroyNotify) gst_memory_unref);
  } else {
    mem = GST_MEMORY_CAST (ivasmem);
  }

  return mem;
}

static gpointer
gst_ivas_mem_map (GstMemory * mem, gsize maxsize, GstMapFlags flags)
{
  GstIvasMemory *ivasmem = (GstIvasMemory *) mem;
  GstIvasAllocator *alloc = ivasmem->alloc;
  gpointer ret = NULL;

  g_mutex_lock (&ivasmem->lock);

  if (ivasmem->data) {
    /* only return address if mapping flags are a subset
     * of the previous flags */
    if ((ivasmem->mmapping_flags & flags) == flags) {
      ret = ivasmem->data;
      ivasmem->mmap_count++;
    }
    goto out;
  }
#ifdef XLNX_PCIe_PLATFORM
  if (!gst_ivas_memory_sync_with_flags (mem, flags)) {
    g_mutex_unlock (&ivasmem->lock);
    return NULL;
  }
#endif

  ivasmem->data =
      xclMapBO (alloc->priv->handle, ivasmem->bo, flags & GST_MAP_WRITE);
  GST_DEBUG_OBJECT (alloc, "mapped pointer %p with size %lu do_write %d",
      ivasmem->data, maxsize, flags & GST_MAP_WRITE);

  if (ivasmem->data) {
    ivasmem->mmapping_flags = flags;
    ivasmem->mmap_count++;
    ret = ivasmem->data;
  }

out:
  g_mutex_unlock (&ivasmem->lock);
  return ret;
}

static GstMemory *
gst_ivas_mem_share (GstMemory * mem, gssize offset, gssize size)
{
  GstIvasMemory *ivasmem = (GstIvasMemory *) mem;
  GstMemory *parent;
  GstIvasMemory *sub;

  GST_DEBUG ("%p: share %" G_GSSIZE_FORMAT " %" G_GSIZE_FORMAT, mem, offset,
      size);

  /* find the real parent */
  if ((parent = ivasmem->parent.parent) == NULL)
    parent = GST_MEMORY_CAST (ivasmem);

  if (size == -1)
    size = mem->maxsize - offset;

  sub = g_slice_new0 (GstIvasMemory);

  /* shared memory is always readonly */
  gst_memory_init (GST_MEMORY_CAST (sub), GST_MINI_OBJECT_FLAGS (parent) |
      GST_MINI_OBJECT_FLAG_LOCK_READONLY, ivasmem->parent.allocator, parent,
      ivasmem->parent.maxsize, ivasmem->parent.align,
      ivasmem->parent.offset + offset, size);

  g_mutex_init (&sub->lock);
  sub->bo = ivasmem->bo;
  sub->alloc = ivasmem->alloc;
  sub->size = ivasmem->size;

#ifdef XLNX_PCIe_PLATFORM
  sub->sync_flags = ivasmem->sync_flags;
#endif

  GST_DEBUG ("%p: share mem created", sub);

  return GST_MEMORY_CAST (sub);
}

static void
gst_ivas_mem_unmap (GstMemory * mem)
{
  GstIvasMemory *ivasmem = (GstIvasMemory *) mem;
  GstIvasAllocator *alloc = ivasmem->alloc;
  int ret = 0;

  if (mem->parent)
    return gst_ivas_mem_unmap (mem->parent);

  g_mutex_lock (&ivasmem->lock);
  if (ivasmem->data && !(--ivasmem->mmap_count)) {
    //ret = xclUnmapBO (alloc->priv->handle, ivasmem->bo, ivasmem->data);
    ret = munmap (ivasmem->data, ivasmem->size);
    if (ret) {
      GST_ERROR ("failed to unmap %p", ivasmem->data);
    }
    ivasmem->data = NULL;
    ivasmem->mmapping_flags = 0;
    GST_DEBUG_OBJECT (alloc, "%p: bo %d unmapped", ivasmem, ivasmem->bo);
  }
  g_mutex_unlock (&ivasmem->lock);
}

static void
gst_ivas_allocator_free (GstAllocator * allocator, GstMemory * mem)
{
  GstIvasMemory *ivasmem = (GstIvasMemory *) mem;
  GstIvasAllocator *alloc = GST_IVAS_ALLOCATOR (allocator);

  if (ivasmem->bo != NULLBO && mem->parent == NULL)
    xclFreeBO (alloc->priv->handle, ivasmem->bo);

  g_mutex_clear (&ivasmem->lock);
  g_slice_free (GstIvasMemory, ivasmem);
  GST_DEBUG ("%p: freed", mem);
}

static void
gst_ivas_allocator_finalize (GObject * obj)
{
  GstIvasAllocator *alloc = GST_IVAS_ALLOCATOR (obj);

  if (alloc->priv->dmabuf_alloc)
    gst_object_unref (alloc->priv->dmabuf_alloc);

  xclClose (alloc->priv->handle);
  G_OBJECT_CLASS (parent_class)->finalize (obj);
}

static void
gst_ivas_allocator_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstIvasAllocator *alloc = GST_IVAS_ALLOCATOR (object);

  switch (prop_id) {
    case PROP_DEVICE_INDEX:
      alloc->priv->dev_idx = g_value_get_uint (value);
      alloc->priv->handle = ivas_open_device (alloc->priv->dev_idx);
      break;
    case PROP_NEED_DMA:
      alloc->priv->need_dma = g_value_get_boolean (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_ivas_allocator_class_init (GstIvasAllocatorClass * klass)
{
  GObjectClass *gobject_class;
  GstAllocatorClass *allocator_class;

  gobject_class = G_OBJECT_CLASS (klass);
  allocator_class = GST_ALLOCATOR_CLASS (klass);

  gobject_class->finalize = gst_ivas_allocator_finalize;
  gobject_class->set_property = gst_ivas_allocator_set_property;

  allocator_class->free = gst_ivas_allocator_free;
  allocator_class->alloc = gst_ivas_allocator_alloc;

  g_object_class_install_property (gobject_class, PROP_DEVICE_INDEX,
      g_param_spec_uint ("device-index", "IVAS device index",
          "IVAS device index to allocate memory", 0, G_MAXUINT,
          DEFAULT_DEVICE_INDEX, G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_NEED_DMA,
      g_param_spec_boolean ("need-dma", "Expose as DMABuf Allocator",
          "Expose as DMABuf Allocator", DEFAULT_NEED_DMA,
          G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

  GST_DEBUG_CATEGORY_GET (GST_CAT_PERFORMANCE, "GST_PERFORMANCE");
}

static void
gst_ivas_allocator_init (GstIvasAllocator * allocator)
{
  GstAllocator *alloc;

  alloc = GST_ALLOCATOR_CAST (allocator);
  allocator->priv = (GstIvasAllocatorPrivate *)
      gst_ivas_allocator_get_instance_private (allocator);

  alloc->mem_type = GST_IVAS_MEMORY_TYPE;
  alloc->mem_map = gst_ivas_mem_map;
  alloc->mem_unmap = gst_ivas_mem_unmap;
  alloc->mem_share = gst_ivas_mem_share;

  allocator->priv->dmabuf_alloc = gst_dmabuf_allocator_new ();

  GST_OBJECT_FLAG_SET (allocator, GST_ALLOCATOR_FLAG_CUSTOM_ALLOC);
}

GstAllocator *
gst_ivas_allocator_new (guint dev_idx, gboolean need_dma)
{
  GstAllocator *alloc = NULL;

  alloc = (GstAllocator *) g_object_new (GST_TYPE_IVAS_ALLOCATOR,
      "device-index", dev_idx, "need-dma", need_dma, NULL);
  gst_object_ref_sink (alloc);
  return alloc;
}

gboolean
gst_is_ivas_memory (GstMemory * mem)
{
  g_return_val_if_fail (mem != NULL, FALSE);

  if (get_ivas_mem (mem))
    return TRUE;
  return FALSE;
}

guint64
gst_ivas_allocator_get_paddr (GstMemory * mem)
{
  GstIvasMemory *ivasmem;
  GstIvasAllocator *alloc;
  struct xclBOProperties p;

  g_return_val_if_fail (mem != NULL, FALSE);

  ivasmem = get_ivas_mem (mem);
  if (ivasmem == NULL) {
    GST_ERROR ("failed to get ivas memory");
    return 0;
  }

  alloc = ivasmem->alloc;

  if (!xclGetBOProperties (alloc->priv->handle, ivasmem->bo, &p)) {
    return p.paddr;
  } else {
    GST_ERROR_OBJECT (alloc, "failed to get physical address...");
    return 0;
  }
}

guint
gst_ivas_allocator_get_bo (GstMemory * mem)
{
  GstIvasMemory *ivasmem;

  ivasmem = get_ivas_mem (mem);
  if (ivasmem == NULL) {
    GST_ERROR ("failed to get ivas memory");
    return 0;
  }
  return ivasmem->bo;
}

gboolean
gst_ivas_memory_can_avoid_copy (GstMemory * mem, guint cur_devid)
{
  GstIvasAllocator *alloc;
  GstIvasMemory *ivasmem;

  ivasmem = get_ivas_mem (mem);
  if (ivasmem == NULL) {
    GST_ERROR ("failed to get ivas memory");
    return FALSE;
  }

  alloc = ivasmem->alloc;
  return (alloc->priv->dev_idx == cur_devid);
}

guint
gst_ivas_allocator_get_device_idx (GstAllocator * allocator)
{
  GstIvasAllocator *alloc;
  g_return_val_if_fail (GST_IS_IVAS_ALLOCATOR (allocator), -1);
  alloc = GST_IVAS_ALLOCATOR (allocator);
  return alloc->priv->dev_idx;
}

#ifdef XLNX_PCIe_PLATFORM
void
gst_ivas_memory_set_sync_flag (GstMemory * mem, IvasSyncFlags flag)
{
  GstIvasMemory *ivasmem;
  ivasmem = get_ivas_mem (mem);
  ivasmem->sync_flags |= flag;
}

gboolean
gst_ivas_memory_sync_with_flags (GstMemory * mem, GstMapFlags flags)
{
  GstIvasMemory *ivasmem;
  GstIvasAllocator *alloc;

  ivasmem = get_ivas_mem (mem);
  if (ivasmem == NULL) {
    GST_ERROR ("failed to get ivas memory");
    return FALSE;
  }

  alloc = ivasmem->alloc;

  if ((flags & GST_MAP_READ) && (ivasmem->sync_flags & IVAS_SYNC_FROM_DEVICE)) {
    int iret = 0;

    GST_LOG_OBJECT (alloc,
        "sync to device %p : bo = %u, size = %lu, handle = %p", ivasmem,
        ivasmem->bo, ivasmem->size, alloc->priv->handle);

    GST_CAT_LOG_OBJECT (GST_CAT_PERFORMANCE, alloc,
        "slow copy data from device");

    iret =
        xclSyncBO (alloc->priv->handle, ivasmem->bo, XCL_BO_SYNC_BO_FROM_DEVICE,
        ivasmem->size, 0);
    if (iret != 0) {
      GST_ERROR_OBJECT (alloc, "failed to sync output buffer. reason : %d, %s",
          iret, strerror (errno));
      return FALSE;
    }
    ivasmem->sync_flags &= ~IVAS_SYNC_FROM_DEVICE;
  }

  if (flags & GST_MAP_WRITE) {
    ivasmem->sync_flags |= IVAS_SYNC_TO_DEVICE;
    /* ivas plugins does XCL_BO_SYNC_BO_TO_DEVICE to update data, for others dont care */
    GST_LOG_OBJECT (alloc, "enabling sync to device flag for %p", ivasmem);
  }
  return TRUE;
}

gboolean
gst_ivas_memory_sync_bo (GstMemory * mem)
{
  GstIvasMemory *ivasmem;
  GstIvasAllocator *alloc;
  int iret = 0;

  ivasmem = get_ivas_mem (mem);
  if (ivasmem == NULL) {
    GST_ERROR ("failed to get ivas memory");
    return 0;
  }

  alloc = ivasmem->alloc;

  GST_LOG_OBJECT (alloc, "sync bo %u of size %lu, handle = %p, flags %d",
      ivasmem->bo, ivasmem->size, alloc->priv->handle, ivasmem->sync_flags);

  if ((ivasmem->sync_flags & IVAS_SYNC_TO_DEVICE)
      && (ivasmem->sync_flags & IVAS_SYNC_FROM_DEVICE)) {
    GST_ERROR_OBJECT (alloc, "should not reach here.. both sync flags enabled");
    return FALSE;
  }

  if (ivasmem->sync_flags & IVAS_SYNC_TO_DEVICE) {
    GST_CAT_LOG_OBJECT (GST_CAT_PERFORMANCE, alloc, "slow copy data to device");

    iret =
        xclSyncBO (alloc->priv->handle, ivasmem->bo, XCL_BO_SYNC_BO_TO_DEVICE,
        ivasmem->size, 0);
    if (iret != 0) {
      GST_ERROR_OBJECT (alloc,
          "failed to sync output buffer to device. reason : %d, %s", iret,
          strerror (errno));
      return FALSE;
    }
  }

  return TRUE;
}
#endif

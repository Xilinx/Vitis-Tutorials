/*
* Copyright (C) 2020 - 2021 Xilinx, Inc.  All rights reserved.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef XLNX_PCIe_PLATFORM
#undef USE_XRM                  // for PCie based platforms use XRM
#define USE_DMABUF 0
#define MANUAL_SOFTKERNEL_DOWNLOAD      // till XRM stabilizes, use manual download of sk
#else /* Embedded */
#undef USE_XRM
#define USE_DMABUF 1
#endif

#include <gst/gst.h>
#include <gst/base/base.h>
#include <gst/ivas/gstivasallocator.h>
#include <gst/allocators/gstdmabuf.h>
#include <dlfcn.h>              /* for dlXXX APIs */
#include <sys/mman.h>           /* for munmap */
#include <jansson.h>
#ifdef XLNX_PCIe_PLATFORM
#include <experimental/xrt-next.h>
#else
#include <xrt/experimental/xrt-next.h>
#endif
#include <ivas/ivas_kernel.h>
#include "gstivas_xfilter.h"

GST_DEBUG_CATEGORY_STATIC (gst_ivas_xfilter_debug);
#define GST_CAT_DEFAULT gst_ivas_xfilter_debug
GST_DEBUG_CATEGORY_STATIC (GST_CAT_PERFORMANCE);

pthread_mutex_t count_mutex;

static const int ERT_CMD_SIZE = 4096;
#define CMD_EXEC_TIMEOUT 1000   // 1 sec
#define MIN_POOL_BUFFERS 2
#define DEFAULT_IVAS_LIB_PATH "/usr/lib/"
#define DEFAULT_DEVICE_INDEX 0
#define MAX_PRIV_POOLS 10
#define ALIGN(size,align) (((size) + (align) - 1) & ~((align) - 1))

#if defined(XLNX_PCIe_PLATFORM) && defined (USE_XRM)
#include "xrm.h"
#endif

#include "ivas/xrt_utils.h"

#if defined(XLNX_PCIe_PLATFORM) && defined (MANUAL_SOFTKERNEL_DOWNLOAD)
#define  IVAS_SK_START_IDX (self->priv->sk_start_idx)
#define IVAS_SK_COUNT 8
#define SOFTKERNEL_BASE_ADDRESS 0x1800000
#endif

typedef struct _GstIvas_XFilterPrivate GstIvas_XFilterPrivate;

enum
{
  SIGNAL_IVAS,

  /* add more signal above this */
  SIGNAL_LAST
};

static guint ivas_signals[SIGNAL_LAST] = { 0 };

typedef struct
{
  gchar *skname;
} IvasSoftKernelInfo;

typedef struct
{
  gchar *name;
  json_t *config;
  gchar *ivas_lib_path;
  void *lib_fd;
  gint cu_idx;
  IVASKernelInit kernel_init_func;
  IVASKernelStartFunc kernel_start_func;
  IVASKernelDoneFunc kernel_done_func;
  IVASKernelDeInit kernel_deinit_func;
  IVASKernel *ivas_handle;
  GstVideoFrame in_vframe;
  GstVideoFrame out_vframe;
  IVASFrame *input[MAX_NUM_OBJECT];
  IVASFrame *output[MAX_NUM_OBJECT];
  gboolean is_softkernel;
#ifdef XLNX_PCIe_PLATFORM
  IvasSoftKernelInfo *skinfo;
#endif
} Ivas_XFilter;

enum
{
  PROP_0,
  PROP_CONFIG_LOCATION,
  PROP_DYNAMIC_CONFIG,
#if defined(XLNX_PCIe_PLATFORM)
#if defined (MANUAL_SOFTKERNEL_DOWNLOAD)
  PROP_SK_LIB_PATH,             /* will be depecated after XRM */
  PROP_SK_CURRENT_INDEX,
  PROP_SK_START_INDEX,
  PROP_DEVICE_INDEX,
#elif defined(USE_XRM)
  PROP_RESERVATION_ID,
#endif
#endif
};

typedef enum
{
  IVAS_ELEMENT_MODE_NOT_SUPPORTED,
  IVAS_ELEMENT_MODE_PASSTHROUGH,        /* does not alter input buffer */
  IVAS_ELEMENT_MODE_IN_PLACE,   /* going to change input buffer content */
  IVAS_ELEMENT_MODE_TRANSFORM,  /* input and output buffers are different */
} Ivas_XFilterMode;

struct _GstIvas_XFilterPrivate
{
  guint dev_idx;
  xclDeviceHandle xcl_handle;
  gchar *xclbin_loc;
  json_t *root;
  Ivas_XFilter *kernel;
  Ivas_XFilterMode element_mode;
  gboolean do_init;
  GstVideoInfo *in_vinfo;
  GstVideoInfo *out_vinfo;
  uuid_t xclbinId;
  xrt_buffer *ert_cmd_buf;
  GstBufferPool *input_pool;
  GstBufferPool *priv_pools[MAX_PRIV_POOLS];
  GHashTable *priv_buf_vmaps;
  json_t *dyn_json_config;
#ifdef XLNX_PCIe_PLATFORM
#ifdef MANUAL_SOFTKERNEL_DOWNLOAD
  gchar *sk_lib_path;           /* will be deprecated */
  gint sk_cur_idx;
  gint sk_start_idx;
#endif                          /* XLNX_PCIe_PLATFORM */
#ifdef USE_XRM
  xrmContext xrm_ctx;
  xrmCuListResource *xrm_resources;
#endif                          /* USE_XRM */
#endif                          /* XLNX_PCIe_PLATFORM */
};

static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (GST_VIDEO_CAPS_MAKE ("{GRAY8, NV12, BGR, RGB, YUY2,"
        "r210, v308, GRAY10_LE32, ABGR, ARGB}")));

static GstStaticPadTemplate src_template = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (GST_VIDEO_CAPS_MAKE ("{GRAY8, NV12, BGR, RGB, YUY2,"
        "r210, v308, GRAY10_LE32, ABGR, ARGB}")));

#define gst_ivas_xfilter_parent_class parent_class
G_DEFINE_TYPE_WITH_PRIVATE (GstIvas_XFilter, gst_ivas_xfilter,
    GST_TYPE_BASE_TRANSFORM);
#define GST_IVAS_XFILTER_PRIVATE(self) (GstIvas_XFilterPrivate *) (gst_ivas_xfilter_get_instance_private (self))

void ivas_vframe_free (gpointer data);
int32_t ivas_buffer_alloc (IVASKernel * handle, IVASFrame * ivas_frame,
    void *data);
void ivas_buffer_free (IVASKernel * handle, IVASFrame * ivas_frame, void *data);
static void gst_ivas_xfilter_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_ivas_xfilter_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);
static GstFlowReturn gst_ivas_xfilter_transform_ip (GstBaseTransform * base,
    GstBuffer * outbuf);
static GstFlowReturn
gst_ivas_xfilter_transform (GstBaseTransform * base, GstBuffer * inbuf,
    GstBuffer * outbuf);
static void gst_ivas_xfilter_finalize (GObject * obj);

static Ivas_XFilterMode
get_kernel_mode (const gchar * mode)
{
  if (!g_strcmp0 ("passthrough", mode))
    return IVAS_ELEMENT_MODE_PASSTHROUGH;
  else if (!g_strcmp0 ("inplace", mode))
    return IVAS_ELEMENT_MODE_IN_PLACE;
  else if (!g_strcmp0 ("transform", mode))
    return IVAS_ELEMENT_MODE_TRANSFORM;
  else
    return IVAS_ELEMENT_MODE_NOT_SUPPORTED;
}

static inline IVASVideoFormat
get_kernellib_format (GstVideoFormat gst_fmt)
{
  switch (gst_fmt) {
    case GST_VIDEO_FORMAT_GRAY8:
      return IVAS_VFMT_Y8;
    case GST_VIDEO_FORMAT_NV12:
      return IVAS_VFMT_Y_UV8_420;
    case GST_VIDEO_FORMAT_BGR:
      return IVAS_VFMT_BGR8;
    case GST_VIDEO_FORMAT_RGB:
      return IVAS_VFMT_RGB8;
    case GST_VIDEO_FORMAT_YUY2:
      return IVAS_VFMT_YUYV8;
    case GST_VIDEO_FORMAT_r210:
      return IVAS_VFMT_RGBX10;
    case GST_VIDEO_FORMAT_v308:
      return IVAS_VFMT_YUV8;
    case GST_VIDEO_FORMAT_GRAY10_LE32:
      return IVAS_VFMT_Y10;
    case GST_VIDEO_FORMAT_ABGR:
      return IVAS_VFMT_ABGR8;
    case GST_VIDEO_FORMAT_ARGB:
      return IVAS_VFMT_ARGB8;
    default:
      GST_ERROR ("Not supporting %s yet", gst_video_format_to_string (gst_fmt));
      return IVAS_VMFT_UNKNOWN;
  }
}

static inline GstVideoFormat
get_gst_format (IVASVideoFormat kernel_fmt)
{
  switch (kernel_fmt) {
    case IVAS_VFMT_Y8:
      return GST_VIDEO_FORMAT_GRAY8;
    case IVAS_VFMT_Y_UV8_420:
      return GST_VIDEO_FORMAT_NV12;
    case IVAS_VFMT_BGR8:
      return GST_VIDEO_FORMAT_BGR;
    case IVAS_VFMT_RGB8:
      return GST_VIDEO_FORMAT_RGB;
    case IVAS_VFMT_YUYV8:
      return GST_VIDEO_FORMAT_YUY2;
    case IVAS_VFMT_RGBX10:
      return GST_VIDEO_FORMAT_r210;
    case IVAS_VFMT_YUV8:
      return GST_VIDEO_FORMAT_v308;
    case IVAS_VFMT_Y10:
      return GST_VIDEO_FORMAT_GRAY10_LE32;
    case IVAS_VFMT_ABGR8:
      return GST_VIDEO_FORMAT_ABGR;
    case IVAS_VFMT_ARGB8:
      return GST_VIDEO_FORMAT_ARGB;
    default:
      GST_ERROR ("Not supporting kernel format %d yet", kernel_fmt);
      return GST_VIDEO_FORMAT_UNKNOWN;
  }
}

void
ivas_vframe_free (gpointer data)
{
  GstVideoFrame *vframe = (GstVideoFrame *) data;

  if (vframe)
    gst_video_frame_unmap (vframe);

  g_free (data);
}

int32_t
ivas_buffer_alloc (IVASKernel * handle, IVASFrame * ivas_frame, void *data)
{
  GstIvas_XFilter *self = GST_IVAS_XFILTER (data);
  GstBuffer *outbuf = NULL;
  GstMemory *out_mem = NULL;
  GstFlowReturn fret;
  guint64 phy_addr;
  guint plane_id;
  GstVideoMeta *vmeta;
  GstVideoInfo out_info, max_info;
  gsize size_requested, max_size;
  GstBufferPool *priv_pool = NULL;
  int oidx;
  GstVideoFrame *vframe;

  if (!gst_video_info_set_format (&out_info,
          get_gst_format (ivas_frame->props.fmt), ivas_frame->props.width,
          ivas_frame->props.height)) {
    GST_ERROR_OBJECT (self, "failed to get videoinfo");
    return -1;
  }

  /* max video info from input width & height */
  // TODO: Better to take from out info if available
  if (!gst_video_info_set_format (&max_info,
          get_gst_format (ivas_frame->props.fmt),
          GST_VIDEO_INFO_WIDTH (self->priv->in_vinfo),
          GST_VIDEO_INFO_HEIGHT (self->priv->in_vinfo))) {
    GST_ERROR_OBJECT (self, "failed to get max videoinfo");
    return -1;
  }

  size_requested = GST_VIDEO_INFO_SIZE (&out_info);
  max_size = GST_VIDEO_INFO_SIZE (&max_info);

  if (size_requested > max_size) {
    GST_FIXME_OBJECT (self,
        "requested output buffer size %lu greater than max size %lu",
        size_requested, max_size);
    return -1;
  }

  oidx = 1;
  while (size_requested * MAX_PRIV_POOLS > oidx * max_size) {
    oidx++;
  }
  priv_pool = self->priv->priv_pools[oidx - 1];

  GST_DEBUG_OBJECT (self,
      "choosen outpool %p at index %d for requested buffer size %lu", priv_pool,
      oidx - 1, size_requested);

  if (priv_pool == NULL) {
    GstAllocator *allocator;
    GstCaps *caps;
    gsize pool_buf_size;
    gboolean bret;
    GstAllocationParams params =
        { GST_MEMORY_FLAG_PHYSICALLY_CONTIGUOUS, 0, 0, 0 };
    GstStructure *config;
    GstVideoInfo tmp_info;

    // TODO: taking width & height from input caps to allocate max size buffer pool
    // need to improve to take it from nearest pool
    caps = gst_caps_new_simple ("video/x-raw",
        "format", G_TYPE_STRING,
        gst_video_format_to_string (get_gst_format (ivas_frame->props.fmt)),
        "width", G_TYPE_INT, GST_VIDEO_INFO_WIDTH (self->priv->in_vinfo),
        "height", G_TYPE_INT,
        (oidx * GST_VIDEO_INFO_HEIGHT (self->priv->in_vinfo)) / MAX_PRIV_POOLS,
        NULL);

    if (!gst_video_info_from_caps (&tmp_info, caps))
      return -1;

    pool_buf_size = GST_VIDEO_INFO_SIZE (&tmp_info);

    priv_pool = gst_video_buffer_pool_new ();
    pool_buf_size = ALIGN (pool_buf_size, 4096);
    GST_INFO_OBJECT (self, "allocated internal private pool %p with size %lu",
        priv_pool, pool_buf_size);
    allocator = gst_ivas_allocator_new (DEFAULT_DEVICE_INDEX, TRUE);

    config = gst_buffer_pool_get_config (priv_pool);
    gst_buffer_pool_config_set_params (config, caps, pool_buf_size, 2, 0);
    gst_buffer_pool_config_set_allocator (config, allocator, &params);

    if (allocator)
      gst_object_unref (allocator);

    if (!gst_buffer_pool_set_config (priv_pool, config)) {
      GST_ERROR_OBJECT (self, "failed to configure  pool");
      return -1;
    }

    GST_INFO_OBJECT (self,
        "setting config %" GST_PTR_FORMAT " on private pool %" GST_PTR_FORMAT,
        config, priv_pool);

    bret = gst_buffer_pool_set_active (priv_pool, TRUE);
    if (!bret) {
      GST_ERROR_OBJECT (self, "failed to active private pool");
      return -1;
    }
    self->priv->priv_pools[oidx - 1] = priv_pool;
  }

  fret = gst_buffer_pool_acquire_buffer (priv_pool, &outbuf, NULL);
  if (fret != GST_FLOW_OK) {
    GST_ERROR_OBJECT (self, "failed to allocate buffer from pool %p",
        priv_pool);
    goto error;
  }
  GST_LOG_OBJECT (self, "acquired buffer %p from private pool %p", outbuf,
      priv_pool);

  gst_buffer_add_video_meta (outbuf, GST_VIDEO_FRAME_FLAG_NONE,
      get_gst_format (ivas_frame->props.fmt), ivas_frame->props.width,
      ivas_frame->props.height);

  out_mem = gst_buffer_get_memory (outbuf, 0);
  if (out_mem == NULL) {
    GST_ERROR_OBJECT (self, "failed to get memory from  buffer");
    goto error;
  }

  phy_addr = gst_ivas_allocator_get_paddr (out_mem);

  vmeta = gst_buffer_get_video_meta (outbuf);
  if (vmeta == NULL) {
    GST_ERROR_OBJECT (self, "video meta not present in buffer");
    goto error;
  }

  vframe = (GstVideoFrame *) g_malloc0 (sizeof (GstVideoFrame));
  if (NULL == vframe) {
    GST_ERROR_OBJECT (self, "failed to allocate memory");
    goto error;
  }

  /* map internal buffer in write mode */
  if (!gst_video_frame_map (vframe, &out_info, outbuf,
          (GstMapFlags) (GST_MAP_WRITE))) {
    GST_ERROR_OBJECT (self, "failed to map internal input buffer");
    goto error;
  }

  g_hash_table_insert (self->priv->priv_buf_vmaps, ivas_frame, vframe);

  // TODO: check whether we can use buffer pool memory size with requested size
  // return error when size > pool_mem_size

  for (plane_id = 0; plane_id < GST_VIDEO_INFO_N_PLANES (&out_info); plane_id++) {
    ivas_frame->paddr[plane_id] = phy_addr + vmeta->offset[plane_id];
    ivas_frame->vaddr[plane_id] = GST_VIDEO_FRAME_PLANE_DATA (vframe, plane_id);

    GST_LOG_OBJECT (self,
        "outbuf plane[%d] : paddr = %p, vaddr = %p, offset = %lu", plane_id,
        (void *) ivas_frame->paddr[plane_id], ivas_frame->vaddr[plane_id],
        vmeta->offset[plane_id]);
  }

  ivas_frame->app_priv = outbuf;

  GST_LOG_OBJECT (self, "associating ivasframe %p with outbuf %p", ivas_frame,
      outbuf);

  gst_memory_unref (out_mem);

  return 0;

error:
  if (out_mem)
    gst_memory_unref (out_mem);

  return -1;
}

void
ivas_buffer_free (IVASKernel * handle, IVASFrame * ivas_frame, void *data)
{
  GstIvas_XFilter *self = GST_IVAS_XFILTER (data);

  g_hash_table_remove (self->priv->priv_buf_vmaps, ivas_frame);

  if (ivas_frame->app_priv) {
    gst_buffer_unref ((GstBuffer *) ivas_frame->app_priv);
  }
  memset (ivas_frame, 0x0, sizeof (IVASFrame));
}

static gboolean
find_kernel_lib_symbols (GstIvas_XFilter * self, Ivas_XFilter * kernel)
{
  kernel->lib_fd = dlopen (kernel->ivas_lib_path, RTLD_LAZY);
  if (!kernel->lib_fd) {
    GST_ERROR_OBJECT (self, " unable to open shared library %s",
        kernel->ivas_lib_path);
    return FALSE;
  }

  GST_INFO_OBJECT (self,
      "opened kernel library %s successfully with fd %p",
      kernel->ivas_lib_path, kernel->lib_fd);

  /* Clear any existing error */
  dlerror ();

  kernel->kernel_init_func = (IVASKernelInit) dlsym (kernel->lib_fd,
      "xlnx_kernel_init");
  if (kernel->kernel_init_func == NULL) {
    GST_ERROR_OBJECT (self,
        "could not find ivas_xfilter_init function. reason : %s", dlerror ());
    return FALSE;
  }

  kernel->kernel_start_func = (IVASKernelStartFunc) dlsym (kernel->lib_fd,
      "xlnx_kernel_start");
  if (kernel->kernel_start_func == NULL) {
    GST_ERROR_OBJECT (self,
        "could not find ivas_xfilter_start function. reason : %s", dlerror ());
    return FALSE;
  }

  kernel->kernel_done_func = (IVASKernelDoneFunc) dlsym (kernel->lib_fd,
      "xlnx_kernel_done");
  if (kernel->kernel_done_func == NULL) {
    GST_ERROR_OBJECT (self,
        "could not find ivas_xfilter_done function. reason : %s", dlerror ());
    return FALSE;
  }

  kernel->kernel_deinit_func = (IVASKernelDeInit) dlsym (kernel->lib_fd,
      "xlnx_kernel_deinit");
  if (kernel->kernel_deinit_func == NULL) {
    GST_ERROR_OBJECT (self,
        "could not find ivas_xfilter_deinit function. reason : %s", dlerror ());
    return FALSE;
  }
  return TRUE;
}

static gboolean
ivas_xfilter_allocate_sink_internal_pool (GstIvas_XFilter * self)
{
  GstVideoInfo info;
  GstBufferPool *pool = NULL;
  GstStructure *config;
  GstAllocator *allocator = NULL;
  GstAllocationParams alloc_params;
  GstCaps *caps = NULL;

  caps = gst_pad_get_current_caps (GST_BASE_TRANSFORM (self)->sinkpad);

  if (!gst_video_info_from_caps (&info, caps)) {
    GST_WARNING_OBJECT (self, "Failed to parse caps %" GST_PTR_FORMAT, caps);
    gst_caps_unref (caps);
    return FALSE;
  }

  pool = gst_video_buffer_pool_new ();
  GST_LOG_OBJECT (self, "allocated internal sink pool %p", pool);

  allocator = gst_ivas_allocator_new (self->priv->dev_idx, USE_DMABUF);
  gst_allocation_params_init (&alloc_params);
  alloc_params.flags = GST_MEMORY_FLAG_PHYSICALLY_CONTIGUOUS;

  config = gst_buffer_pool_get_config (pool);
  gst_buffer_pool_config_set_params (config, caps, GST_VIDEO_INFO_SIZE (&info),
      3, 0);
  gst_buffer_pool_config_set_allocator (config, allocator, &alloc_params);
  gst_buffer_pool_config_add_option (config, GST_BUFFER_POOL_OPTION_VIDEO_META);

  if (!gst_buffer_pool_set_config (pool, config)) {
    GST_ERROR_OBJECT (self, "Failed to set config on input pool");
    goto error;
  }

  if (self->priv->input_pool)
    gst_object_unref (self->priv->input_pool);

  self->priv->input_pool = pool;

  GST_INFO_OBJECT (self, "allocated %" GST_PTR_FORMAT " pool",
      self->priv->input_pool);
  gst_caps_unref (caps);

  return TRUE;

error:
  gst_caps_unref (caps);
  return FALSE;
}

static gboolean
gst_ivas_xfilter_propose_allocation (GstBaseTransform * trans,
    GstQuery * decide_query, GstQuery * query)
{
  GstIvas_XFilter *self = GST_IVAS_XFILTER (trans);
  GstCaps *caps;
  GstVideoInfo info;
  GstBufferPool *pool;
  guint size;

  GST_BASE_TRANSFORM_CLASS (parent_class)->propose_allocation (trans,
      decide_query, query);

  gst_query_parse_allocation (query, &caps, NULL);

  if (caps == NULL)
    return FALSE;

  if (!gst_video_info_from_caps (&info, caps))
    return FALSE;

  size = GST_VIDEO_INFO_SIZE (&info);

  if (gst_query_get_n_allocation_pools (query) == 0) {
    GstStructure *structure;
    GstAllocator *allocator = NULL;
    GstAllocationParams params = { GST_MEMORY_FLAG_PHYSICALLY_CONTIGUOUS, 0, 0,
      0
    };

    if (gst_query_get_n_allocation_params (query) > 0) {
      gst_query_parse_nth_allocation_param (query, 0, &allocator, &params);
    } else {
      allocator = gst_ivas_allocator_new (self->priv->dev_idx, USE_DMABUF);
      gst_query_add_allocation_param (query, allocator, &params);
    }

    pool = gst_video_buffer_pool_new ();

    GST_LOG_OBJECT (self, "allocated internal pool %p", pool);

    structure = gst_buffer_pool_get_config (pool);
    gst_buffer_pool_config_set_params (structure, caps, size, MIN_POOL_BUFFERS,
        0);
    gst_buffer_pool_config_add_option (structure,
        GST_BUFFER_POOL_OPTION_VIDEO_META);
    gst_buffer_pool_config_set_allocator (structure, allocator, &params);

    if (!gst_buffer_pool_set_config (pool, structure))
      goto config_failed;

    GST_OBJECT_LOCK (self);
    gst_query_add_allocation_pool (query, pool, size, MIN_POOL_BUFFERS, 0);
    GST_OBJECT_UNLOCK (self);

    if (self->priv->input_pool)
      gst_object_unref (self->priv->input_pool);

    self->priv->input_pool = pool;
    gst_query_add_allocation_meta (query, GST_VIDEO_META_API_TYPE, NULL);

    GST_DEBUG_OBJECT (self, "prepared query %" GST_PTR_FORMAT, query);

    if (allocator)
      gst_object_unref (allocator);
  }

  return TRUE;

  /* ERRORS */
config_failed:
  {
    GST_ERROR_OBJECT (self, "failed to set config");
    gst_object_unref (pool);
    return FALSE;
  }
}

static gboolean
gst_ivas_xfilter_decide_allocation (GstBaseTransform * trans, GstQuery * query)
{
  GstIvas_XFilter *self = GST_IVAS_XFILTER (trans);
  GstAllocator *allocator = NULL;
  GstAllocationParams params;
  GstBufferPool *pool = NULL;
  guint size, min, max;
  GstCaps *outcaps;
  gboolean update_allocator;
  gboolean update_pool;
  GstStructure *config = NULL;

  gst_query_parse_allocation (query, &outcaps, NULL);

  /* we got configuration from our peer or the decide_allocation method,
   * parse them */
  if (gst_query_get_n_allocation_params (query) > 0) {
    /* try the allocator */
    gst_query_parse_nth_allocation_param (query, 0, &allocator, &params);
    update_allocator = TRUE;
  } else {
    allocator = NULL;
    update_allocator = FALSE;
    gst_allocation_params_init (&params);
  }

  if (gst_query_get_n_allocation_pools (query) > 0) {
    gst_query_parse_nth_allocation_pool (query, 0, &pool, &size, &min, &max);
    update_pool = TRUE;
  } else {
    GstVideoInfo info;

    if (!gst_video_info_from_caps (&info, outcaps))
      goto error;

    pool = NULL;
    min = 2;
    max = 0;
    size = info.size;

    update_pool = FALSE;
  }

  if (allocator) {
    if (!GST_IS_IVAS_ALLOCATOR (allocator)) {
      GST_DEBUG_OBJECT (self, "replace %" GST_PTR_FORMAT " to IVAS allocator",
          allocator);
      gst_object_unref (allocator);
      gst_allocation_params_init (&params);
      allocator = NULL;
    } else if (gst_ivas_allocator_get_device_idx (allocator) != self->priv->dev_idx) {
      GST_INFO_OBJECT (self, "downstream allocator (%d) and filter (%d) are on different devices",
          gst_ivas_allocator_get_device_idx (allocator), self->priv->dev_idx);
      gst_object_unref (allocator);
      gst_allocation_params_init (&params);
      allocator = NULL;
    }
  } else {
    gst_allocation_params_init (&params);
    allocator = NULL;
  }

  if (!allocator) {
    /* making sdx allocator for the HW mode without dmabuf */
    allocator = gst_ivas_allocator_new (self->priv->dev_idx, USE_DMABUF);
    //params.flags = GST_MEMORY_FLAG_PHYSICALLY_CONTIGUOUS;
    // TODO: Need to add XRT related flags here
  }

  if (update_allocator)
    gst_query_set_nth_allocation_param (query, 0, allocator, &params);
  else
    gst_query_add_allocation_param (query, allocator, &params);

  if (allocator)
    gst_object_unref (allocator);

  if (pool == NULL) {
    GST_DEBUG_OBJECT (self, "no pool, making new pool");
    pool = gst_video_buffer_pool_new ();
  }

  if (update_pool)
    gst_query_set_nth_allocation_pool (query, 0, pool, size, min, max);
  else
    gst_query_add_allocation_pool (query, pool, size, min, max);

  config = gst_buffer_pool_get_config (pool);
  gst_buffer_pool_config_set_params (config, outcaps, size, min, max);
  gst_buffer_pool_config_add_option (config, GST_BUFFER_POOL_OPTION_VIDEO_META);

  if (!gst_buffer_pool_set_config (pool, config)) {
   GST_ERROR_OBJECT (self, "failed to set config on own pool %p", pool);
   goto error;
  }

  gst_query_add_allocation_meta (query, GST_VIDEO_META_API_TYPE, NULL);
  gst_object_unref (pool);

  return GST_BASE_TRANSFORM_CLASS (parent_class)->decide_allocation (trans,
      query);

error:
  GST_ERROR_OBJECT (trans, "Invalid video caps %" GST_PTR_FORMAT, outcaps);

  if (allocator)
    gst_object_unref (allocator);
  if (pool)
    gst_object_unref (pool);

  return FALSE;
}

static gboolean
ivas_xfilter_open_device (GstIvas_XFilter * self)
{
  struct xclDeviceInfo2 deviceInfo;

  if (self->priv->dev_idx >= xclProbe ()) {
    GST_ERROR_OBJECT (self, "Cannot find device index %d", self->priv->dev_idx);
    goto error;
  }

  self->priv->xcl_handle = xclOpen (self->priv->dev_idx, NULL, XCL_INFO);
  if (!self->priv->xcl_handle) {
    GST_ERROR_OBJECT (self, "failed to open device index %u",
        self->priv->dev_idx);
    goto error;
  }

  if (xclGetDeviceInfo2 (self->priv->xcl_handle, &deviceInfo)) {
    GST_ERROR_OBJECT (self, "Unable to obtain device information");
    goto error;
  }

  GST_INFO_OBJECT (self, "DSA = %s, PCIe = GEN%ux%u, OCL Frequency = %u, "
      "DDR Bank = %u. Device Temp = %u, MIG Calibration = %u", deviceInfo.mName,
      deviceInfo.mPCIeLinkSpeed, deviceInfo.mPCIeLinkWidth,
      deviceInfo.mOCLFrequency[0], deviceInfo.mDDRBankCount,
      deviceInfo.mOnChipTemp, deviceInfo.mMigCalib);

  return TRUE;

error:
  return FALSE;
}

#if defined(XLNX_PCIe_PLATFORM) && defined(USE_XRM)
static int
calculate_load_percentage (GstIvas_XFilter * self, unsigned long long max_load)
{
  int cur_load_percent = 100;   /* 100 % */
  float fps = 0.0;

  if (max_load == 0) {
    GST_WARNING_OBJECT (self, "max_load not available. Requesting full load");
    return cur_load_percent;
  }

  fps =
      GST_VIDEO_INFO_FPS_N (self->priv->in_vinfo) /
      GST_VIDEO_INFO_FPS_D (self->priv->in_vinfo);
  cur_load_percent =
      (((GST_VIDEO_INFO_WIDTH (self->priv->in_vinfo) *
              GST_VIDEO_INFO_HEIGHT (self->priv->in_vinfo)) * fps) * 100) /
      max_load;

  return cur_load_percent;
}

static gboolean
ivas_xfilter_xrm_resource_alloc (GstIvas_XFilter * self)
{
  GstIvas_XFilterPrivate *priv = self->priv;
  int ret = -1;
  xrmCuListProperty sk_props;

  memset (&sk_props, 0, sizeof (xrmCuListProperty));

  /* One for soft kernel and one for actual VCU hardware instance */
  sk_props.cuNum = 2;

  /*create local XRM context */
  priv->xrm_ctx = xrmCreateContext (XRM_API_VERSION_1);
  if (priv->xrm_ctx == NULL) {
    GST_ERROR_OBJECT (self, "create local XRM context failed\n");
    return FALSE;
  }
  /* Fill default Values */
  strcpy (sk_props.cuProps[0].kernelName, "simple");
  sk_props.cuProps[0].devExcl = false;
  sk_props.cuProps[0].requestLoad = 100;

  /* Soft kernel information */
  strcpy (sk_props.cuProps[1].kernelName, priv->kernel->name);
  sk_props.cuProps[1].devExcl = false;
  sk_props.cuProps[1].requestLoad = 100;        // TODO: load can be taken from json file

  if (self->reservation_id) {
    GST_INFO_OBJECT (self, "Reservation id: %d received theorugh property",
        self->reservation_id);
  } else if (getenv ("XRM_RESERVE_ID")) {
    self->reservation_id = atoi (getenv ("XRM_RESERVE_ID"));
  }
  sk_props.cuProps[0].poolId = self->reservation_id;
  sk_props.cuProps[1].poolId = self->reservation_id;

  priv->xrm_resources =
      (xrmCuListResource *) calloc (1, sizeof (xrmCuListResource));
  if (!priv->xrm_resources) {
    GST_ERROR_OBJECT (self, "failed to allocate memory");
    return FALSE;
  }

  ret = xrmCuListAlloc (priv->xrm_ctx, &sk_props, priv->xrm_resources);
  if (ret != 0) {
    GST_ERROR_OBJECT (self,
        "xrm_allocation: failed to allocate cu list from reserve id %d",
        self->reservation_id);
    return FALSE;
  }

  GST_INFO_OBJECT (self, "allocated resource with reservation_id %d",
      self->reservation_id);

  priv->dev_idx = priv->xrm_resources->cuResources[0].deviceId;

  uuid_copy (priv->xclbinId, priv->xrm_resources->cuResources[0].uuid);

  priv->kernel->cu_idx = priv->xrm_resources->cuResources[1].cuId;

  GST_INFO_OBJECT (self, "res0 cu_idx = %d and res1 cu_idx = %d",
      priv->xrm_resources->cuResources[0].cuId,
      priv->xrm_resources->cuResources[1].cuId);

  GST_INFO_OBJECT (self, "chosen device %d and cu_idx %d",
      priv->xrm_resources->cuResources[0].deviceId, priv->kernel->cu_idx);

  return TRUE;
}
#endif

#ifndef USE_XRM
static gboolean
ivas_xfilter_download_xclbin (GstIvas_XFilter * self)
{
  char *bit = self->priv->xclbin_loc;
  FILE *fptr = NULL;
  int size = 0;
  char *header = NULL;
  const xclBin *blob = NULL;
  const struct axlf *top = NULL;

  if (!bit || !strlen (bit)) {
    GST_ERROR_OBJECT (self, "invalid xclbin location");
    goto error;
  }

  fptr = fopen (bit, "rb");
  if (!fptr) {
    GST_ERROR_OBJECT (self, "fopen() with %s failed due to %s", bit,
        strerror (errno));
    goto error;
  }

  if (fseek (fptr, 0, SEEK_END) != 0) {
    GST_ERROR_OBJECT (self, "fseek() failed to SEEK_END with %s",
        strerror (errno));
    goto error;
  }

  size = ftell (fptr);
  if (size == (size_t) (-1)) {
    GST_ERROR_OBJECT (self, "ftell failed with %s", strerror (errno));
    goto error;
  }

  if (fseek (fptr, 0, SEEK_SET) != 0) {
    GST_ERROR_OBJECT (self, "fseek() failed to SEEK_SET with %s",
        strerror (errno));
    goto error;
  }

  header = (char *) calloc (1, size);
  if (header == NULL) {
    GST_ERROR_OBJECT (self, "failed to allocate memory");
    goto error;
  }

  if (fread (header, sizeof (char), size, fptr) != size) {
    GST_ERROR_OBJECT (self, "fread failed with %s", strerror (errno));
    goto error;
  }

  if (strncmp (header, "xclbin2", 8)) {
    GST_ERROR_OBJECT (self, "Invalid bitstream xclbin2 tag not present");
    goto error;
  }

  blob = (const xclBin *) header;
  if (xclLoadXclBin (self->priv->xcl_handle, blob)) {
    GST_ERROR_OBJECT (self, "Bitstream download failed");
    goto error;
  }

  GST_INFO_OBJECT (self, "Finished downloading bitstream %s", bit);

  top = (const struct axlf *) header;
  uuid_copy (self->priv->xclbinId, top->m_header.uuid);

  free (header);
  fclose (fptr);
  return TRUE;

error:
  if (header)
    free (header);
  if (fptr)
    fclose (fptr);
  return FALSE;
}
#endif

static gboolean
ivas_xfilter_init (GstIvas_XFilter * self)
{
  GstIvas_XFilterPrivate *priv = self->priv;
  int iret;
  struct ert_start_kernel_cmd *ert_cmd = NULL;
  IVASKernel *ivas_handle = priv->kernel->ivas_handle;
  int i;

#ifndef USE_XRM /* XRM will download xclbin */
  if (priv->kernel->name || priv->kernel->is_softkernel) {
    /* xclbin need to be downloaded only when hardkernel or softkernel is used */
    if (!ivas_xfilter_download_xclbin (self))
      return FALSE;
  }
#endif

  if (priv->kernel->name) {
    //iret = xclIPName2Index (priv->xcl_handle, priv->kernel->name, &priv->kernel->cu_idx);
    priv->kernel->cu_idx =
        xclIPName2Index (priv->xcl_handle, priv->kernel->name);
    //xclCuName2Index (priv->xcl_handle, priv->kernel->name, &priv->kernel->cu_idx);
    if (priv->kernel->cu_idx < 0) {
      GST_ERROR_OBJECT (self, "failed to get cu index for IP name %s",
          priv->kernel->name);
      return FALSE;
    }

    GST_INFO_OBJECT (self, "cu_idx for kernel %s is %d", priv->kernel->name,
        priv->kernel->cu_idx);
  }

  if (priv->kernel->name || priv->kernel->is_softkernel) {
    if (xclOpenContext (priv->xcl_handle, priv->xclbinId, priv->kernel->cu_idx,
        true)) {
      GST_ERROR_OBJECT (self, "failed to do xclOpenContext...");
      return FALSE;
    }
  }

#if defined(XLNX_PCIe_PLATFORM) && defined (MANUAL_SOFTKERNEL_DOWNLOAD)
  if (priv->kernel->is_softkernel) {
    gboolean bret;
    char sk_lock_file[256];

    sprintf (sk_lock_file, "/tmp/%s.txt", priv->kernel->skinfo->skname);

    GST_LOG_OBJECT (self, "lock file for sk %s", sk_lock_file);

    bret =
        is_softkernel_downloaded (priv->xcl_handle, priv->dev_idx,
        sk_lock_file);
    if (!bret) {
      GST_DEBUG_OBJECT (self,
          "Going download kernel : sk lib path %s and sk name = %s",
          self->priv->sk_lib_path, priv->kernel->name);
      iret =
          download_softkernel (self->priv->sk_lib_path,
          priv->kernel->skinfo->skname, priv->xcl_handle, IVAS_SK_START_IDX,
          IVAS_SK_COUNT);
      if (iret) {
        GST_ERROR_OBJECT (self, "unable to download soft kernel %s (%s)",
            priv->kernel->skinfo->skname, self->priv->sk_lib_path);
        return FALSE;
      }
      GST_DEBUG_OBJECT (self,
          "Successfully downloaded soft kernel : sk lib path %s and sk name = %s",
          self->priv->sk_lib_path, priv->kernel->skinfo->skname);
    }
  }
#endif

  /* allocate ert command buffer */
  iret =
      alloc_xrt_buffer (priv->xcl_handle, ERT_CMD_SIZE, XCL_BO_SHARED_VIRTUAL,
      XCL_BO_FLAGS_EXECBUF, ivas_handle->ert_cmd_buf);
  if (iret < 0) {
    GST_ERROR_OBJECT (self, "failed to allocate ert command buffer..");
    return FALSE;
  }

  ert_cmd =
      (struct ert_start_kernel_cmd *) (ivas_handle->ert_cmd_buf->user_ptr);
  memset (ert_cmd, 0x0, ERT_CMD_SIZE);

  ivas_handle->xcl_handle = priv->xcl_handle;
#if defined(XLNX_PCIe_PLATFORM)
  ivas_handle->is_softkernel = priv->kernel->is_softkernel;
  if (priv->kernel->is_softkernel) {
    ivas_handle->cu_idx = priv->sk_cur_idx;
  } else {
    ivas_handle->cu_idx = priv->kernel->cu_idx;
  }
#else
  ivas_handle->cu_idx = priv->kernel->cu_idx;
#endif
  ivas_handle->kernel_config = priv->kernel->config;

  GST_INFO_OBJECT (self, "ivas library cu_idx = %d", ivas_handle->cu_idx);

  /* no need to do alloc in ivas library, so no callbacks required */
  ivas_handle->alloc_func = ivas_buffer_alloc;
  ivas_handle->free_func = ivas_buffer_free;
  ivas_handle->cb_user_data = self;

  pthread_mutex_lock (&count_mutex);    /* lock for TDM */
  iret = priv->kernel->kernel_init_func (ivas_handle);
  if (iret < 0) {
    GST_ERROR_OBJECT (self, "failed to do kernel init..");
    return FALSE;
  }
  pthread_mutex_unlock (&count_mutex);
  GST_INFO_OBJECT (self, "completed kernel init");

  memset (priv->kernel->input, 0x0, sizeof (IVASFrame *) * MAX_NUM_OBJECT);
  memset (priv->kernel->output, 0x0, sizeof (IVASFrame *) * MAX_NUM_OBJECT);

  priv->kernel->input[0] = (IVASFrame *) calloc (1, sizeof (IVASFrame));
  if (NULL == priv->kernel->input[0]) {
    GST_ERROR_OBJECT (self, "failed to allocate memory");
    return FALSE;
  }

  if (priv->element_mode == IVAS_ELEMENT_MODE_TRANSFORM) {
    priv->kernel->output[0] = (IVASFrame *) calloc (1, sizeof (IVASFrame));
    if (NULL == priv->kernel->output[0]) {
      GST_ERROR_OBJECT (self, "failed to allocate memory");
      return FALSE;
    }
  }

  for (i = 0; i < MAX_PRIV_POOLS; i++)
    priv->priv_pools[i] = NULL;

  return TRUE;
}

static gboolean
ivas_xfilter_deinit (GstIvas_XFilter * self)
{
  GstIvas_XFilterPrivate *priv = self->priv;
  int iret, i;
  guint cu_idx = -1;;

  if (priv->kernel) {
    if (priv->kernel->input[0])
      free (priv->kernel->input[0]);

    if (priv->kernel->output[0])
      free (priv->kernel->output[0]);

    cu_idx = priv->kernel->cu_idx;

    if (priv->kernel->kernel_deinit_func) {
      iret = priv->kernel->kernel_deinit_func (priv->kernel->ivas_handle);
      if (iret < 0) {
        GST_ERROR_OBJECT (self, "failed to do kernel deinit..");
      }
      GST_DEBUG_OBJECT (self, "successfully completed deinit");
    }

    if (priv->kernel->ivas_handle) {
      if (priv->kernel->ivas_handle->ert_cmd_buf) {
        free_xrt_buffer (priv->xcl_handle,
            priv->kernel->ivas_handle->ert_cmd_buf);
        free (priv->kernel->ivas_handle->ert_cmd_buf);
      }
      free (priv->kernel->ivas_handle);
    }
#ifdef XLNX_PCIe_PLATFORM
    if (priv->kernel->skinfo) {
      if (priv->kernel->skinfo->skname) {
        g_free (priv->kernel->skinfo->skname);
        priv->kernel->skinfo->skname = NULL;
      }
      free (priv->kernel->skinfo);
    }
#endif

    if (priv->kernel->config)
      json_decref (priv->kernel->config);

    if (priv->kernel->ivas_lib_path)
      g_free (priv->kernel->ivas_lib_path);

    if (priv->kernel->name)
      free (priv->kernel->name);

    free (priv->kernel);
    priv->kernel = NULL;
  }

  for (i = 0; i < MAX_PRIV_POOLS; i++) {
    if (priv->priv_pools[i]) {
      gst_buffer_pool_set_active (priv->priv_pools[i], FALSE);
      gst_object_unref (priv->priv_pools[i]);
      priv->priv_pools[i] = NULL;
    }
  }

  if (priv->xclbin_loc)
    free (priv->xclbin_loc);

  if (priv->xcl_handle) {
    if (cu_idx <= 0)
      xclCloseContext (priv->xcl_handle, priv->xclbinId, cu_idx);
    xclClose (priv->xcl_handle);
  }
#if defined(XLNX_PCIe_PLATFORM) && defined (USE_XRM)
  if (priv->xrm_resources) {
    xrmCuListRelease (priv->xrm_ctx, priv->xrm_resources);
    priv->xrm_resources = NULL;
  }

  if (priv->xrm_ctx) {
    xrmDestroyContext (priv->xrm_ctx);
    priv->xrm_ctx = NULL;
  }
#endif
  return TRUE;
}

static gboolean
gst_ivas_xfilter_start (GstBaseTransform * trans)
{
  GstIvas_XFilter *self = GST_IVAS_XFILTER (trans);
  GstIvas_XFilterPrivate *priv = self->priv;
  json_t *root = NULL, *karray, *kernel, *value;
  json_error_t error;
  gchar *lib_path = NULL;
  guint kernel_count;
  IVASKernel *ivas_handle;

  /* get root json object */
  root = json_load_file (self->json_file, JSON_DECODE_ANY, &error);
  if (!root) {
    GST_ERROR_OBJECT (self, "failed to load json file. reason %s", error.text);
    goto error;
  }

  /* get xclbin location */
  // TODO: xclbin-loc is optional as XRM might have downloaded
  value = json_object_get (root, "xclbin-location");
  if (json_is_string (value)) {
    priv->xclbin_loc = g_strdup (json_string_value (value));
    GST_INFO_OBJECT (self, "xclbin location to download %s", priv->xclbin_loc);
  } else {
    priv->xclbin_loc = NULL;
    GST_INFO_OBJECT (self, "xclbin path is not set");
  }

  /* get IVAS library repository path */
  value = json_object_get (root, "ivas-library-repo");
  if (!value) {
    GST_DEBUG_OBJECT (self,
        "library repo path does not exist.taking default %s",
        DEFAULT_IVAS_LIB_PATH);
    lib_path = g_strdup (DEFAULT_IVAS_LIB_PATH);
  } else {
    gchar *path = g_strdup (json_string_value (value));

    if (!g_str_has_suffix (path, "/")) {
      lib_path = g_strconcat (path, "/", NULL);
      g_free (path);
    } else {
      lib_path = path;
    }
  }

  /* get kernel mode : passthrough/inplace/transform */
  value = json_object_get (root, "element-mode");
  if (!json_is_string (value)) {
    GST_ERROR_OBJECT (self,
        "\"kernel-mode\" not set. possible values (passthrough/inplace/transform)");
    goto error;
  }

  GST_INFO_OBJECT (self, "element is going to operate in %s",
      json_string_value (value));
  priv->element_mode = get_kernel_mode (json_string_value (value));

  gst_base_transform_set_passthrough (trans,
      priv->element_mode == IVAS_ELEMENT_MODE_PASSTHROUGH);
  gst_base_transform_set_in_place (trans,
      priv->element_mode == IVAS_ELEMENT_MODE_IN_PLACE);

  /* get kernels array */
  karray = json_object_get (root, "kernels");
  if (!karray) {
    GST_ERROR_OBJECT (self, "failed to find key kernels");
    goto error;
  }

  if (!json_is_array (karray)) {
    GST_ERROR_OBJECT (self, "kernels key is not of array type");
    goto error;
  }

  kernel_count = json_array_size (karray);
  if (kernel_count > 1) {
    GST_WARNING_OBJECT (self,
        "number of kernels > %d not supported. taking first one only",
        kernel_count);
  }

  kernel = json_array_get (karray, 0);
  if (!kernel) {
    GST_ERROR_OBJECT (self, "failed to get kernel object");
    goto error;
  }

  priv->kernel = (Ivas_XFilter *) calloc (1, sizeof (Ivas_XFilter));
  if (!priv->kernel) {
    GST_ERROR_OBJECT (self, "failed to allocate memory");
    goto error;
  }
  priv->kernel->is_softkernel = FALSE;

  value = json_object_get (kernel, "library-name");
  if (!json_is_string (value)) {
    GST_ERROR_OBJECT (self, "library name is not of string type");
    goto error;
  }

  /* absolute path IVAS library */
  priv->kernel->ivas_lib_path =
      g_strconcat (lib_path, json_string_value (value), NULL);
  GST_DEBUG_OBJECT (self, "ivas library path %s", priv->kernel->ivas_lib_path);

  /* get kernel name */
  value = json_object_get (kernel, "kernel-name");
  if (value) {
    if (!json_is_string (value)) {
      GST_ERROR_OBJECT (self, "primary kernel name is not of string type");
      goto error;
    }
    priv->kernel->name = g_strdup (json_string_value (value));
  } else {
    priv->kernel->name = NULL;
  }
  GST_INFO_OBJECT (self, "Primary kernel name %s", priv->kernel->name);

#ifdef XLNX_PCIe_PLATFORM
  /* softkernels exist in PCIe based platforms only */
  value = json_object_get (kernel, "softkernel");
  if (json_is_object (value)) {
    json_t *skernel = value;

    value = json_object_get (skernel, "name");
    if (!json_is_string (value)) {
      GST_ERROR_OBJECT (self, "softkernel name is not of string type");
      goto error;
    }

    priv->kernel->skinfo =
        (IvasSoftKernelInfo *) calloc (1, sizeof (IvasSoftKernelInfo));
    if (!priv->kernel->skinfo) {
      GST_ERROR_OBJECT (self, "failed to allocate memory");
      goto error;
    }

    priv->kernel->skinfo->skname = g_strdup (json_string_value (value));
    GST_INFO_OBJECT (self, "received kernel is a softkernel with name %s",
        priv->kernel->skinfo->skname);
    priv->kernel->is_softkernel = TRUE;
  }
#endif

  /* get ivas kernel lib internal configuration */
  value = json_object_get (kernel, "config");
  if (!json_is_object (value)) {
    GST_ERROR_OBJECT (self, "config is not of object type");
    goto error;
  }

  priv->kernel->config = json_deep_copy (value);
  GST_DEBUG_OBJECT (self, "kernel config size = %lu", json_object_size (value));

  /* find whether required symbols present or not */
  if (!find_kernel_lib_symbols (self, priv->kernel)) {
    GST_ERROR_OBJECT (self, "failed find symbols in kernel lib...");
    goto error;
  }

  ivas_handle = (IVASKernel *) calloc (1, sizeof (IVASKernel));
  if (!ivas_handle) {
    GST_ERROR_OBJECT (self, "failed to allocate memory");
    goto error;
  }

  ivas_handle->ert_cmd_buf = (xrt_buffer *) calloc (1, sizeof (xrt_buffer));
  if (ivas_handle->ert_cmd_buf == NULL) {
    GST_ERROR_OBJECT (self, "failed to allocate ert cmd memory");
    goto error;
  }

  priv->kernel->ivas_handle = ivas_handle;

  if (priv->do_init) {          /*TODO: why it is protected under do_init */
    if (!ivas_xfilter_open_device (self))
      goto error;

    if (!ivas_xfilter_init (self))
      goto error;

    priv->do_init = FALSE;
  }

  return TRUE;

error:
  if (lib_path)
    g_free (lib_path);
  if (root)
    json_decref (root);
  return FALSE;
}

static gboolean
gst_ivas_xfilter_stop (GstBaseTransform * trans)
{
  GstIvas_XFilter *self = GST_IVAS_XFILTER (trans);

  GST_DEBUG_OBJECT (self, "stopping");
  ivas_xfilter_deinit (self);
  return TRUE;
}

static GstCaps *
ivas_kernelcap_to_gst_cap (kernelcaps * kcap)
{
  GValue list = { 0, };
  GValue aval = { 0, };

  GstCaps *cap = gst_caps_new_empty ();
  GstStructure *s;              // = gst_structure_new ("video/x-raw", NULL);

  if (kcap == NULL)
    return cap;

  g_value_init (&list, GST_TYPE_LIST);
  g_value_init (&aval, G_TYPE_STRING);

  if (kcap->range_height == true) {
    s = gst_structure_new ("video/x-raw", "height", GST_TYPE_INT_RANGE,
        kcap->lower_height, kcap->upper_height, NULL);
  } else {
    s = gst_structure_new ("video/x-raw", "height", G_TYPE_INT,
        kcap->lower_height, NULL);
  }

  if (kcap->range_width == true) {
    gst_structure_set (s, "width", GST_TYPE_INT_RANGE, kcap->lower_width,
        kcap->upper_width, NULL);
  } else {
    gst_structure_set (s, "width", G_TYPE_INT, kcap->lower_width, NULL);
  }

  for (uint8_t i = 0; i < kcap->num_fmt; i++) {
    const char *fourcc =
        gst_video_format_to_string (get_gst_format (kcap->fmt[i]));

    g_value_set_string (&aval, fourcc);
    gst_value_list_append_value (&list, &aval);
    gst_structure_set_value (s, "format", &list);

    g_value_reset (&aval);
  }

  gst_caps_append_structure (cap, s);

  g_value_reset (&aval);
  g_value_unset (&list);
  return cap;
}

static gboolean
gst_ivas_xfilter_query (GstBaseTransform * trans,
    GstPadDirection direction, GstQuery * query)
{
  GstIvas_XFilter *self = GST_IVAS_XFILTER (trans);
  Ivas_XFilter *kernel = self->priv->kernel;
  Ivas_XFilterMode element_mode = self->priv->element_mode;
  IVASKernel *ivas_handle = NULL;
  kernelpads **kernel_pads, *kernel_pad;
  uint8_t nu_pads;              /* number of sink/src pads suported by kenrel */
  uint8_t pad_index;            /* incomming pad index */
  uint8_t nu_caps;              /* number of caps supported by one pad */
  kernelcaps **kcaps;
  gboolean ret = TRUE;

  switch (GST_QUERY_TYPE (query)) {
    case GST_QUERY_CAPS:{
      GstCaps *newcap, *allcaps;

      if (self->priv->do_init == TRUE)
        return FALSE;

      ivas_handle = kernel->ivas_handle;

      if (!ivas_handle->padinfo ||
          ivas_handle->padinfo->nature == IVAS_PAD_DEFAULT) {
        /* Do default handling */
        GST_DEBUG_OBJECT (self,
            "padinfo == NULL || nature == IVAS_PAD_DEFAULT, Do default handling");
        break;
      }

      if (element_mode == IVAS_ELEMENT_MODE_PASSTHROUGH
          || element_mode == IVAS_ELEMENT_MODE_IN_PLACE) {
        /* Same buffer for sink and src,
         * so the same caps for sink and src pads
         * and for all pads
         */
        kernel_pads = ivas_handle->padinfo->sinkpads;
        nu_pads = (direction == GST_PAD_SRC) ?
            ivas_handle->padinfo->nu_srcpad : ivas_handle->padinfo->nu_sinkpad;
      } else {
        kernel_pads = (direction == GST_PAD_SRC) ?
            ivas_handle->padinfo->srcpads : ivas_handle->padinfo->sinkpads;
        nu_pads = (direction == GST_PAD_SRC) ?
            ivas_handle->padinfo->nu_srcpad : ivas_handle->padinfo->nu_sinkpad;
      }
      GST_INFO_OBJECT (self, "element_mode %d nu_pads %d %s", element_mode,
          nu_pads, direction == GST_PAD_SRC ? "SRC" : "SINK");

      pad_index = 0;            /* TODO: how to get incoming pad number */
      kernel_pad = (kernel_pads[pad_index]);
      nu_caps = kernel_pad->nu_caps;
      kcaps = kernel_pad->kcaps;        /* Base of pad's caps */
      GST_DEBUG_OBJECT (self, "nu_caps = %d", nu_caps);


      allcaps = gst_caps_new_empty ();
      /* 0th element has high priority */
      for (uint8_t i = 0; i < nu_caps; i++) {
        kernelcaps *kcap = (kcaps[i]);

        newcap = ivas_kernelcap_to_gst_cap (kcap);
        gst_caps_append (allcaps, newcap);
      }

      if ((ivas_handle->padinfo->nature != IVAS_PAD_RIGID)) {
        GstCaps *padcaps;
        GST_DEBUG_OBJECT (self, "nature != IVAS_PAD_RIGID");

        if (direction == GST_PAD_SRC) {
          padcaps = gst_pad_get_pad_template_caps (trans->srcpad);
        } else {
          padcaps = gst_pad_get_pad_template_caps (trans->sinkpad);
        }

        gst_caps_append (allcaps, padcaps);
      }

      {
        gchar *str = gst_caps_to_string (allcaps);
        GST_INFO_OBJECT (self, "caps from kernel = %s", str);
        g_free (str);
      }

      gst_query_set_caps_result (query, allcaps);
      gst_caps_unref (allcaps);

      return TRUE;
    }
    default:
      ret = TRUE;
      break;
  }

  GST_BASE_TRANSFORM_CLASS (parent_class)->query (trans, direction, query);
  return ret;
}

static gboolean
gst_ivas_xfilter_set_caps (GstBaseTransform * trans, GstCaps * incaps,
    GstCaps * outcaps)
{
  GstIvas_XFilter *self = GST_IVAS_XFILTER (trans);
  gboolean bret = TRUE;
  GstIvas_XFilterPrivate *priv = self->priv;

  GST_INFO_OBJECT (self,
      "incaps = %" GST_PTR_FORMAT "and outcaps = %" GST_PTR_FORMAT, incaps,
      outcaps);

  if (!gst_video_info_from_caps (priv->in_vinfo, incaps)) {
    GST_ERROR_OBJECT (self, "Failed to parse input caps");
    return FALSE;
  }

  if (!gst_video_info_from_caps (priv->out_vinfo, outcaps)) {
    GST_ERROR_OBJECT (self, "Failed to parse output caps");
    return FALSE;
  }
#if defined(XLNX_PCIe_PLATFORM) && defined (USE_XRM)
  // TODO: If existing caps not equal to new caps, need to re-allocate XRM resources
  if (!ivas_xfilter_xrm_resource_alloc (self))
    return FALSE;
#endif

  return bret;
}

static void
gst_ivas_xfilter_class_init (GstIvas_XFilterClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;
  GstBaseTransformClass *transform_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gstelement_class = GST_ELEMENT_CLASS (klass);
  transform_class = GST_BASE_TRANSFORM_CLASS (klass);

  gobject_class->set_property = gst_ivas_xfilter_set_property;
  gobject_class->get_property = gst_ivas_xfilter_get_property;
  gobject_class->finalize = gst_ivas_xfilter_finalize;

  transform_class->start = gst_ivas_xfilter_start;
  transform_class->stop = gst_ivas_xfilter_stop;
  transform_class->set_caps = gst_ivas_xfilter_set_caps;
  transform_class->query = gst_ivas_xfilter_query;
  transform_class->decide_allocation = gst_ivas_xfilter_decide_allocation;
  transform_class->propose_allocation = gst_ivas_xfilter_propose_allocation;
  transform_class->transform_ip = gst_ivas_xfilter_transform_ip;
  transform_class->transform = gst_ivas_xfilter_transform;

  g_object_class_install_property (gobject_class, PROP_CONFIG_LOCATION,
      g_param_spec_string ("kernels-config",
          "IVAS kernels json config file location",
          "Location of the kernels config file in json format", NULL,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
              GST_PARAM_MUTABLE_READY)));

  g_object_class_install_property (gobject_class, PROP_DYNAMIC_CONFIG,
      g_param_spec_string ("dynamic-config",
          "Kernel's dynamic json config string",
          "String contains dynamic json configuration of kernel", NULL,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));

#if defined(XLNX_PCIe_PLATFORM)
#if defined (MANUAL_SOFTKERNEL_DOWNLOAD)
  g_object_class_install_property (gobject_class, PROP_SK_LIB_PATH,
      g_param_spec_string ("sk-lib-path", "softkernel library path",
          "Path of softkernel library to download to device (will be deprecated after XRM integration)",
          NULL,
          G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS | GST_PARAM_MUTABLE_READY));

  g_object_class_install_property (gobject_class, PROP_SK_CURRENT_INDEX,
      g_param_spec_int ("sk-cur-idx", "Current softkernel index",
          "Current softkernel index", 0, 31, 0,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_SK_START_INDEX,
      g_param_spec_int ("sk-start-idx", " softkernel start index",
          "softkernel start index", 0, 31, 0,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_DEVICE_INDEX,
      g_param_spec_int ("dev-idx", "Device index",
          "Index of the device on which IP/kernel resides", 0, 31, 0,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

#elif define (USE_XRM)
  g_object_class_install_property (gobject_class, PROP_RESERVATION_ID,
      g_param_spec_int ("reservation-id", "Resource reservation id",
          "Reservation id provided by XRM", 0, 1024, 0,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));
#endif
#endif

  gst_element_class_set_details_simple (gstelement_class,
      "IVAS Generic Filter Plugin",
      "Filter/Effect/Video",
      "Performs operations on HW IP/SW IP/Softkernel using IVAS library APIs",
      "Xilinx Inc <www.xilinx.com>");

  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&src_template));
  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&sink_template));

  /*
   * Will be emitted when kernel is sucessfully done.
   */
  ivas_signals[SIGNAL_IVAS] =
      g_signal_new ("ivas-kernel-done", G_TYPE_FROM_CLASS (klass),
      G_SIGNAL_RUN_LAST, 0,
      NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0, G_TYPE_NONE);

  GST_DEBUG_CATEGORY_INIT (gst_ivas_xfilter_debug, "ivas_xfilter", 0,
      "IVAS Generic Filter plugin");
  GST_DEBUG_CATEGORY_GET (GST_CAT_PERFORMANCE, "GST_PERFORMANCE");
}

/* initialize the new element
 * initialize instance structure
 */
static void
gst_ivas_xfilter_init (GstIvas_XFilter * self)
{
  GstIvas_XFilterPrivate *priv = GST_IVAS_XFILTER_PRIVATE (self);

  self->priv = priv;
  priv->dev_idx = DEFAULT_DEVICE_INDEX;
  priv->ert_cmd_buf = NULL;
  priv->in_vinfo = gst_video_info_new ();
  priv->out_vinfo = gst_video_info_new ();
  priv->element_mode = IVAS_ELEMENT_MODE_NOT_SUPPORTED;
  priv->do_init = TRUE;
  priv->dyn_json_config = NULL;
  // TODO: if vaddr is not required remove priv_buf_vmaps
  priv->priv_buf_vmaps = g_hash_table_new_full (g_direct_hash, g_direct_equal,
      NULL, ivas_vframe_free);
#if defined(XLNX_PCIe_PLATFORM) && defined (MANUAL_SOFTKERNEL_DOWNLOAD)
  priv->sk_lib_path = NULL;
  priv->sk_cur_idx = -1;
  priv->sk_start_idx = 0;
#endif
}

static void
gst_ivas_xfilter_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstIvas_XFilter *self = GST_IVAS_XFILTER (object);

  switch (prop_id) {
#if defined(XLNX_PCIe_PLATFORM) && defined (USE_XRM)
    case PROP_RESERVATION_ID:
      self->reservation_id = g_value_get_int (value);
      break;
#endif
    case PROP_CONFIG_LOCATION:
      if (GST_STATE (self) != GST_STATE_NULL) {
        g_warning
            ("can't set json_file path when instance is NOT in NULL state");
        return;
      }
      self->json_file = g_value_dup_string (value);
      break;
    case PROP_DYNAMIC_CONFIG:
      self->dyn_config = g_value_dup_string (value);
      if (self->priv->dyn_json_config) {
        json_decref (self->priv->dyn_json_config);
      }
      self->priv->dyn_json_config = json_loads (self->dyn_config, JSON_DECODE_ANY, NULL);
      break;
#if defined(XLNX_PCIe_PLATFORM) && defined (MANUAL_SOFTKERNEL_DOWNLOAD)
    case PROP_SK_LIB_PATH:
      self->priv->sk_lib_path = g_value_dup_string (value);
      break;
    case PROP_SK_CURRENT_INDEX:
      self->priv->sk_cur_idx = g_value_get_int (value);
      break;
    case PROP_SK_START_INDEX:
      self->priv->sk_start_idx = g_value_get_int (value);
      break;
    case PROP_DEVICE_INDEX:
      self->priv->dev_idx = g_value_get_int (value);
      break;
#endif
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_ivas_xfilter_get_property (GObject * object, guint prop_id, GValue * value,
    GParamSpec * pspec)
{
  GstIvas_XFilter *self = GST_IVAS_XFILTER (object);

  switch (prop_id) {
#if defined(XLNX_PCIe_PLATFORM) && defined (USE_XRM)
    case PROP_RESERVATION_ID:
      g_value_set_int (value, self->reservation_id);
      break;
#endif
    case PROP_CONFIG_LOCATION:
      g_value_set_string (value, self->json_file);
      break;
    case PROP_DYNAMIC_CONFIG:
      g_value_set_string (value, self->dyn_config);
      break;
#if defined(XLNX_PCIe_PLATFORM) && defined (MANUAL_SOFTKERNEL_DOWNLOAD)
    case PROP_SK_CURRENT_INDEX:
      g_value_set_int (value, self->priv->sk_cur_idx);
      break;
    case PROP_SK_START_INDEX:
      g_value_set_int (value, self->priv->sk_start_idx);
      break;
    case PROP_DEVICE_INDEX:
      g_value_set_int (value, self->priv->dev_idx);
      break;
#endif
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_ivas_xfilter_finalize (GObject * obj)
{
  GstIvas_XFilter *self = GST_IVAS_XFILTER (obj);

  gst_video_info_free (self->priv->in_vinfo);
  gst_video_info_free (self->priv->out_vinfo);

  if (self->priv->input_pool)
    gst_object_unref (self->priv->input_pool);

  g_hash_table_unref (self->priv->priv_buf_vmaps);

#if defined(XLNX_PCIe_PLATFORM) && defined (MANUAL_SOFTKERNEL_DOWNLOAD)
  g_free (self->priv->sk_lib_path);
#endif
  g_free (self->json_file);
}

static gboolean
ivas_xfilter_copy_input_buffer (GstIvas_XFilter * self, GstBuffer * inbuf,
    GstBuffer ** internal_inbuf)
{
  GstBuffer *new_inbuf;
  GstFlowReturn fret;
  GstVideoFrame in_vframe = { 0, }, new_vframe = {
  0,};
  gboolean bret;

  if (!self->priv->input_pool) {
    /* allocate input internal pool */
    bret = ivas_xfilter_allocate_sink_internal_pool (self);
    if (!bret)
      goto error;

    if (!gst_buffer_pool_is_active (self->priv->input_pool))
      gst_buffer_pool_set_active (self->priv->input_pool, TRUE);
  }

  /* acquire buffer from own input pool */
  fret =
      gst_buffer_pool_acquire_buffer (self->priv->input_pool, &new_inbuf, NULL);
  if (fret != GST_FLOW_OK) {
    GST_ERROR_OBJECT (self, "failed to allocate buffer from pool %p",
        self->priv->input_pool);
    goto error;
  }
  GST_LOG_OBJECT (self, "acquired buffer %p from own pool", new_inbuf);

  /* map internal buffer in write mode */
  if (!gst_video_frame_map (&new_vframe, self->priv->in_vinfo, new_inbuf,
          GST_MAP_WRITE)) {
    GST_ERROR_OBJECT (self, "failed to map internal input buffer");
    goto error;
  }

  /* map input buffer in read mode */
  if (!gst_video_frame_map (&in_vframe, self->priv->in_vinfo, inbuf,
          GST_MAP_READ)) {
    GST_ERROR_OBJECT (self, "failed to map input buffer");
    goto error;
  }
  GST_CAT_LOG_OBJECT (GST_CAT_PERFORMANCE, self,
      "slow copy to internal input pool buffer");

  gst_video_frame_copy (&new_vframe, &in_vframe);
  gst_video_frame_unmap (&in_vframe);
  gst_video_frame_unmap (&new_vframe);
  gst_buffer_copy_into (new_inbuf, inbuf,
      (GstBufferCopyFlags) GST_BUFFER_COPY_METADATA, 0, -1);
  *internal_inbuf = new_inbuf;

  return TRUE;

error:
  if (in_vframe.data[0]) {
    gst_video_frame_unmap (&in_vframe);
  }
  if (new_vframe.data[0]) {
    gst_video_frame_unmap (&new_vframe);
  }
  return FALSE;
}

static gboolean
ivas_xfilter_prepare_input_frame (GstIvas_XFilter * self, GstBuffer * inbuf,
    GstBuffer ** new_inbuf)
{
  GstIvas_XFilterPrivate *priv = self->priv;
  IVASFrame *ivas_frame = NULL;
  guint64 phy_addr = 0;
  guint plane_id;
  gboolean bret = FALSE;
  GstVideoMeta *vmeta = NULL;
  GstMemory *in_mem = NULL;
  GstMapFlags map_flags;

  ivas_frame = priv->kernel->input[0];

  if (priv->kernel->name || priv->kernel->is_softkernel) { /*HW IP/softkernel */
    in_mem = gst_buffer_get_memory (inbuf, 0);
    if (in_mem == NULL) {
      GST_ERROR_OBJECT (self, "failed to get memory from input buffer");
      goto error;
    }

    GST_LOG_OBJECT (self, "getting physical address for kernel library");

    if (gst_is_ivas_memory (in_mem)
        && gst_ivas_memory_can_avoid_copy (in_mem, priv->dev_idx)) {
      phy_addr = gst_ivas_allocator_get_paddr (in_mem);
    } else if (gst_is_dmabuf_memory (in_mem)) {
      guint bo = NULLBO;
      gint dma_fd = -1;
      struct xclBOProperties p;

      dma_fd = gst_dmabuf_memory_get_fd (in_mem);
      if (dma_fd < 0) {
        GST_ERROR_OBJECT (self, "failed to get DMABUF FD");
        goto error;
      }

      /* dmabuf but not from ivas allocator */
      bo = xclImportBO (priv->xcl_handle, dma_fd, 0);
      if (bo == NULLBO) {
        GST_WARNING_OBJECT (self,
            "failed to get XRT BO...fall back to copy input");
      }

      GST_DEBUG_OBJECT (self, "received dma fd %d and its xrt BO = %u", dma_fd,
          bo);

      if (!xclGetBOProperties (priv->xcl_handle, bo, &p)) {
        phy_addr = p.paddr;
      } else {
        GST_WARNING_OBJECT (self,
            "failed to get physical address...fall back to copy input");
      }
    }

    if (!phy_addr) {
      GST_DEBUG_OBJECT (self,
          "could not get phy_addr, copy input buffer to internal pool buffer");
      bret = ivas_xfilter_copy_input_buffer (self, inbuf, new_inbuf);
      if (!bret)
        goto error;

      gst_memory_unref (in_mem);

      in_mem = gst_buffer_get_memory (*new_inbuf, 0);
      if (in_mem == NULL) {
        GST_ERROR_OBJECT (self, "failed to get memory from input buffer");
        goto error;
      }

      phy_addr = gst_ivas_allocator_get_paddr (in_mem);
      if (!phy_addr) {
        GST_ERROR_OBJECT (self, "failed to get physical address");
        goto error;
      }
      inbuf = *new_inbuf;
    }
#ifdef XLNX_PCIe_PLATFORM
    /* syncs data when IVAS_SYNC_TO_DEVICE flag is enabled */
    bret = gst_ivas_memory_sync_bo (in_mem);
    if (!bret)
      goto error;
#endif

    GST_LOG_OBJECT (self, "input paddr %p", (void *) phy_addr);

    gst_memory_unref (in_mem);
    in_mem = NULL;
  } else { /* Soft IP */
    map_flags = GST_MAP_READ | GST_VIDEO_FRAME_MAP_FLAG_NO_REF;

    if (priv->element_mode == IVAS_ELEMENT_MODE_IN_PLACE)
      map_flags = map_flags | GST_MAP_WRITE;

    if (!gst_video_frame_map (&(priv->kernel->in_vframe), self->priv->in_vinfo,
          inbuf, map_flags)) {
      GST_ERROR_OBJECT (self, "failed to map input buffer");
      goto error;
    }

    for (plane_id = 0;
        plane_id < GST_VIDEO_INFO_N_PLANES (self->priv->in_vinfo); plane_id++) {
      ivas_frame->vaddr[plane_id] =
          GST_VIDEO_FRAME_PLANE_DATA (&(priv->kernel->in_vframe), plane_id);
      GST_LOG_OBJECT (self, "inbuf plane[%d] : vaddr = %p", plane_id,
          ivas_frame->vaddr[plane_id]);
    }
  }

  vmeta = gst_buffer_get_video_meta (inbuf);
  if (vmeta) {
    ivas_frame->props.stride = vmeta->stride[0];
  } else {
    GST_DEBUG_OBJECT (self, "video meta not present in buffer");
    ivas_frame->props.stride =
        GST_VIDEO_INFO_PLANE_STRIDE (self->priv->in_vinfo, 0);
  }

  ivas_frame->props.width = GST_VIDEO_INFO_WIDTH (self->priv->in_vinfo);
  ivas_frame->props.height = GST_VIDEO_INFO_HEIGHT (self->priv->in_vinfo);
  ivas_frame->props.fmt =
      get_kernellib_format (GST_VIDEO_INFO_FORMAT (self->priv->in_vinfo));
  ivas_frame->n_planes = GST_VIDEO_INFO_N_PLANES (self->priv->in_vinfo);
  ivas_frame->app_priv = inbuf;

  if (phy_addr) {
    for (plane_id = 0;
        plane_id < GST_VIDEO_INFO_N_PLANES (self->priv->in_vinfo); plane_id++) {
      gsize offset;

      if (vmeta) {
        offset = vmeta->offset[plane_id];
      } else {
        offset = GST_VIDEO_INFO_PLANE_OFFSET (self->priv->in_vinfo, plane_id);
      }
      ivas_frame->paddr[plane_id] = phy_addr + offset;
      GST_LOG_OBJECT (self,
          "inbuf plane[%d] : paddr = %p, offset = %lu, stride = %d", plane_id,
          (void *) ivas_frame->paddr[plane_id], offset,
          ivas_frame->props.stride);
    }
  }
  GST_LOG_OBJECT (self, "successfully prepared input ivas frame");
  return TRUE;

error:
  if (in_mem)
    gst_memory_unref (in_mem);

  return FALSE;
}

static gboolean
ivas_xfilter_prepare_output_frame (GstIvas_XFilter * self, GstBuffer * outbuf)
{
  GstIvas_XFilterPrivate *priv = self->priv;
  IVASFrame *ivas_frame = NULL;
  GstMemory *out_mem = NULL;
  guint64 phy_addr = -1;
  guint plane_id;
  GstVideoMeta *vmeta = NULL;

  out_mem = gst_buffer_get_memory (outbuf, 0);
  if (out_mem == NULL) {
    GST_ERROR_OBJECT (self, "failed to get memory from output buffer");
    goto error;
  }

  /* assuming outbuf allocation is from device as kernel */
  phy_addr = gst_ivas_allocator_get_paddr (out_mem);

  vmeta = gst_buffer_get_video_meta (outbuf);
  if (vmeta == NULL) {
    GST_ERROR_OBJECT (self, "video meta not present in output buffer");
    goto error;
  }

  ivas_frame = priv->kernel->output[0];
  ivas_frame->props.width = GST_VIDEO_INFO_WIDTH (self->priv->out_vinfo);
  ivas_frame->props.height = GST_VIDEO_INFO_HEIGHT (self->priv->out_vinfo);
  ivas_frame->props.stride = vmeta->stride[0];
  ivas_frame->props.fmt =
      get_kernellib_format (GST_VIDEO_INFO_FORMAT (self->priv->out_vinfo));
  ivas_frame->n_planes = GST_VIDEO_INFO_N_PLANES (self->priv->out_vinfo);
  ivas_frame->app_priv = outbuf;

  if (!(priv->kernel->name || priv->kernel->is_softkernel)) {
    /* software lib mode */
    if (!gst_video_frame_map (&(priv->kernel->out_vframe), self->priv->out_vinfo,
        outbuf, GST_MAP_WRITE)) {
      GST_ERROR_OBJECT (self, "failed to map output buffer");
      goto error;
    }

    for (plane_id = 0;
        plane_id < GST_VIDEO_INFO_N_PLANES (self->priv->out_vinfo); plane_id++) {
      ivas_frame->vaddr[plane_id] =
          GST_VIDEO_FRAME_PLANE_DATA (&(priv->kernel->out_vframe), plane_id);
      GST_LOG_OBJECT (self, "outbuf plane[%d] : vaddr = %p", plane_id,
          ivas_frame->vaddr[plane_id]);
    }
  }

  for (plane_id = 0;
      plane_id < GST_VIDEO_INFO_N_PLANES (self->priv->out_vinfo); plane_id++) {
    ivas_frame->paddr[plane_id] = phy_addr + vmeta->offset[plane_id];

    GST_LOG_OBJECT (self,
        "outbuf plane[%d] : paddr = %p, offset = %lu, stride = %d", plane_id,
        (void *) ivas_frame->paddr[plane_id], vmeta->offset[plane_id],
        ivas_frame->props.stride);
  }

  gst_memory_unref (out_mem);

  GST_LOG_OBJECT (self, "successfully prepared output ivas frame");
  return TRUE;

error:
  if (out_mem)
    gst_memory_unref (out_mem);

  return FALSE;
}

static GstFlowReturn
gst_ivas_xfilter_transform_ip (GstBaseTransform * base, GstBuffer * buf)
{
  GstIvas_XFilter *self = GST_IVAS_XFILTER (base);
  Ivas_XFilter *kernel = self->priv->kernel;
  GstBuffer *new_inbuf = NULL;
  int ret;
  gboolean bret = FALSE;

  bret = ivas_xfilter_prepare_input_frame (self, buf, &new_inbuf);
  if (!bret)
    goto error;

  /* update dynamic json config to kernel */
  kernel->ivas_handle->kernel_dyn_config = self->priv->dyn_json_config;

  ret = kernel->kernel_start_func (kernel->ivas_handle, 0, kernel->input, NULL);
  if (ret < 0) {
    GST_ERROR_OBJECT (self, "kernel start failed");
    goto error;
  }

  ret = kernel->kernel_done_func (kernel->ivas_handle);
  if (ret < 0) {
    GST_ERROR_OBJECT (self, "kernel done failed");
    goto error;
  }
  g_signal_emit (self, ivas_signals[SIGNAL_IVAS], 0);

  if (new_inbuf) {
    GstBufferCopyFlags flags = GST_BUFFER_COPY_FLAGS |
        GST_BUFFER_COPY_TIMESTAMPS;

    if (self->priv->element_mode == IVAS_ELEMENT_MODE_IN_PLACE) {
      GstMemory *mem = NULL;
      GstVideoFrame in_vframe = {0,}, new_vframe = {0,};

      flags |= GST_BUFFER_COPY_META;

      mem = gst_buffer_get_memory (new_inbuf, 0);
      if (mem == NULL) {
        GST_ERROR_OBJECT (self, "failed to get memory from internal buffer");
        goto error;
      }

#ifdef XLNX_PCIe_PLATFORM
      /* sync internal buffer buffer data*/
      gst_ivas_memory_set_sync_flag (mem, IVAS_SYNC_FROM_DEVICE);
      gst_ivas_memory_sync_with_flags (mem, GST_MAP_READ);
#endif

      /* map internal buffer in READ mode */
      if (!gst_video_frame_map (&new_vframe, self->priv->in_vinfo, new_inbuf,
              GST_MAP_READ)) {
        GST_ERROR_OBJECT (self, "failed to map internal input buffer");
        goto error;
      }

      /* map input buffer in WRITE mode */
      if (!gst_video_frame_map (&in_vframe, self->priv->in_vinfo, buf,
              GST_MAP_WRITE)) {
        GST_ERROR_OBJECT (self, "failed to map input buffer");
        goto error;
      }

      GST_CAT_LOG_OBJECT (GST_CAT_PERFORMANCE, self,
          "slow copy to input buffer");

      /* copy data from internal buffer to input buffer */
      gst_video_frame_copy (&in_vframe, &new_vframe);
      gst_video_frame_unmap (&in_vframe);
      gst_video_frame_unmap (&new_vframe);
      gst_memory_unref (mem);
    }

    /* copy back any updates done by ivas kernel lib */
    gst_buffer_copy_into (buf, new_inbuf, flags, 0, -1);
    gst_buffer_unref (new_inbuf);
  }

  /* remove private buffer maps if present */
  g_hash_table_remove_all (self->priv->priv_buf_vmaps);

  if (!(self->priv->kernel->name || self->priv->kernel->is_softkernel)) {
    if (kernel->in_vframe.data[0]) {
      gst_video_frame_unmap (&(kernel->in_vframe));
    }
  }

  GST_LOG_OBJECT (self, "processed buffer %p", buf);

  return GST_FLOW_OK;

error:
  /* remove private buffer maps if present */
  g_hash_table_remove_all (self->priv->priv_buf_vmaps);

  if (new_inbuf)
    gst_buffer_unref (new_inbuf);

  return GST_FLOW_ERROR;
}

static GstFlowReturn
gst_ivas_xfilter_transform (GstBaseTransform * base, GstBuffer * inbuf,
    GstBuffer * outbuf)
{
  GstIvas_XFilter *self = GST_IVAS_XFILTER (base);
  Ivas_XFilter *kernel = self->priv->kernel;
  GstBuffer *new_inbuf = NULL;
  int ret;
  gboolean bret = FALSE;

  bret = ivas_xfilter_prepare_input_frame (self, inbuf, &new_inbuf);
  if (!bret)
    goto error;

  bret = ivas_xfilter_prepare_output_frame (self, outbuf);
  if (!bret)
    goto error;

  /* update dynamic json config to kernel */
  kernel->ivas_handle->kernel_dyn_config = self->priv->dyn_json_config;

  ret =
      kernel->kernel_start_func (kernel->ivas_handle, 0, kernel->input,
      kernel->output);
  if (ret < 0) {
    GST_ERROR_OBJECT (self, "kernel start failed");
    goto error;
  }

  ret = kernel->kernel_done_func (kernel->ivas_handle);
  if (ret < 0) {
    GST_ERROR_OBJECT (self, "kernel done failed");
    goto error;
  }
  g_signal_emit (self, ivas_signals[SIGNAL_IVAS], 0);
#ifdef XLNX_PCIe_PLATFORM
  {
    GstMemory *outmem = NULL;
    outmem = gst_buffer_get_memory (outbuf, 0);
    if (outmem == NULL) {
      GST_ERROR_OBJECT (self, "failed to get memory from output buffer");
      goto error;
    }
    gst_ivas_memory_set_sync_flag (outmem, IVAS_SYNC_FROM_DEVICE);
    gst_memory_unref (outmem);
  }
#endif

  if (!(self->priv->kernel->name || self->priv->kernel->is_softkernel)) {
    if (kernel->in_vframe.data[0]) {
      gst_video_frame_unmap (&(kernel->in_vframe));
    }

    if (kernel->out_vframe.data[0]) {
      gst_video_frame_unmap (&(kernel->out_vframe));
    }
  }

  /* remove private buffer maps if present */
  g_hash_table_remove_all (self->priv->priv_buf_vmaps);

  if (new_inbuf)
    gst_buffer_unref (new_inbuf);

  return GST_FLOW_OK;

error:
  /* remove private buffer maps if present */
  g_hash_table_remove_all (self->priv->priv_buf_vmaps);

  if (new_inbuf)
    gst_buffer_unref (new_inbuf);

  return GST_FLOW_ERROR;
}

static gboolean
plugin_init (GstPlugin * ivas_xfilter)
{
  return gst_element_register (ivas_xfilter, "ivas_xfilter", GST_RANK_NONE,
      GST_TYPE_IVAS_XFILTER);
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    ivas_xfilter,
    "GStreamer IVAS plug-in for filters",
    plugin_init, "1.0", "Proprietary",
    "Xilinx IVAS SDK plugin", "http://xilinx.com/")

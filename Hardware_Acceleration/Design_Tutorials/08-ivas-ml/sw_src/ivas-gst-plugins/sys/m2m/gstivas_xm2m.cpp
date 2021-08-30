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
/* GStreamer
 * Copyright (C) <1999> Erik Walthinsen <omega@cse.ogi.edu>
 * Copyright (C) 2005-2012 David Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <stdio.h>
#include <gst/allocators/gstdmabuf.h>
#include <gst/video/video.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <jansson.h>
#include "gstivas_xm2m.h"
extern "C" {
#include "ivas/xrt_utils.h"
}
#ifdef XLNX_PCIe_PLATFORM
#include <experimental/xrt-next.h>
#else
#include <xrt/experimental/xrt-next.h>
#endif
#include "gstivas_xm2m.h"

#define MAX_KERNELS	10
int f_num = 0;
int fps = 0;
#define PROFILING 1
struct timespec start, end;

pthread_mutex_t count_mutex;
GST_DEBUG_CATEGORY_STATIC (gst_ivas_xm2m_debug);
#define GST_CAT_DEFAULT gst_ivas_xm2m_debug

#define REPO_PATH	"/usr/lib"

#define DEFAULT_DEVICE_INDEX 0
enum
{
	PROP_0,
	PROP_XCLBIN_LOCATION,
	PROP_CONFIG_LOCATION,
};

static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE ("sink",
		GST_PAD_SINK,
		GST_PAD_ALWAYS,
		GST_STATIC_CAPS(GST_VIDEO_CAPS_MAKE("{RGB, YUY2, NV16, GRAY8, BGRx, UYVY, BGR, NV12, BGRx, RGBx, RGBA, BGRA}")));

static GstStaticPadTemplate src_request_template =
GST_STATIC_PAD_TEMPLATE ("src_%u",
		GST_PAD_SRC,
		GST_PAD_REQUEST,
		GST_STATIC_CAPS(GST_VIDEO_CAPS_MAKE("{RGB, YUY2, NV16, GRAY8, BGRx, UYVY, BGR, NV12, BGRx, RGBx, RGBA, BGRA}")));

GType gst_ivas_xm2m_pad_get_type (void);

typedef struct _GstIvasXM2MPad GstIvasXM2MPad;
typedef struct _GstIvasXM2MPadClass GstIvasXM2MPadClass;

struct _GstIvasXM2MPad {
	GstPad parent;
	guint index;
	GstBufferPool *pool;
	GstVideoInfo *out_vinfo;
};

struct _GstIvasXM2MPadClass {
	GstPadClass parent;
};

#define GST_TYPE_IVAS_XM2M_PAD \
	(gst_ivas_xm2m_pad_get_type())
#define GST_IVAS_XM2M_PAD(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_IVAS_XM2M_PAD, \
				     GstIvasXM2MPad))
#define GST_IVAS_XM2M_PAD_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_IVAS_XM2M_PAD, \
				  GstIvasXM2MPadClass))
#define GST_IS_IVAS_XM2M_PAD(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_IVAS_XM2M_PAD))
#define GST_IS_IVAS_XM2M_PAD_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_IVAS_XM2M_PAD))
#define GST_IVAS_XM2M_PAD_CAST(obj) \
	((GstIvasXM2MPad *)(obj))

G_DEFINE_TYPE (GstIvasXM2MPad, gst_ivas_xm2m_pad, GST_TYPE_PAD);

#define gst_ivas_xm2m_srcpad_at_index(self, idx) ((GstIvasXM2MPad *)(g_list_nth ((self)->srcpads, idx))->data)

	static void
gst_ivas_xm2m_pad_class_init (GstIvasXM2MPadClass * klass)
{
	/* nothing */
}

static void gst_ivas_xm2m_pad_init (GstIvasXM2MPad * pad) {}

static void gst_ivas_xm2m_set_property (GObject * object, guint prop_id,
		const GValue * value, GParamSpec * pspec);
static void gst_ivas_xm2m_get_property (GObject * object, guint prop_id,
		GValue * value, GParamSpec * pspec);
static gboolean gst_ivas_xm2m_sink_event (GstPad * pad,
		GstObject * parent, GstEvent * event);
static GstFlowReturn gst_ivas_xm2m_chain (GstPad * pad,
		GstObject * parent, GstBuffer * buf);
static gboolean gst_ivas_xm2m_sink_query (GstPad * pad,
		GstObject * parent, GstQuery * query);
static GstStateChangeReturn gst_ivas_xm2m_change_state
(GstElement * element, GstStateChange transition);
static GstCaps *gst_ivas_xm2m_transform_caps (GstIvasXM2M * self,
		GstPadDirection direction, GstCaps * caps, GstCaps * filter);
static GstPad *gst_ivas_xm2m_request_new_pad (GstElement * element,
		GstPadTemplate * templ, const gchar * name_templ, const GstCaps * caps);
static void gst_ivas_xm2m_release_pad (GstElement * element,
		GstPad * pad);

typedef struct _GstIvasXM2MKernel {
	gchar *lib_path;
	IVASKernel *ivas_handle;
	void *lib_fd;
	IVASKernelInit kernel_init_func;
	IVASKernelStartFunc kernel_start_func;
	IVASKernelDeInit kernel_deinit_func;
} GstIvasXM2MKernel;

struct _GstIvasXM2MPrivate {
	GstVideoInfo *in_vinfo;
	xclDeviceHandle xcl_handle;
	uuid_t xclbinId;
	GstBuffer *outbufs[MAX_CHANNELS];
	GstBufferPool *input_pool;
	gboolean validate_import;
	gboolean use_inpool;
	size_t kernel_count;
	IVASFrame *input;
	IVASFrame *output[MAX_CHANNELS];
	GstIvasXM2MKernel kernels[MAX_KERNELS];
	IVASKernelDoneFunc kernel_done_func;
};

#define gst_ivas_xm2m_parent_class parent_class
G_DEFINE_TYPE_WITH_PRIVATE (GstIvasXM2M, gst_ivas_xm2m, GST_TYPE_ELEMENT);
#define GST_IVAS_XM2M_PRIVATE(self) (GstIvasXM2MPrivate *) (gst_ivas_xm2m_get_instance_private (self))

	static gboolean
ivas_xm2m_register_prep_write_with_caps (GstIvasXM2M * self, guint chan_id, GstCaps *in_caps, GstCaps *out_caps)
{
	GstIvasXM2MPad *srcpad = NULL;
	GstVideoInfo in_vinfo = {0, };

	if (!gst_video_info_from_caps (&in_vinfo, in_caps)) {
		GST_ERROR_OBJECT (self, "failed to get video info from input caps");
		return FALSE;
	}

	srcpad = gst_ivas_xm2m_srcpad_at_index(self, chan_id);

	if (!gst_video_info_from_caps (srcpad->out_vinfo, out_caps)) {
		GST_ERROR_OBJECT (self, "failed to get video info from output caps");
		return FALSE;
	}

	/* activate output buffer pool */
	if (!gst_buffer_pool_set_active (srcpad->pool, TRUE)) {
		GST_ERROR_OBJECT (srcpad, "failed to activate pool");
		goto error;
	}

	return TRUE;

error:
	return FALSE;
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
		default:
			GST_ERROR ("Not supporting %s yet", gst_video_format_to_string (gst_fmt));
			return IVAS_VMFT_UNKNOWN;
	}
}

	static gboolean
ivas_xm2m_open (GstIvasXM2M * self)
{
	GstIvasXM2MPrivate *priv = self->priv;
	unsigned int cu_index = 0;
	json_t *root, *karray, *kernel, *value;
	gchar *lib_path = NULL;
	size_t i;
	char *error;
	json_error_t jerror;

	GST_DEBUG_OBJECT (self, "ivas M2M open");

	root = json_load_file(self->config_file, JSON_DECODE_ANY, &jerror);
	if(!root) {
		GST_ERROR_OBJECT (self, "failed to load json file. reason %s", jerror.text);
		json_decref(root);
		return FALSE;
	}

	value = json_object_get (root, "xclbin-location");
	if (!json_is_string (value)) {
		GST_ERROR_OBJECT (self, "xclbin location is not of string type");
		json_decref(root);
		return FALSE;
	}
	self->xclbin_path = (gchar *)json_string_value (value);
	GST_INFO_OBJECT (self, "xclbin path %s", self->xclbin_path);

	if (download_xclbin (self->xclbin_path, DEFAULT_DEVICE_INDEX, NULL, &(priv->xcl_handle), &(priv->xclbinId))) {
		GST_ERROR_OBJECT (self, "failed to initialize XRT");
		json_decref(root);
		return FALSE;
	}

	value = json_object_get (root, "ivas-library-repo");
	if (!value) {
		GST_DEBUG_OBJECT (self, "library repo path does not exist.taking default %s", REPO_PATH);
		lib_path = g_strdup (REPO_PATH);
	} else {
		gchar *path = g_strdup(json_string_value (value));

		if (!g_str_has_suffix (path, "/")) {
			lib_path = g_strconcat (path, "/", NULL);
			g_free (path);
		} else {
			lib_path = path;
		}
	}

	karray = json_object_get (root, "kernels");
	if (!karray) {
		GST_ERROR_OBJECT (self, "failed to find key kernels");
		json_decref(root);
		return FALSE;
	}

	if (!json_is_array (karray)) {
		GST_ERROR_OBJECT (self, "kernels key is not of array type");
		json_decref(root);
		return FALSE;
	}


	json_array_foreach(karray, i, kernel) {

		priv->kernels[priv->kernel_count].ivas_handle = (IVASKernel *) calloc (1, sizeof (IVASKernel));
		if (!priv->kernels[priv->kernel_count].ivas_handle) {
			GST_ERROR_OBJECT (self, "ERROR: failed to allocate ivas_handles memory\n");
			exit(EXIT_FAILURE);
		}
		priv->kernels[priv->kernel_count].ivas_handle->xcl_handle = (void *)priv->xcl_handle;
		cu_index = 0;

		/* kernel name */
		value = json_object_get (kernel, "kernel-name");
		if (json_is_string (value)) {
      			cu_index = xclIPName2Index (priv->xcl_handle, json_string_value (value)); //2020.1 new
      			//xclIPName2Index (priv->xcl_handle, json_string_value (value), &cu_index); //2020.1
			//xclCuName2Index (priv->xcl_handle, json_string_value (value), &cu_index);  //2019.2
			GST_INFO_OBJECT (self, "kernel[%lu] name = %s and cu_idx = %u", i, (char *)json_string_value (value), cu_index);
		} else {
			GST_INFO_OBJECT (self, "kernel name is not available, kernel lib is non-XRT based one");
		}
		priv->kernels[priv->kernel_count].ivas_handle->cu_idx = cu_index;

		if (xclOpenContext(priv->xcl_handle, priv->xclbinId, cu_index, true)) {
		    GST_ERROR_OBJECT (self, "failed to do xclOpenContext...");
		    return FALSE;
		}
		/* library name */
		value = json_object_get (kernel, "library-name");
		if (!json_is_string (value)) {
			GST_ERROR_OBJECT (self, "library name is not of string type");
			return FALSE;
		}
		priv->kernels[priv->kernel_count].lib_path = (gchar *)g_strconcat (lib_path, json_string_value (value), NULL);
		GST_DEBUG_OBJECT (self, "library path at index %lu : %s", i, priv->kernels[priv->kernel_count].lib_path);

		/* kernel config reading done */
		value = json_object_get (kernel, "config");
		if (!json_is_object (value)) {
			GST_ERROR_OBJECT (self, "config is not of object type");
			json_decref(root);
			return FALSE;
		}
		priv->kernels[priv->kernel_count].ivas_handle->kernel_config = json_deep_copy(value);
		GST_DEBUG_OBJECT (self, "kernel config size = %lu", json_object_size(value));


		priv->kernel_count++;
	}
	json_decref(root);

	for (i = 0; i < priv->kernel_count; i++) {

		priv->kernels[i].lib_fd = dlopen(priv->kernels[i].lib_path, RTLD_LAZY);
		if (!priv->kernels[i].lib_fd) {
			GST_ERROR_OBJECT (self, " unable to open shared library %s", priv->kernels[i].lib_path);
			return FALSE;
		}
		dlerror();    /* Clear any existing error */
		self->priv->kernels[i].kernel_start_func = (IVASKernelStartFunc) dlsym (priv->kernels[i].lib_fd, "xlnx_kernel_start");
		error = dlerror();
		if (error != NULL) {
			GST_ERROR_OBJECT (self, "ERROR: %s", error);
			return FALSE;
		}
		if (0 == i) {
			self->priv->kernel_done_func = (IVASKernelDoneFunc) dlsym (priv->kernels[i].lib_fd, "xlnx_kernel_done");
		}
		self->priv->kernels[i].kernel_init_func = (IVASKernelInit) dlsym (priv->kernels[i].lib_fd, "xlnx_kernel_init");
		self->priv->kernels[i].kernel_deinit_func = (IVASKernelInit) dlsym (priv->kernels[i].lib_fd, "xlnx_kernel_deinit");
		if (self->priv->kernels[i].kernel_init_func)
			self->priv->kernels[i].kernel_init_func(self->priv->kernels[i].ivas_handle);
	}
#ifdef DEBUG
	for (i=0; i< priv->kernel_count; i++) {
		GST_DEBUG_OBJECT (self, "Lib path ==> %s\n", priv->kernels[i].lib_path);
		GST_DEBUG_OBJECT(self, "kernel index => %d\n", priv->kernels[i].ivas_handle->cu_idx);
		GST_DEBUG_OBJECT(self, "\n---------\n");
	}
#endif
	return TRUE;
}

	static gboolean
ivas_xm2m_allocate_internal_pool (GstIvasXM2M *self)
{
	GstVideoInfo info;
	GstBufferPool *pool = NULL;
	GstStructure *config;
	GstAllocator *allocator = NULL;
	GstAllocationParams alloc_params;
	GstCaps *caps = NULL;

	caps = gst_pad_get_current_caps (self->sinkpad);

	if (!gst_video_info_from_caps (&info, caps)) {
		GST_WARNING_OBJECT (self, "Failed to parse caps %" GST_PTR_FORMAT, caps);
		gst_caps_unref (caps);
		return FALSE;
	}

	pool = gst_video_buffer_pool_new ();
	GST_LOG_OBJECT (self, "allocated internal sink pool %p", pool);

	allocator = gst_ivas_allocator_new(DEFAULT_DEVICE_INDEX, TRUE);
	gst_allocation_params_init (&alloc_params);
	alloc_params.flags = GST_MEMORY_FLAG_PHYSICALLY_CONTIGUOUS;

	config = gst_buffer_pool_get_config (pool);
	gst_buffer_pool_config_set_params (config, caps, GST_VIDEO_INFO_SIZE (&info), 3, 5);
	gst_buffer_pool_config_set_allocator (config, allocator, &alloc_params);
	gst_buffer_pool_config_add_option (config, GST_BUFFER_POOL_OPTION_VIDEO_META);

	if (!gst_buffer_pool_set_config (pool, config)) {
		GST_ERROR_OBJECT (self, "Failed to set config on input pool");
		goto error;
	}

	if (self->priv->input_pool)
		gst_object_unref (self->priv->input_pool);

	self->priv->input_pool = pool;

	GST_INFO_OBJECT (self, "allocated %" GST_PTR_FORMAT" pool", self->priv->input_pool);
	gst_caps_unref (caps);

	return TRUE;

error:
	gst_caps_unref (caps);
	return FALSE;
}

	static gboolean
ivas_xm2m_write_input_registers (GstIvasXM2M * self, GstBuffer **inbuf)
{
	GstMemory *in_mem = NULL;
	GstVideoFrame in_vframe = {0, }, own_vframe = {0, };
	guint64 phy_addr = -1;
	GstVideoMeta *vmeta = NULL;
	gboolean bret;
	guint plane_id;

	in_mem = gst_buffer_get_memory (*inbuf, 0);
	if (!in_mem) {
		GST_ERROR_OBJECT (self, "failed to get memory from input buffer");
		goto error;
	}

	if (gst_is_ivas_memory (in_mem)
			&& gst_ivas_memory_can_avoid_copy (in_mem, DEFAULT_DEVICE_INDEX)) {
		phy_addr = gst_ivas_allocator_get_paddr (in_mem);
		/* syncs data wheXLNX_SYNC_TO_DEVICE flag is enabled */
#ifdef XLNX_PCIe_PLATFORM 
		bret = gst_ivas_memory_sync_bo (in_mem);
		if (!bret)
			goto error;
#endif

	} else if (gst_is_dmabuf_memory (in_mem)) {
		guint bo = NULLBO;
		gint dma_fd = -1;
		struct xclBOProperties p;

		dma_fd = gst_dmabuf_memory_get_fd (in_mem);
		if (dma_fd < 0) {
			GST_ERROR_OBJECT (self, "failed to get DMABUF FD");
			goto error;
		}
		/* dmabuf but not from xrt */
		bo = xclImportBO (self->priv->xcl_handle, dma_fd, 0);
		if (bo == NULLBO) {
			GST_WARNING_OBJECT (self,
					"failed to get XRT BO...fall back to copy input");
		}

		GST_DEBUG_OBJECT (self, "received dma fd %d and its xrt BO = %u", dma_fd,
				bo);

		if (!xclGetBOProperties (self->priv->xcl_handle, bo, &p)) {
			phy_addr = p.paddr;
		} else {
			GST_WARNING_OBJECT (self,
					"failed to get physical address...fall back to copy input");
			self->priv->use_inpool = TRUE;
		}
	} else {
		self->priv->use_inpool = TRUE;
	}
	gst_memory_unref (in_mem);

	if (self->priv->use_inpool) {
		if (self->priv->validate_import) {
			if (!self->priv->input_pool) {
				bret = ivas_xm2m_allocate_internal_pool (self);
				if (!bret)
					goto error;
			}
			if (!gst_buffer_pool_is_active (self->priv->input_pool))
				gst_buffer_pool_set_active (self->priv->input_pool, TRUE);
			self->priv->validate_import = FALSE;
		}
		GstBuffer *own_inbuf;
		GstFlowReturn fret;

		/* acquire buffer from own input pool */
		fret = gst_buffer_pool_acquire_buffer (self->priv->input_pool, &own_inbuf, NULL);
		if (fret != GST_FLOW_OK) {
			GST_ERROR_OBJECT (self, "failed to allocate buffer from pool %p", self->priv->input_pool);
			goto error;
		}
		GST_LOG_OBJECT (self, "acquired buffer %p from own pool", own_inbuf);

		/* map internal buffer in write mode */
		if (!gst_video_frame_map (&own_vframe, self->priv->in_vinfo, own_inbuf, GST_MAP_WRITE)) {
			GST_ERROR_OBJECT (self, "failed to map internal input buffer");
			goto error;
		}

		/* map input buffer in read mode */
		if (!gst_video_frame_map (&in_vframe, self->priv->in_vinfo, *inbuf, GST_MAP_READ)) {
			GST_ERROR_OBJECT (self, "failed to map input buffer");
			goto error;
		}
		gst_video_frame_copy (&own_vframe, &in_vframe);

		gst_video_frame_unmap (&in_vframe);
		gst_video_frame_unmap (&own_vframe);
		gst_buffer_copy_into (own_inbuf, *inbuf, (GstBufferCopyFlags)(GST_BUFFER_COPY_FLAGS | GST_BUFFER_COPY_TIMESTAMPS), 0, -1);
		gst_buffer_unref (*inbuf);
		*inbuf = own_inbuf;
	}

	vmeta = gst_buffer_get_video_meta (*inbuf);
	if (vmeta == NULL) {
		GST_ERROR_OBJECT (self, "video meta not present in buffer");
		goto error;
	}

	if (phy_addr == (uint64_t) - 1) {
		in_mem = gst_buffer_get_memory (*inbuf, 0);
		if (in_mem == NULL) {
			GST_ERROR_OBJECT (self, "failed to get memory from input buffer");
			goto error;
		}
		phy_addr = gst_ivas_allocator_get_paddr(in_mem);
		gst_memory_unref (in_mem);
	}
	GST_LOG_OBJECT (self, "input paddr %p", (void*)phy_addr);
	self->priv->input = (IVASFrame *) calloc (1, sizeof (IVASFrame));
	if (NULL == self->priv->input) {
		GST_ERROR_OBJECT (self, "failed to allocate memory");
		goto error;
	}
	self->priv->input->props.width = vmeta->width;
	self->priv->input->props.height = vmeta->height;
	self->priv->input->props.stride = *(vmeta->stride);
	self->priv->input->props.fmt = get_kernellib_format (vmeta->format);
	self->priv->input->n_planes = GST_VIDEO_INFO_N_PLANES (self->priv->in_vinfo);
	for (plane_id = 0;
			plane_id < GST_VIDEO_INFO_N_PLANES (self->priv->in_vinfo); plane_id++) {
		self->priv->input->paddr[plane_id] = phy_addr + vmeta->offset[plane_id];
		GST_LOG_OBJECT (self, "inbuf plane[%d] : paddr = %p, offset = %lu", plane_id, (void *) self->priv->input->paddr[plane_id], vmeta->offset[plane_id]);
	}

	return TRUE;

error:
	if (in_vframe.data)
		gst_video_frame_unmap (&in_vframe);
	if (own_vframe.data)
		gst_video_frame_unmap (&own_vframe);
	if (in_mem)
		gst_memory_unref (in_mem);

	return FALSE;
}

	static gboolean
ivas_xm2m_write_output_registers (GstIvasXM2M * self)
{
	guint chan_id, plane_id;
	GstMemory *mem = NULL;
	GstIvasXM2MPad *srcpad = NULL;

	for (chan_id = 0; chan_id < self->num_request_pads; chan_id++) {
		GstBuffer *outbuf = NULL;
		GstFlowReturn fret;
		GstVideoMeta *vmeta;
		guint64 phy_addr = -1;

		srcpad = gst_ivas_xm2m_srcpad_at_index (self, chan_id);

		fret = gst_buffer_pool_acquire_buffer (srcpad->pool, &outbuf, NULL);
		if (fret != GST_FLOW_OK) {
			GST_ERROR_OBJECT (srcpad, "failed to allocate buffer from pool %p", srcpad->pool);
			goto error;
		}
		GST_LOG_OBJECT (srcpad, "acquired buffer %p from pool", outbuf);

		self->priv->outbufs[chan_id] = outbuf;

		mem = gst_buffer_get_memory (outbuf, 0);
		if (mem == NULL) {
			GST_ERROR_OBJECT (srcpad, "chan-%d : failed to get memory from output buffer", chan_id);
			goto error;
		}
		/* No need to check whether memory is from device or not here.
		 * Because, we have made sure memory is allocated from device in decide_allocation
		 */
		if (gst_is_ivas_memory (mem)
				&& gst_ivas_memory_can_avoid_copy (mem, DEFAULT_DEVICE_INDEX)) {
			phy_addr = gst_ivas_allocator_get_paddr (mem);

		} else if (gst_is_dmabuf_memory (mem)) {
			guint bo = NULLBO;
			gint dma_fd = -1;
			struct xclBOProperties p;

			dma_fd = gst_dmabuf_memory_get_fd (mem);
			if (dma_fd < 0) {
				GST_ERROR_OBJECT (self, "failed to get DMABUF FD");
				goto error;
			}

			/* dmabuf but not from xrt */
			bo = xclImportBO (self->priv->xcl_handle, dma_fd, 0);
			if (bo == NULLBO) {
				GST_WARNING_OBJECT (self,
						"failed to get XRT BO...fall back to copy input");
			}

			GST_INFO_OBJECT (self, "received dma fd %d and its xrt BO = %u", dma_fd,
					bo);

			if (!xclGetBOProperties (self->priv->xcl_handle, bo, &p)) {
				phy_addr = p.paddr;
			} else {
				GST_WARNING_OBJECT (self,
						"failed to get physical address...fall back to copy input");
			}

		}
		if (phy_addr == (uint64_t) - 1) {
			phy_addr = gst_ivas_allocator_get_paddr (mem);
			if (!phy_addr)
				goto error;
		}


		vmeta = gst_buffer_get_video_meta (outbuf);
		if (vmeta == NULL) {
			GST_ERROR_OBJECT (srcpad, "video meta not present in buffer");
			goto error;
		}

		self->priv->output[chan_id] = (IVASFrame *) calloc (1, sizeof (IVASFrame));
		if (NULL == self->priv->output[chan_id]) {
			GST_ERROR_OBJECT (self, "failed to allocate memory");
			goto error;
		}
		self->priv->output[chan_id]->props.width = vmeta->width;
		self->priv->output[chan_id]->props.height = vmeta->height;
		self->priv->output[chan_id]->props.stride = *(vmeta->stride);
		self->priv->output[chan_id]->props.fmt = get_kernellib_format (vmeta->format);
		self->priv->output[chan_id]->n_planes = vmeta->n_planes;
		for (plane_id = 0; plane_id < vmeta->n_planes; plane_id++) {
			self->priv->output[chan_id]->paddr[plane_id] = phy_addr + vmeta->offset[plane_id];
			GST_LOG_OBJECT (self, "Outbuf plane[%d] : paddr = %p, offset = %lu", plane_id, (void *) self->priv->output[chan_id]->paddr[plane_id], vmeta->offset[plane_id]);
		}


		/* when plugins/app request to map this memory, sync will occur */
		//gst_xrt_memory_set_sync_flag (mem, XLNX_SYNC_FROM_DEVICE);
		gst_memory_unref (mem);
	}

	return TRUE;

error:
	if (mem)
		gst_memory_unref (mem);

	return FALSE;
}

static gboolean ivas_xm2m_process (GstIvasXM2M * self)
{
	size_t i, ret, start = 0x1;
	GstIvasXM2MPrivate *priv = self->priv;

	for (i = 0; i < priv->kernel_count ; i++) {
		ret = self->priv->kernels[i].kernel_start_func (self->priv->kernels[i].ivas_handle, start, &(self->priv->input), self->priv->output);
		if (ret < 0) {
			GST_ERROR_OBJECT (self, "kernel start failed");
			return FALSE;
		}
	}

	ret = self->priv->kernel_done_func (self->priv->kernels[0].ivas_handle);
	if (ret < 0) {
		GST_ERROR_OBJECT (self, "kernel done failed");
		return FALSE;
	}
	return ret;
}

	static void
ivas_xm2m_close (GstIvasXM2M * self)
{
	GstIvasXM2MPrivate *priv = self->priv;
	GST_DEBUG_OBJECT (self, "Closing");
	size_t i;

	for (i = 0; i < priv->kernel_count; i++){
		if (self->priv->kernels[i].kernel_deinit_func)
			self->priv->kernels[i].kernel_deinit_func(self->priv->kernels[i].ivas_handle);
		if (priv->kernels[i].lib_fd)
			dlclose (priv->kernels[i].lib_fd);
		if (priv->kernels[i].ivas_handle)
			free (priv->kernels[i].ivas_handle);
		xclCloseContext (priv->xcl_handle, priv->xclbinId, priv->kernels[i].ivas_handle->cu_idx);
	}
	xclClose(priv->xcl_handle);
}

	static void
gst_ivas_xm2m_finalize (GObject * object)
{
	GstIvasXM2M *self = GST_IVAS_XM2M (object);

	g_hash_table_unref (self->pad_indexes);
	gst_video_info_free(self->priv->in_vinfo);

	G_OBJECT_CLASS (parent_class)->finalize (object);
}

	static void
gst_ivas_xm2m_class_init (GstIvasXM2MClass * klass)
{
	GObjectClass *gobject_class;
	GstElementClass *gstelement_class;

	gobject_class = (GObjectClass *) klass;
	gstelement_class = (GstElementClass *) klass;

	GST_DEBUG_CATEGORY_INIT (gst_ivas_xm2m_debug, "ivas_xm2m",
			0, "Xilinx's M2M plugin");

	gobject_class->set_property =
		GST_DEBUG_FUNCPTR (gst_ivas_xm2m_set_property);
	gobject_class->get_property =
		GST_DEBUG_FUNCPTR (gst_ivas_xm2m_get_property);
	gobject_class->finalize = GST_DEBUG_FUNCPTR (gst_ivas_xm2m_finalize);

	gst_element_class_set_details_simple (gstelement_class,
			"Xilinx M2M plugin",
			"Generic M2M xrt plugin for vitis kernels",
			"Generic M2M plugin using XRT",
			"Saurabh Sengar <saurabh.singh@xilinx.com>");

	gst_element_class_add_static_pad_template (gstelement_class, &sink_template);
	gst_element_class_add_static_pad_template (gstelement_class,
			&src_request_template);

	gstelement_class->change_state =
		GST_DEBUG_FUNCPTR (gst_ivas_xm2m_change_state);
	gstelement_class->request_new_pad =
		GST_DEBUG_FUNCPTR (gst_ivas_xm2m_request_new_pad);
	gstelement_class->release_pad =
		GST_DEBUG_FUNCPTR (gst_ivas_xm2m_release_pad);

	g_object_class_install_property (gobject_class, PROP_CONFIG_LOCATION,
			g_param_spec_string ("kconfig", "config file location",
				"Location of the kernel config to program devices", NULL,
				(GParamFlags)(G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
					GST_PARAM_MUTABLE_READY)));

}

	static void
gst_ivas_xm2m_init (GstIvasXM2M * self)
{
	self->priv = GST_IVAS_XM2M_PRIVATE (self);

	self->sinkpad = gst_pad_new_from_static_template (&sink_template, "sink");
	gst_pad_set_event_function (self->sinkpad,
			GST_DEBUG_FUNCPTR (gst_ivas_xm2m_sink_event));
	gst_pad_set_chain_function (self->sinkpad,
			GST_DEBUG_FUNCPTR (gst_ivas_xm2m_chain));
	gst_pad_set_query_function (self->sinkpad,
			GST_DEBUG_FUNCPTR (gst_ivas_xm2m_sink_query));
	gst_element_add_pad (GST_ELEMENT (self), self->sinkpad);

	self->num_request_pads = 0;
	self->pad_indexes = g_hash_table_new (NULL, NULL);
	self->srcpads = NULL;
	self->priv->in_vinfo = gst_video_info_new();
	self->priv->validate_import = TRUE;
	self->priv->use_inpool = FALSE;
	gst_video_info_init (self->priv->in_vinfo);

}

	static GstPad *
gst_ivas_xm2m_request_new_pad (GstElement * element,
		GstPadTemplate * templ, const gchar * name_templ, const GstCaps * caps)
{
	GstIvasXM2M *self = GST_IVAS_XM2M (element);
	gchar *name = NULL;
	GstPad *srcpad;
	guint index = 0;

	GST_DEBUG_OBJECT (self, "requesting pad");


	GST_OBJECT_LOCK (self);

	if (GST_STATE (self) > GST_STATE_NULL) {
		GST_ERROR_OBJECT (self, "adding pads is supported only when state is NULL");
		return NULL;
	}

	if (self->num_request_pads == MAX_CHANNELS) {
		GST_ERROR_OBJECT (self, "reached maximum supported channels");
		GST_OBJECT_UNLOCK (self);
		return NULL;
	}

	if (name_templ && sscanf (name_templ, "src_%u", &index) == 1) {
		GST_LOG_OBJECT (element, "name: %s (index %d)", name_templ, index);
		if (g_hash_table_contains (self->pad_indexes, GUINT_TO_POINTER (index))) {
			GST_ERROR_OBJECT (element, "pad name %s is not unique", name_templ);
			GST_OBJECT_UNLOCK (self);
			return NULL;
		}
	} else {
		if (name_templ) {
			GST_ERROR_OBJECT (element, "incorrect padname : %s", name_templ);
			GST_OBJECT_UNLOCK (self);
			return NULL;
		}
	}

	g_hash_table_insert (self->pad_indexes, GUINT_TO_POINTER (index), NULL);

	name = g_strdup_printf ("src_%u", index);

	srcpad = GST_PAD_CAST (g_object_new (GST_TYPE_IVAS_XM2M_PAD,
				"name", name, "direction", templ->direction, "template", templ,
				NULL));
	GST_IVAS_XM2M_PAD_CAST (srcpad)->index = index;
	g_free (name);

	GST_IVAS_XM2M_PAD_CAST (srcpad)->out_vinfo = gst_video_info_new();
	gst_video_info_init (GST_IVAS_XM2M_PAD_CAST (srcpad)->out_vinfo);

	self->srcpads = g_list_append (self->srcpads,
			GST_IVAS_XM2M_PAD_CAST (srcpad));
	self->num_request_pads++;

	GST_OBJECT_UNLOCK (self);

	gst_element_add_pad (GST_ELEMENT_CAST (self), srcpad);

	return srcpad;
}

	static void
gst_ivas_xm2m_release_pad (GstElement * element, GstPad * pad)
{
	GstIvasXM2M *self;
	guint index;
	GList *lsrc = NULL;

	self = GST_IVAS_XM2M (element);

	GST_OBJECT_LOCK (self);

	if (GST_STATE (self) > GST_STATE_NULL) {
		GST_ERROR_OBJECT (self, "adding pads is supported only when state is NULL");
		return;
	}

	lsrc = g_list_find (self->srcpads, GST_IVAS_XM2M_PAD_CAST (pad));
	if (!lsrc) {
		GST_ERROR_OBJECT (self, "could not find pad to release");
		return;
	}

	gst_video_info_free(GST_IVAS_XM2M_PAD_CAST(pad)->out_vinfo);

	if (GST_IVAS_XM2M_PAD_CAST(pad)->pool)
		gst_object_unref (GST_IVAS_XM2M_PAD_CAST(pad)->pool);

	self->srcpads =
		g_list_remove (self->srcpads, GST_IVAS_XM2M_PAD_CAST (pad));
	index = GST_IVAS_XM2M_PAD_CAST (pad)->index;
	GST_DEBUG_OBJECT (self, "releasing pad with index = %d", index);

	GST_OBJECT_UNLOCK (self);

	gst_object_ref (pad);
	gst_element_remove_pad (GST_ELEMENT_CAST (self), pad);

	gst_pad_set_active (pad, FALSE);

	gst_object_unref (pad);

	self->num_request_pads--;

	GST_OBJECT_LOCK (self);
	g_hash_table_remove (self->pad_indexes, GUINT_TO_POINTER (index));
	GST_OBJECT_UNLOCK (self);
}

	static void
gst_ivas_xm2m_set_property (GObject * object, guint prop_id,
		const GValue * value, GParamSpec * pspec)
{
	GstIvasXM2M *self = GST_IVAS_XM2M (object);

	switch (prop_id) {
		case PROP_CONFIG_LOCATION:
			if (GST_STATE (self) != GST_STATE_NULL) {
				g_warning ("can't set config_file path when instance is NOT in NULL state");
				return;
			}
			self->config_file = g_value_dup_string (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

	static void
gst_ivas_xm2m_get_property (GObject * object, guint prop_id,
		GValue * value, GParamSpec * pspec)
{
	GstIvasXM2M *self = GST_IVAS_XM2M (object);

	switch (prop_id) {
		case PROP_CONFIG_LOCATION:
			g_value_set_string (value, self->config_file);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

	static GstStateChangeReturn
gst_ivas_xm2m_change_state (GstElement * element,
		GstStateChange transition)
{
	GstIvasXM2M *self = GST_IVAS_XM2M (element);
	GstStateChangeReturn ret;

	switch (transition) {
		case GST_STATE_CHANGE_NULL_TO_READY:
			if (!ivas_xm2m_open (self))
				return GST_STATE_CHANGE_FAILURE;
			break;
		case GST_STATE_CHANGE_PAUSED_TO_READY:
			break;
		default:
			break;
	}

	ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

	switch (transition) {
		case GST_STATE_CHANGE_READY_TO_NULL:
			ivas_xm2m_close (self);
			break;
		default:
			break;
	}

	return ret;
}

	static GstCaps *
gst_ivas_xm2m_fixate_caps (GstIvasXM2M * self,
		GstPadDirection direction, GstCaps * caps, GstCaps * othercaps)
{
	GstStructure *ins, *outs;
	const GValue *from_par, *to_par;
	GValue fpar = G_VALUE_INIT, tpar = G_VALUE_INIT;

	othercaps = gst_caps_truncate (othercaps);
	othercaps = gst_caps_make_writable (othercaps);

	GST_DEBUG_OBJECT (self, "trying to fixate othercaps %" GST_PTR_FORMAT
			" based on caps %" GST_PTR_FORMAT, othercaps, caps);

	ins = gst_caps_get_structure (caps, 0);
	outs = gst_caps_get_structure (othercaps, 0);

	{
		const gchar *in_format;

		in_format = gst_structure_get_string (ins, "format");
		if (in_format) {
			/* Try to set output format for pass through */
			gst_structure_fixate_field_string (outs, "format", in_format);
		}
	}

	from_par = gst_structure_get_value (ins, "pixel-aspect-ratio");
	to_par = gst_structure_get_value (outs, "pixel-aspect-ratio");

	/* If we're fixating from the sinkpad we always set the PAR and
	 * assume that missing PAR on the sinkpad means 1/1 and
	 * missing PAR on the srcpad means undefined
	 */
	if (direction == GST_PAD_SINK) {
		if (!from_par) {
			g_value_init (&fpar, GST_TYPE_FRACTION);
			gst_value_set_fraction (&fpar, 1, 1);
			from_par = &fpar;
		}
		if (!to_par) {
			g_value_init (&tpar, GST_TYPE_FRACTION_RANGE);
			gst_value_set_fraction_range_full (&tpar, 1, G_MAXINT, G_MAXINT, 1);
			to_par = &tpar;
		}
	} else {
		if (!to_par) {
			g_value_init (&tpar, GST_TYPE_FRACTION);
			gst_value_set_fraction (&tpar, 1, 1);
			to_par = &tpar;

			gst_structure_set (outs, "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
					NULL);
		}
		if (!from_par) {
			g_value_init (&fpar, GST_TYPE_FRACTION);
			gst_value_set_fraction (&fpar, 1, 1);
			from_par = &fpar;
		}
	}

	/* we have both PAR but they might not be fixated */
	{
		gint from_w, from_h, from_par_n, from_par_d, to_par_n, to_par_d;
		gint w = 0, h = 0;
		gint from_dar_n, from_dar_d;
		gint num, den;

		/* from_par should be fixed */
		g_return_val_if_fail (gst_value_is_fixed (from_par), othercaps);

		from_par_n = gst_value_get_fraction_numerator (from_par);
		from_par_d = gst_value_get_fraction_denominator (from_par);

		gst_structure_get_int (ins, "width", &from_w);
		gst_structure_get_int (ins, "height", &from_h);

		gst_structure_get_int (outs, "width", &w);
		gst_structure_get_int (outs, "height", &h);

		/* if both width and height are already fixed, we can't do anything
		 * about it anymore */
		if (w && h) {
			guint n, d;

			GST_DEBUG_OBJECT (self, "dimensions already set to %dx%d, not fixating",
					w, h);
			if (!gst_value_is_fixed (to_par)) {
				if (gst_video_calculate_display_ratio (&n, &d, from_w, from_h,
							from_par_n, from_par_d, w, h)) {
					GST_DEBUG_OBJECT (self, "fixating to_par to %dx%d", n, d);
					if (gst_structure_has_field (outs, "pixel-aspect-ratio"))
						gst_structure_fixate_field_nearest_fraction (outs,
								"pixel-aspect-ratio", n, d);
					else if (n != d)
						gst_structure_set (outs, "pixel-aspect-ratio", GST_TYPE_FRACTION,
								n, d, NULL);
				}
			}
			goto done;
		}

		/* Calculate input DAR */
		if (!gst_util_fraction_multiply (from_w, from_h, from_par_n, from_par_d,
					&from_dar_n, &from_dar_d)) {
			GST_ELEMENT_ERROR (self, CORE, NEGOTIATION, (NULL),
					("Error calculating the output scaled size - integer overflow"));
			goto done;
		}

		GST_DEBUG_OBJECT (self, "Input DAR is %d/%d", from_dar_n, from_dar_d);

		/* If either width or height are fixed there's not much we
		 * can do either except choosing a height or width and PAR
		 * that matches the DAR as good as possible
		 */
		if (h) {
			GstStructure *tmp;
			gint set_w, set_par_n, set_par_d;

			GST_DEBUG_OBJECT (self, "height is fixed (%d)", h);

			/* If the PAR is fixed too, there's not much to do
			 * except choosing the width that is nearest to the
			 * width with the same DAR */
			if (gst_value_is_fixed (to_par)) {
				to_par_n = gst_value_get_fraction_numerator (to_par);
				to_par_d = gst_value_get_fraction_denominator (to_par);

				GST_DEBUG_OBJECT (self, "PAR is fixed %d/%d", to_par_n, to_par_d);

				if (!gst_util_fraction_multiply (from_dar_n, from_dar_d, to_par_d,
							to_par_n, &num, &den)) {
					GST_ELEMENT_ERROR (self, CORE, NEGOTIATION, (NULL),
							("Error calculating the output scaled size - integer overflow"));
					goto done;
				}

				w = (guint) gst_util_uint64_scale_int (h, num, den);
				gst_structure_fixate_field_nearest_int (outs, "width", w);

				goto done;
			}

			/* The PAR is not fixed and it's quite likely that we can set
			 * an arbitrary PAR. */

			/* Check if we can keep the input width */
			tmp = gst_structure_copy (outs);
			gst_structure_fixate_field_nearest_int (tmp, "width", from_w);
			gst_structure_get_int (tmp, "width", &set_w);

			/* Might have failed but try to keep the DAR nonetheless by
			 * adjusting the PAR */
			if (!gst_util_fraction_multiply (from_dar_n, from_dar_d, h, set_w,
						&to_par_n, &to_par_d)) {
				GST_ELEMENT_ERROR (self, CORE, NEGOTIATION, (NULL),
						("Error calculating the output scaled size - integer overflow"));
				gst_structure_free (tmp);
				goto done;
			}

			if (!gst_structure_has_field (tmp, "pixel-aspect-ratio"))
				gst_structure_set_value (tmp, "pixel-aspect-ratio", to_par);
			gst_structure_fixate_field_nearest_fraction (tmp, "pixel-aspect-ratio",
					to_par_n, to_par_d);
			gst_structure_get_fraction (tmp, "pixel-aspect-ratio", &set_par_n,
					&set_par_d);
			gst_structure_free (tmp);

			/* Check if the adjusted PAR is accepted */
			if (set_par_n == to_par_n && set_par_d == to_par_d) {
				if (gst_structure_has_field (outs, "pixel-aspect-ratio") ||
						set_par_n != set_par_d)
					gst_structure_set (outs, "width", G_TYPE_INT, set_w,
							"pixel-aspect-ratio", GST_TYPE_FRACTION, set_par_n, set_par_d,
							NULL);
				goto done;
			}

			/* Otherwise scale the width to the new PAR and check if the
			 * adjusted with is accepted. If all that fails we can't keep
			 * the DAR */
			if (!gst_util_fraction_multiply (from_dar_n, from_dar_d, set_par_d,
						set_par_n, &num, &den)) {
				GST_ELEMENT_ERROR (self, CORE, NEGOTIATION, (NULL),
						("Error calculating the output scaled size - integer overflow"));
				goto done;
			}

			w = (guint) gst_util_uint64_scale_int (h, num, den);
			gst_structure_fixate_field_nearest_int (outs, "width", w);
			if (gst_structure_has_field (outs, "pixel-aspect-ratio") ||
					set_par_n != set_par_d)
				gst_structure_set (outs, "pixel-aspect-ratio", GST_TYPE_FRACTION,
						set_par_n, set_par_d, NULL);

			goto done;
		} else if (w) {
			GstStructure *tmp;
			gint set_h, set_par_n, set_par_d;

			GST_DEBUG_OBJECT (self, "width is fixed (%d)", w);

			/* If the PAR is fixed too, there's not much to do
			 * except choosing the height that is nearest to the
			 * height with the same DAR */
			if (gst_value_is_fixed (to_par)) {
				to_par_n = gst_value_get_fraction_numerator (to_par);
				to_par_d = gst_value_get_fraction_denominator (to_par);

				GST_DEBUG_OBJECT (self, "PAR is fixed %d/%d", to_par_n, to_par_d);

				if (!gst_util_fraction_multiply (from_dar_n, from_dar_d, to_par_d,
							to_par_n, &num, &den)) {
					GST_ELEMENT_ERROR (self, CORE, NEGOTIATION, (NULL),
							("Error calculating the output scaled size - integer overflow"));
					goto done;
				}

				h = (guint) gst_util_uint64_scale_int (w, den, num);
				gst_structure_fixate_field_nearest_int (outs, "height", h);

				goto done;
			}

			/* The PAR is not fixed and it's quite likely that we can set
			 * an arbitrary PAR. */

			/* Check if we can keep the input height */
			tmp = gst_structure_copy (outs);
			gst_structure_fixate_field_nearest_int (tmp, "height", from_h);
			gst_structure_get_int (tmp, "height", &set_h);

			/* Might have failed but try to keep the DAR nonetheless by
			 * adjusting the PAR */
			if (!gst_util_fraction_multiply (from_dar_n, from_dar_d, set_h, w,
						&to_par_n, &to_par_d)) {
				GST_ELEMENT_ERROR (self, CORE, NEGOTIATION, (NULL),
						("Error calculating the output scaled size - integer overflow"));
				gst_structure_free (tmp);
				goto done;
			}
			if (!gst_structure_has_field (tmp, "pixel-aspect-ratio"))
				gst_structure_set_value (tmp, "pixel-aspect-ratio", to_par);
			gst_structure_fixate_field_nearest_fraction (tmp, "pixel-aspect-ratio",
					to_par_n, to_par_d);
			gst_structure_get_fraction (tmp, "pixel-aspect-ratio", &set_par_n,
					&set_par_d);
			gst_structure_free (tmp);

			/* Check if the adjusted PAR is accepted */
			if (set_par_n == to_par_n && set_par_d == to_par_d) {
				if (gst_structure_has_field (outs, "pixel-aspect-ratio") ||
						set_par_n != set_par_d)
					gst_structure_set (outs, "height", G_TYPE_INT, set_h,
							"pixel-aspect-ratio", GST_TYPE_FRACTION, set_par_n, set_par_d,
							NULL);
				goto done;
			}

			/* Otherwise scale the height to the new PAR and check if the
			 * adjusted with is accepted. If all that fails we can't keep
			 * the DAR */
			if (!gst_util_fraction_multiply (from_dar_n, from_dar_d, set_par_d,
						set_par_n, &num, &den)) {
				GST_ELEMENT_ERROR (self, CORE, NEGOTIATION, (NULL),
						("Error calculating the output scaled size - integer overflow"));
				goto done;
			}

			h = (guint) gst_util_uint64_scale_int (w, den, num);
			gst_structure_fixate_field_nearest_int (outs, "height", h);
			if (gst_structure_has_field (outs, "pixel-aspect-ratio") ||
					set_par_n != set_par_d)
				gst_structure_set (outs, "pixel-aspect-ratio", GST_TYPE_FRACTION,
						set_par_n, set_par_d, NULL);

			goto done;
		} else if (gst_value_is_fixed (to_par)) {
			GstStructure *tmp;
			gint set_h, set_w, f_h, f_w;

			to_par_n = gst_value_get_fraction_numerator (to_par);
			to_par_d = gst_value_get_fraction_denominator (to_par);

			GST_DEBUG_OBJECT (self, "PAR is fixed %d/%d", to_par_n, to_par_d);

			/* Calculate scale factor for the PAR change */
			if (!gst_util_fraction_multiply (from_dar_n, from_dar_d, to_par_d,
						to_par_n, &num, &den)) {
				GST_ELEMENT_ERROR (self, CORE, NEGOTIATION, (NULL),
						("Error calculating the output scaled size - integer overflow"));
				goto done;
			}

			/* Try to keep the input height (because of interlacing) */
			tmp = gst_structure_copy (outs);
			gst_structure_fixate_field_nearest_int (tmp, "height", from_h);
			gst_structure_get_int (tmp, "height", &set_h);

			/* This might have failed but try to scale the width
			 * to keep the DAR nonetheless */
			w = (guint) gst_util_uint64_scale_int (set_h, num, den);
			gst_structure_fixate_field_nearest_int (tmp, "width", w);
			gst_structure_get_int (tmp, "width", &set_w);
			gst_structure_free (tmp);

			/* We kept the DAR and the height is nearest to the original height */
			if (set_w == w) {
				gst_structure_set (outs, "width", G_TYPE_INT, set_w, "height",
						G_TYPE_INT, set_h, NULL);
				goto done;
			}

			f_h = set_h;
			f_w = set_w;

			/* If the former failed, try to keep the input width at least */
			tmp = gst_structure_copy (outs);
			gst_structure_fixate_field_nearest_int (tmp, "width", from_w);
			gst_structure_get_int (tmp, "width", &set_w);

			/* This might have failed but try to scale the width
			 * to keep the DAR nonetheless */
			h = (guint) gst_util_uint64_scale_int (set_w, den, num);
			gst_structure_fixate_field_nearest_int (tmp, "height", h);
			gst_structure_get_int (tmp, "height", &set_h);
			gst_structure_free (tmp);

			/* We kept the DAR and the width is nearest to the original width */
			if (set_h == h) {
				gst_structure_set (outs, "width", G_TYPE_INT, set_w, "height",
						G_TYPE_INT, set_h, NULL);
				goto done;
			}

			/* If all this failed, keep the height that was nearest to the orignal
			 * height and the nearest possible width. This changes the DAR but
			 * there's not much else to do here.
			 */
			gst_structure_set (outs, "width", G_TYPE_INT, f_w, "height", G_TYPE_INT,
					f_h, NULL);
			goto done;
		} else {
			GstStructure *tmp;
			gint set_h, set_w, set_par_n, set_par_d, tmp2;

			/* width, height and PAR are not fixed but passthrough is not possible */

			/* First try to keep the height and width as good as possible
			 * and scale PAR */
			tmp = gst_structure_copy (outs);
			gst_structure_fixate_field_nearest_int (tmp, "height", from_h);
			gst_structure_get_int (tmp, "height", &set_h);
			gst_structure_fixate_field_nearest_int (tmp, "width", from_w);
			gst_structure_get_int (tmp, "width", &set_w);

			if (!gst_util_fraction_multiply (from_dar_n, from_dar_d, set_h, set_w,
						&to_par_n, &to_par_d)) {
				GST_ELEMENT_ERROR (self, CORE, NEGOTIATION, (NULL),
						("Error calculating the output scaled size - integer overflow"));
				gst_structure_free (tmp);
				goto done;
			}

			if (!gst_structure_has_field (tmp, "pixel-aspect-ratio"))
				gst_structure_set_value (tmp, "pixel-aspect-ratio", to_par);
			gst_structure_fixate_field_nearest_fraction (tmp, "pixel-aspect-ratio",
					to_par_n, to_par_d);
			gst_structure_get_fraction (tmp, "pixel-aspect-ratio", &set_par_n,
					&set_par_d);
			gst_structure_free (tmp);

			if (set_par_n == to_par_n && set_par_d == to_par_d) {
				gst_structure_set (outs, "width", G_TYPE_INT, set_w, "height",
						G_TYPE_INT, set_h, NULL);

				if (gst_structure_has_field (outs, "pixel-aspect-ratio") ||
						set_par_n != set_par_d)
					gst_structure_set (outs, "pixel-aspect-ratio", GST_TYPE_FRACTION,
							set_par_n, set_par_d, NULL);
				goto done;
			}

			/* Otherwise try to scale width to keep the DAR with the set
			 * PAR and height */
			if (!gst_util_fraction_multiply (from_dar_n, from_dar_d, set_par_d,
						set_par_n, &num, &den)) {
				GST_ELEMENT_ERROR (self, CORE, NEGOTIATION, (NULL),
						("Error calculating the output scaled size - integer overflow"));
				goto done;
			}

			w = (guint) gst_util_uint64_scale_int (set_h, num, den);
			tmp = gst_structure_copy (outs);
			gst_structure_fixate_field_nearest_int (tmp, "width", w);
			gst_structure_get_int (tmp, "width", &tmp2);
			gst_structure_free (tmp);

			if (tmp2 == w) {
				gst_structure_set (outs, "width", G_TYPE_INT, tmp2, "height",
						G_TYPE_INT, set_h, NULL);
				if (gst_structure_has_field (outs, "pixel-aspect-ratio") ||
						set_par_n != set_par_d)
					gst_structure_set (outs, "pixel-aspect-ratio", GST_TYPE_FRACTION,
							set_par_n, set_par_d, NULL);
				goto done;
			}

			/* ... or try the same with the height */
			h = (guint) gst_util_uint64_scale_int (set_w, den, num);
			tmp = gst_structure_copy (outs);
			gst_structure_fixate_field_nearest_int (tmp, "height", h);
			gst_structure_get_int (tmp, "height", &tmp2);
			gst_structure_free (tmp);

			if (tmp2 == h) {
				gst_structure_set (outs, "width", G_TYPE_INT, set_w, "height",
						G_TYPE_INT, tmp2, NULL);
				if (gst_structure_has_field (outs, "pixel-aspect-ratio") ||
						set_par_n != set_par_d)
					gst_structure_set (outs, "pixel-aspect-ratio", GST_TYPE_FRACTION,
							set_par_n, set_par_d, NULL);
				goto done;
			}

			/* If all fails we can't keep the DAR and take the nearest values
			 * for everything from the first try */
			gst_structure_set (outs, "width", G_TYPE_INT, set_w, "height",
					G_TYPE_INT, set_h, NULL);
			if (gst_structure_has_field (outs, "pixel-aspect-ratio") ||
					set_par_n != set_par_d)
				gst_structure_set (outs, "pixel-aspect-ratio", GST_TYPE_FRACTION,
						set_par_n, set_par_d, NULL);
		}
	}

done:
	GST_DEBUG_OBJECT (self, "fixated othercaps to %" GST_PTR_FORMAT, othercaps);

	if (from_par == &fpar)
		g_value_unset (&fpar);
	if (to_par == &tpar)
		g_value_unset (&tpar);

	/* fixate remaining fields */
	othercaps = gst_caps_fixate (othercaps);

	if (direction == GST_PAD_SINK) {
		if (gst_caps_is_subset (caps, othercaps)) {
			gst_caps_replace (&othercaps, caps);
		}
	}

	return othercaps;
}

	static GstCaps *
gst_ivas_xm2m_transform_caps (GstIvasXM2M * self,
		GstPadDirection direction, GstCaps * caps, GstCaps * filter)
{
	GstCaps *ret;
	GstStructure *structure;
	GstCapsFeatures *features;
	gint i, n;

	GST_DEBUG_OBJECT (self,
			"Transforming caps %" GST_PTR_FORMAT " in direction %s", caps,
			(direction == GST_PAD_SINK) ? "sink" : "src");

	ret = gst_caps_new_empty ();
	n = gst_caps_get_size (caps);
	for (i = 0; i < n; i++) {
		structure = gst_caps_get_structure (caps, i);
		features = gst_caps_get_features (caps, i);

		/* If this is already expressed by the existing caps
		 * skip this structure */
		if (i > 0 && gst_caps_is_subset_structure_full (ret, structure, features))
			continue;

		/* make copy */
		structure = gst_structure_copy (structure);

		/* If the features are non-sysmem we can only do passthrough */
		if (!gst_caps_features_is_any (features)
				&& gst_caps_features_is_equal (features,
					GST_CAPS_FEATURES_MEMORY_SYSTEM_MEMORY)) {
			gst_structure_set (structure, "width", GST_TYPE_INT_RANGE, 1, G_MAXINT,
					"height", GST_TYPE_INT_RANGE, 1, G_MAXINT, NULL);

			gst_structure_remove_fields (structure, "format", "colorimetry", "chroma-site",
					NULL);
			/* if pixel aspect ratio, make a range of it */
			if (gst_structure_has_field (structure, "pixel-aspect-ratio")) {
				gst_structure_set (structure, "pixel-aspect-ratio",
						GST_TYPE_FRACTION_RANGE, 1, G_MAXINT, G_MAXINT, 1, NULL);
			}
		}
		gst_caps_append_structure_full (ret, structure,
				gst_caps_features_copy (features));
	}

	if (filter) {
		GstCaps *intersection;

		intersection =
			gst_caps_intersect_full (filter, ret, GST_CAPS_INTERSECT_FIRST);
		gst_caps_unref (ret);
		ret = intersection;
	}

	GST_DEBUG_OBJECT (self, "returning caps: %" GST_PTR_FORMAT, ret);

	return ret;
}

	static GstCaps *
gst_ivas_xm2m_find_transform (GstIvasXM2M * self, GstPad * pad,
		GstPad * otherpad, GstCaps * caps)
{
	GstPad *otherpeer;
	GstCaps *othercaps;
	gboolean is_fixed;

	/* caps must be fixed here, this is a programming error if it's not */
	g_return_val_if_fail (gst_caps_is_fixed (caps), NULL);

	otherpeer = gst_pad_get_peer (otherpad);

	/* see how we can transform the input caps. We need to do this even for
	 * passthrough because it might be possible that this element cannot support
	 * passthrough at all. */
	othercaps = gst_ivas_xm2m_transform_caps (self,
			GST_PAD_DIRECTION (pad), caps, NULL);

	/* The caps we can actually output is the intersection of the transformed
	 * caps with the pad template for the pad */
	if (othercaps && !gst_caps_is_empty (othercaps)) {
		GstCaps *intersect, *templ_caps;

		templ_caps = gst_pad_get_pad_template_caps (otherpad);
		GST_DEBUG_OBJECT (self,
				"intersecting against padtemplate %" GST_PTR_FORMAT, templ_caps);

		intersect =
			gst_caps_intersect_full (othercaps, templ_caps,
					GST_CAPS_INTERSECT_FIRST);

		gst_caps_unref (othercaps);
		gst_caps_unref (templ_caps);
		othercaps = intersect;
	}

	/* check if transform is empty */
	if (!othercaps || gst_caps_is_empty (othercaps))
		goto no_transform;

	/* if the othercaps are not fixed, we need to fixate them, first attempt
	 * is by attempting passthrough if the othercaps are a superset of caps. */
	/* FIXME. maybe the caps is not fixed because it has multiple structures of
	 * fixed caps */
	is_fixed = gst_caps_is_fixed (othercaps);
	if (!is_fixed) {
		GST_DEBUG_OBJECT (self,
				"transform returned non fixed  %" GST_PTR_FORMAT, othercaps);

		/* Now let's see what the peer suggests based on our transformed caps */
		if (otherpeer) {
			GstCaps *peercaps, *intersection, *templ_caps;

			GST_DEBUG_OBJECT (self,
					"Checking peer caps with filter %" GST_PTR_FORMAT, othercaps);

			peercaps = gst_pad_query_caps (otherpeer, othercaps);
			GST_DEBUG_OBJECT (self, "Resulted in %" GST_PTR_FORMAT, peercaps);
			if (!gst_caps_is_empty (peercaps)) {
				templ_caps = gst_pad_get_pad_template_caps (otherpad);

				GST_DEBUG_OBJECT (self,
						"Intersecting with template caps %" GST_PTR_FORMAT, templ_caps);

				intersection =
					gst_caps_intersect_full (peercaps, templ_caps,
							GST_CAPS_INTERSECT_FIRST);
				GST_DEBUG_OBJECT (self, "Intersection: %" GST_PTR_FORMAT, intersection);
				gst_caps_unref (peercaps);
				gst_caps_unref (templ_caps);
				peercaps = intersection;

				GST_DEBUG_OBJECT (self,
						"Intersecting with transformed caps %" GST_PTR_FORMAT, othercaps);
				intersection =
					gst_caps_intersect_full (peercaps, othercaps,
							GST_CAPS_INTERSECT_FIRST);
				GST_DEBUG_OBJECT (self, "Intersection: %" GST_PTR_FORMAT, intersection);
				gst_caps_unref (peercaps);
				gst_caps_unref (othercaps);
				othercaps = intersection;
			} else {
				gst_caps_unref (othercaps);
				othercaps = peercaps;
			}

			is_fixed = gst_caps_is_fixed (othercaps);
		} else {
			goto no_transform_possible;
		}
	}
	if (gst_caps_is_empty (othercaps))
		goto no_transform_possible;

	GST_DEBUG ("have %s fixed caps %" GST_PTR_FORMAT, (is_fixed ? "" : "non-"),
			othercaps);

	/* second attempt at fixation, call the fixate vmethod */
	/* caps could be fixed but the subclass may want to add fields */
	GST_DEBUG_OBJECT (self, "calling fixate_caps for %" GST_PTR_FORMAT
			" using caps %" GST_PTR_FORMAT " on pad %s:%s", othercaps, caps,
			GST_DEBUG_PAD_NAME (otherpad));
	/* note that we pass the complete array of structures to the fixate
	 * function, it needs to truncate itself */
	othercaps =
		gst_ivas_xm2m_fixate_caps (self, GST_PAD_DIRECTION (pad), caps,
				othercaps);
	is_fixed = gst_caps_is_fixed (othercaps);
	GST_DEBUG_OBJECT (self, "after fixating %" GST_PTR_FORMAT, othercaps);

	/* caps should be fixed now, if not we have to fail. */
	if (!is_fixed)
		goto could_not_fixate;

	/* and peer should accept */
	if (otherpeer && !gst_pad_query_accept_caps (otherpeer, othercaps))
		goto peer_no_accept;

	GST_DEBUG_OBJECT (self, "Input caps were %" GST_PTR_FORMAT
			", and got final caps %" GST_PTR_FORMAT, caps, othercaps);

	if (otherpeer)
		gst_object_unref (otherpeer);

	return othercaps;

	/* ERRORS */
no_transform:
	{
		GST_DEBUG_OBJECT (self,
				"transform returned useless  %" GST_PTR_FORMAT, othercaps);
		goto error_cleanup;
	}
no_transform_possible:
	{
		GST_DEBUG_OBJECT (self,
				"transform could not transform %" GST_PTR_FORMAT
				" in anything we support", caps);
		goto error_cleanup;
	}
could_not_fixate:
	{
		GST_DEBUG_OBJECT (self, "FAILED to fixate %" GST_PTR_FORMAT, othercaps);
		goto error_cleanup;
	}
peer_no_accept:
	{
		GST_DEBUG_OBJECT (self, "FAILED to get peer of %" GST_PTR_FORMAT
				" to accept %" GST_PTR_FORMAT, otherpad, othercaps);
		goto error_cleanup;
	}
error_cleanup:
	{
		if (otherpeer)
			gst_object_unref (otherpeer);
		if (othercaps)
			gst_caps_unref (othercaps);
		return NULL;
	}
}

	static gboolean
ivas_xm2m_decide_allocation (GstIvasXM2MPad *srcpad, GstQuery * query, GstCaps *outcaps)
{
	GstAllocator *allocator = NULL;
	GstAllocationParams params;
	GstBufferPool *pool = NULL;
	guint size, min, max;
	gboolean update_allocator, update_pool, bret;
	GstStructure *config = NULL;

	/* we got configuration from our peer or the decide_allocation method,
	 * parse them */
	if (gst_query_get_n_allocation_params (query) > 0) {
		/* try the allocator */
		gst_query_parse_nth_allocation_param (query, 0, &allocator, &params);
		GST_DEBUG_OBJECT (srcpad, "has allocator %p", allocator);
		update_allocator = TRUE;
	} else {
		allocator = NULL;
		update_allocator = FALSE;
		gst_allocation_params_init (&params);
	}

	if (gst_query_get_n_allocation_pools (query) > 0) {
		gst_query_parse_nth_allocation_pool (query, 0, &pool, &size, &min, &max);
		update_pool = TRUE;
		min += 3;
		max += 5;
	} else {
		GstVideoInfo info;

		if (!gst_video_info_from_caps (&info, outcaps)) {
			GST_ERROR_OBJECT (srcpad, "invalid caps");
			goto error;
		}

		pool = NULL;
		min = 3;
		max = 5;
		size = info.size;
		update_pool = FALSE;
	}

	if (pool) {
		GstStructure *config = gst_buffer_pool_get_config (pool);

		if (gst_buffer_pool_config_has_option (config, "GstBufferPoolOptionKMSPrimeExport")) {
			gst_structure_free (config);
			goto next;
		}

		gst_structure_free (config);
		gst_object_unref (pool);
		pool = NULL;
		GST_DEBUG_OBJECT (srcpad,
				"pool deos not have the KMSPrimeExport option, \
				unref the pool and create sdx allocator");
	}

	if (allocator && (!GST_IS_IVAS_ALLOCATOR (allocator) ||
				gst_ivas_allocator_get_device_idx(allocator) != DEFAULT_DEVICE_INDEX)) {
		GST_DEBUG_OBJECT (srcpad, "replace %" GST_PTR_FORMAT " to xrt allocator",
				allocator);
		gst_object_unref (allocator);
		gst_allocation_params_init (&params);
		allocator = NULL;
	}

	if (!allocator) {
		/* making sdx allocator for the HW mode without dmabuf */
		allocator = gst_ivas_allocator_new (DEFAULT_DEVICE_INDEX, TRUE);
		params.flags = GST_MEMORY_FLAG_PHYSICALLY_CONTIGUOUS;
		GST_INFO_OBJECT (srcpad, "creating new xrt allocator %" GST_PTR_FORMAT, allocator);
	}

next:
	if (update_allocator)
		gst_query_set_nth_allocation_param (query, 0, allocator, &params);
	else
		gst_query_add_allocation_param (query, allocator, &params);

	if (pool == NULL) {
		GST_DEBUG_OBJECT (srcpad, "no pool, making new pool");
		pool = gst_video_buffer_pool_new ();
	}

	config = gst_buffer_pool_get_config (pool);
	gst_buffer_pool_config_set_params (config, outcaps, size, min, max);
	gst_buffer_pool_config_set_allocator (config, allocator, &params);
	gst_buffer_pool_config_add_option (config, GST_BUFFER_POOL_OPTION_VIDEO_META);
	bret = gst_buffer_pool_set_config (pool, config);
	if (!bret) {
		GST_ERROR_OBJECT (srcpad, "failed configure pool");
		goto error;
	}

	if (allocator)
		gst_object_unref (allocator);

	if (update_pool)
		gst_query_set_nth_allocation_pool (query, 0, pool, size, min, max);
	else
		gst_query_add_allocation_pool (query, pool, size, min, max);

	srcpad->pool = pool;

	GST_DEBUG_OBJECT (srcpad, "done decide allocation with query %" GST_PTR_FORMAT, query);
	return TRUE;

error:
	if (allocator)
		gst_object_unref (allocator);
	if (pool)
		gst_object_unref (pool);
	return FALSE;
}

	static gboolean
gst_ivas_xm2m_sink_setcaps (GstIvasXM2M * self, GstPad * sinkpad,
		GstCaps * in_caps)
{
	GstCaps *outcaps = NULL, *prev_incaps = NULL, *prev_outcaps = NULL;
	gboolean bret = TRUE;
	guint idx = 0;
	GstIvasXM2MPad *srcpad = NULL;
	GstCaps *incaps = gst_caps_copy (in_caps);

	GST_DEBUG_OBJECT (self, "have new sink caps %p %" GST_PTR_FORMAT, incaps,
			incaps);

	self->priv->validate_import = TRUE;

	/* store sinkpad info */
	if (!gst_video_info_from_caps (self->priv->in_vinfo, in_caps)) {
		GST_ERROR_OBJECT (self, "failed to get video info from input caps");
		return FALSE;
	}

	prev_incaps = gst_pad_get_current_caps (self->sinkpad);

	for (idx = 0; idx < g_list_length (self->srcpads); idx++) {
		srcpad = gst_ivas_xm2m_srcpad_at_index (self, idx);

		/* find best possible caps for the other pad */
		outcaps =
			gst_ivas_xm2m_find_transform (self, sinkpad,
					GST_PAD_CAST (srcpad), incaps);
		if (!outcaps || gst_caps_is_empty (outcaps))
			goto no_transform_possible;

		prev_outcaps = gst_pad_get_current_caps (GST_PAD_CAST (srcpad));

		bret = prev_incaps && prev_outcaps && gst_caps_is_equal (prev_incaps, incaps)
			&& gst_caps_is_equal (prev_outcaps, outcaps);

		if (bret) {
			GST_DEBUG_OBJECT (self,
					"New caps equal to old ones: %" GST_PTR_FORMAT " -> %" GST_PTR_FORMAT,
					incaps, outcaps);
		} else {
			GstQuery *query = NULL;

			if (!prev_outcaps || !gst_caps_is_equal (outcaps, prev_outcaps)) {
				/* let downstream know about our caps */
				bret = gst_pad_set_caps (GST_PAD_CAST (srcpad), outcaps);
				if (!bret)
					goto failed_configure;
			}

			GST_DEBUG_OBJECT (self, "doing allocation query");
			query = gst_query_new_allocation (outcaps, TRUE);
			if (!gst_pad_peer_query (GST_PAD(srcpad), query)) {
				/* not a problem, just debug a little */
				GST_DEBUG_OBJECT (self, "peer ALLOCATION query failed");
			}

			bret = ivas_xm2m_decide_allocation (srcpad, query, outcaps);
			if (!bret)
				goto failed_configure;

			bret = ivas_xm2m_register_prep_write_with_caps (self, idx, incaps, outcaps);
			if (!bret)
				goto failed_configure;
		}

		gst_caps_unref (incaps);
		incaps = outcaps;

		if (prev_outcaps) {
			gst_caps_unref (prev_outcaps);
			prev_outcaps = NULL;
		}
	}

done:
	if (outcaps)
		gst_caps_unref (outcaps);
	if (prev_incaps)
		gst_caps_unref (prev_incaps);
	if (prev_outcaps)
		gst_caps_unref (prev_outcaps);

	return bret;

	/* ERRORS */
no_transform_possible:
	{
		GST_ERROR_OBJECT (self,
				"transform could not transform %" GST_PTR_FORMAT
				" in anything we support", incaps);
		bret = FALSE;
		goto done;
	}
failed_configure:
	{
		GST_ERROR_OBJECT (self, "FAILED to configure incaps %" GST_PTR_FORMAT
				" and outcaps %" GST_PTR_FORMAT, incaps, outcaps);
		bret = FALSE;
		goto done;
	}
}

	static gboolean
gst_ivas_xm2m_sink_event (GstPad * pad, GstObject * parent,
		GstEvent * event)
{
	GstIvasXM2M *self = GST_IVAS_XM2M (parent);
	gboolean ret = TRUE;

	GST_DEBUG_OBJECT (pad, "received event '%s' %p %" GST_PTR_FORMAT,
			gst_event_type_get_name (GST_EVENT_TYPE (event)), event, event);

	switch (GST_EVENT_TYPE (event)) {
		case GST_EVENT_CAPS:{
					    GstCaps *caps;

					    gst_event_parse_caps (event, &caps);
					    ret = gst_ivas_xm2m_sink_setcaps (self, self->sinkpad, caps);
					    gst_event_unref (event);
					    break;
				    }
		default:
				    ret = gst_pad_event_default (pad, parent, event);
				    break;
	}

	return ret;
}

	static gboolean
ivas_xm2m_propose_allocation (GstIvasXM2M * self, GstQuery * query)
{
	GstCaps *caps;
	GstVideoInfo info;
	GstBufferPool *pool;
	guint size;

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
			0 };

		if (gst_query_get_n_allocation_params (query) > 0) {
			gst_query_parse_nth_allocation_param (query, 0, &allocator, &params);
		} else {
			allocator = gst_ivas_allocator_new (DEFAULT_DEVICE_INDEX, TRUE);
			gst_query_add_allocation_param (query, allocator, &params);
		}

		pool = gst_video_buffer_pool_new ();
		GST_LOG_OBJECT (self, "allocated internal sink pool %p", pool);

		structure = gst_buffer_pool_get_config (pool);
		gst_buffer_pool_config_set_params (structure, caps, size, 3, 0);
		gst_buffer_pool_config_set_allocator (structure, allocator, &params);

		gst_buffer_pool_config_add_option (structure, GST_BUFFER_POOL_OPTION_VIDEO_META);

		if (allocator)
			gst_object_unref (allocator);

		if (!gst_buffer_pool_set_config (pool, structure))
			goto config_failed;

		GST_OBJECT_LOCK (self);
		gst_query_add_allocation_pool (query, pool, size, 3, 0);
		GST_OBJECT_UNLOCK (self);

		if (self->priv->input_pool)
			gst_object_unref (self->priv->input_pool);

		self->priv->input_pool = pool;
		gst_query_add_allocation_pool (query, pool, size, 3, 0);
		gst_query_add_allocation_meta (query, GST_VIDEO_META_API_TYPE, NULL);
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
gst_ivas_xm2m_sink_query (GstPad * pad, GstObject * parent,
		GstQuery * query)
{
	GstIvasXM2M *self = GST_IVAS_XM2M (parent);
	gboolean ret = TRUE;
	GstIvasXM2MPad *srcpad = NULL;

	switch (GST_QUERY_TYPE (query)) {
		case GST_QUERY_ALLOCATION:{
						  ret = ivas_xm2m_propose_allocation (self, query);
						  break;
					  }
		case GST_QUERY_CAPS:{
					    GstCaps *filter, *caps = NULL, *result = NULL;

					    gst_query_parse_caps (query, &filter);

					    // TODO: Query caps only going to src pad 0 and check for others
					    if (!g_list_length (self->srcpads)) { 
						    GST_DEBUG_OBJECT (pad, "0 source pads in list");
						    return FALSE;
					    }

					    srcpad = gst_ivas_xm2m_srcpad_at_index (self, 0);
					    if (!srcpad) {
						    GST_ERROR_OBJECT (pad, "source pads not available..");
						    return FALSE;
					    }

					    caps = gst_pad_get_pad_template_caps (pad);

					    if (filter) {
						    GstCaps *tmp = caps;
						    caps = gst_caps_intersect_full (filter, tmp, GST_CAPS_INTERSECT_FIRST);
						    gst_caps_unref (tmp);
					    }

					    if (srcpad) {
						    result = gst_pad_peer_query_caps (GST_PAD_CAST (srcpad), caps);
						    result = gst_caps_make_writable (result);
						    gst_caps_append (result, caps);

						    GST_DEBUG_OBJECT (self, "Returning %s caps %" GST_PTR_FORMAT,
								    GST_PAD_NAME (pad), result);

						    gst_query_set_caps_result (query, result);
						    gst_caps_unref (result);
					    }
					    break;
				    }
		case GST_QUERY_ACCEPT_CAPS:
				    {
					    GstCaps *caps;

					    gst_query_parse_accept_caps (query, &caps);

					    gst_query_set_accept_caps_result (query, ret);
					    /* return TRUE, we answered the query */
					    ret = TRUE;
					    break;
				    }
		default:
				    ret = gst_pad_query_default (pad, parent, query);
				    break;
	}

	return ret;
}

	static GstFlowReturn
gst_ivas_xm2m_chain (GstPad * pad, GstObject * parent, GstBuffer * inbuf)
{
	GstIvasXM2M *self = GST_IVAS_XM2M (parent);
	GstFlowReturn fret = GST_FLOW_OK;
	guint chan_id = 0;
	gboolean bret = FALSE;
#if PROFILING
	uint64_t delta_us;
#endif

	bret = ivas_xm2m_write_input_registers (self, &inbuf);
	if (!bret)
		goto error;

	bret = ivas_xm2m_write_output_registers (self);
	if (!bret)
		goto error;

	pthread_mutex_lock(&count_mutex); /* lock for TDM */
#if PROFILING
	f_num++;
	if (f_num == 1)
	        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	if (f_num == 1000) {
        	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        	delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
		GST_INFO_OBJECT (self, "IVAS M2M %d fps %ld\n", f_num, 1000000/(delta_us/1000));
		f_num = 0;
	}
#endif
	bret = ivas_xm2m_process (self);
	if (!bret)
		goto error;

	pthread_mutex_unlock(&count_mutex);

	/* pad push of each output buffer to respective srcpad */
	for (chan_id = 0; chan_id < self->num_request_pads; chan_id++) {
		GstBuffer *outbuf = self->priv->outbufs[chan_id];
		GstIvasXM2MPad *srcpad = gst_ivas_xm2m_srcpad_at_index (self, chan_id);

		gst_buffer_copy_into (outbuf, inbuf,
				(GstBufferCopyFlags) (GST_BUFFER_COPY_FLAGS | GST_BUFFER_COPY_TIMESTAMPS), 0, -1);
		GST_LOG_OBJECT (srcpad,
				"pushing outbuf %p with pts = %" GST_TIME_FORMAT " dts = %"
				GST_TIME_FORMAT " duration = %" GST_TIME_FORMAT, outbuf,
				GST_TIME_ARGS (GST_BUFFER_PTS (outbuf)),
				GST_TIME_ARGS (GST_BUFFER_DTS (outbuf)),
				GST_TIME_ARGS (GST_BUFFER_DURATION (outbuf)));
		fret = gst_pad_push (GST_PAD_CAST (srcpad), outbuf);
		if (G_UNLIKELY (fret != GST_FLOW_OK)) {
			GST_ERROR_OBJECT (self, "failed with reason : %s", gst_flow_get_name (fret));
			goto error2;
		}
	}
	if (self->priv->input) {
		free (self->priv->input);
	}
	for (chan_id = 0; chan_id < self->num_request_pads; chan_id++) {
		if (self->priv->output[chan_id])
			free (self->priv->output[chan_id]);
	}

	gst_buffer_unref (inbuf);
	return fret;

error:
	pthread_mutex_unlock(&count_mutex);
error2:
	gst_buffer_unref (inbuf);
	return fret;
}

/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
	static gboolean
ivas_xm2m_init (GstPlugin * m2m)
{
	return gst_element_register (m2m, "ivas_xm2m", GST_RANK_NONE,
			GST_TYPE_IVAS_XM2M);
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
		GST_VERSION_MINOR,
		ivas_xm2m,
		"Xilinx generic M2M plugin",
		ivas_xm2m_init,
		"0.1",
		"LGPL",
		"GStreamer xrt",
		"http://xilinx.com/")

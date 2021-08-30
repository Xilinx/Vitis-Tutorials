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

#include <stdio.h>
#include <stdint.h>
#include "ivas_multi_scaler_hw.h"
#include <ivas/ivas_kernel.h>
#include <gst/ivas/gstivasmeta.h>

#define MULTI_SCALER_TIMEOUT 1000 // 1 sec
#define MAX_CHANNELS 40
//#define PROFILING 1
#define FRAME_SIZE(w,h) ((w)*(h)*3) // frame size for RGB

typedef struct _kern_priv {
	IVASFrame *msPtr[MAX_CHANNELS];
} ivasMSKernelPriv;

struct _roi {
	uint32_t y_cord;
	uint32_t x_cord;
	uint32_t height;
	uint32_t width;
};

typedef struct _ivas_ms_roi {
	uint32_t nobj;
	struct _roi roi[MAX_CHANNELS];
} ivas_ms_roi;


static ivas_ms_roi ivas_ms_parse_roi (GstIvasMeta *ivas_meta);
int32_t xlnx_kernel_start (IVASKernel *handle, int start, IVASFrame *input[MAX_NUM_OBJECT], IVASFrame *output[MAX_NUM_OBJECT]);
int32_t xlnx_kernel_done(IVASKernel *handle);
int32_t xlnx_kernel_init (IVASKernel * handle);
uint32_t xlnx_kernel_deinit (IVASKernel *handle);

static uint32_t xlnx_multiscaler_align(uint32_t stride_in, uint16_t AXIMMDataWidth) {
	uint32_t stride;
	uint16_t MMWidthBytes = AXIMMDataWidth / 8;

	stride = ((( stride_in ) + MMWidthBytes - 1) / MMWidthBytes) * MMWidthBytes;
	return stride;
}

static void xlnx_multiscaler_descriptor_create (IVASKernel *handle, IVASFrame *input[MAX_NUM_OBJECT], IVASFrame *output[MAX_NUM_OBJECT], ivas_ms_roi roi_data, GstIvasMeta *ivas_meta)
{
	MULTISCALER_DESC_STRUCT *msPtr;
	guint chan_id;
	ivasMSKernelPriv *kpriv = (ivasMSKernelPriv *)handle->kernel_priv;
	IVASFrame *in_ivas_frame, *out_ivas_frame;
	IVASFrameProps out_props = {0, };
	IvasObjectMetadata *xva_obj = NULL;
	uint32_t size, nobjects;
	uint32_t y_cord, x_cord;
	uint32_t stride;

	in_ivas_frame = input[0];
	stride = xlnx_multiscaler_align(in_ivas_frame->props.stride, MS_ALIGN);

	nobjects = 0;
	for (chan_id = 0; chan_id < roi_data.nobj; chan_id++) {
		out_props.width = xlnx_multiscaler_align(roi_data.roi[chan_id].width, MS_ALIGN);
		out_props.height = roi_data.roi[chan_id].height;
		y_cord = ((roi_data.roi[chan_id].y_cord + 1 )/2);
		y_cord = y_cord * 2;		/* fixing odd y cordinate for chroma*/
		x_cord = xlnx_multiscaler_align(roi_data.roi[chan_id].x_cord, MS_ALIGN);
		size = FRAME_SIZE (out_props.width, out_props.height);
		out_props.fmt = IVAS_VFMT_BGR8;

		out_ivas_frame = ivas_alloc_buffer (handle, size, IVAS_FRAME_MEMORY, &out_props);
		if (!out_ivas_frame) {
			printf("ERROR: IVAS MS: failed to allocate frame memory");
			return;
		}
		xva_obj = (IvasObjectMetadata *) g_list_nth_data (ivas_meta->xmeta.objects, nobjects++);
		if (xva_obj)
        		xva_obj->obj_list = g_list_append (xva_obj->obj_list, out_ivas_frame->app_priv);
		else {
			printf("ERROR: IVAS MS: Unable to attach metadata for resized image");
			return;
		}
		msPtr = kpriv->msPtr[chan_id]->vaddr[0];
		msPtr->msc_srcImgBuf0 = (uint64_t) in_ivas_frame->paddr[0] + ((stride*y_cord) + x_cord);
		msPtr->msc_srcImgBuf1 = (uint64_t) in_ivas_frame->paddr[1] + xlnx_multiscaler_align(((stride*y_cord/2) + x_cord), MS_ALIGN); /* plane 2 */
		msPtr->msc_srcImgBuf2 = (uint64_t) 0;

		msPtr->msc_dstImgBuf0 = (uint64_t) out_ivas_frame->paddr[0];
		msPtr->msc_dstImgBuf1 = (uint64_t) out_ivas_frame->paddr[1]; /* plane 2*/
		msPtr->msc_dstImgBuf2 = (uint64_t) 0;

		msPtr->msc_widthIn = out_ivas_frame->props.width;
		msPtr->msc_heightIn = out_ivas_frame->props.height;
		msPtr->msc_inPixelFmt = XV_MULTI_SCALER_Y_UV8_420;
		msPtr->msc_strideIn = stride;
		msPtr->msc_widthOut = out_ivas_frame->props.width;
		msPtr->msc_heightOut = out_ivas_frame->props.height;
		msPtr->msc_lineRate = (uint32_t) ((float) ((msPtr->msc_heightIn * STEP_PRECISION) + ((msPtr->msc_heightOut) / 2)) / (float) msPtr->msc_heightOut);
		msPtr->msc_pixelRate = (uint32_t) ((float) (((msPtr->msc_widthIn) * STEP_PRECISION) + ((msPtr->msc_widthOut) / 2)) / (float) msPtr->msc_widthOut);
		msPtr->msc_outPixelFmt = XV_MULTI_SCALER_BGR8;

		msPtr->msc_strideOut = xlnx_multiscaler_align((out_ivas_frame->props.width * 3), MS_ALIGN);

		msPtr->msc_blkmm_hfltCoeff = 0;
		msPtr->msc_blkmm_vfltCoeff = 0;
		msPtr->msc_nxtaddr = kpriv->msPtr[chan_id + 1]->paddr[0];
        	out_ivas_frame->app_priv = NULL;
        	ivas_free_buffer (handle, out_ivas_frame);
		out_ivas_frame = NULL;
	}
	nobjects = 0;
	for (; chan_id < 2*roi_data.nobj; chan_id++) {
		out_props.width = 80;
		out_props.height = 80;
		size = FRAME_SIZE (out_props.width, out_props.height);
		y_cord = ((roi_data.roi[chan_id - roi_data.nobj].y_cord + 1 )/2);
		y_cord = y_cord * 2;		/* fixing odd y cordinate for chroma*/
		x_cord = xlnx_multiscaler_align(roi_data.roi[chan_id - roi_data.nobj].x_cord, MS_ALIGN);
		out_props.fmt = IVAS_VFMT_BGR8;

		out_ivas_frame = ivas_alloc_buffer (handle, size, IVAS_FRAME_MEMORY, &out_props);
		if (!out_ivas_frame) {
			printf("ERROR: IVAS MS: failed to allocate frame memory");
			return;
		}
		xva_obj = (IvasObjectMetadata *) g_list_nth_data (ivas_meta->xmeta.objects, nobjects++);
		if (xva_obj)
        		xva_obj->obj_list = g_list_append (xva_obj->obj_list, out_ivas_frame->app_priv);
		else {
			printf("ERROR: IVAS MS: Unable to attach metadata for resized image");
			return;
		}
		msPtr = kpriv->msPtr[chan_id]->vaddr[0];
		msPtr->msc_srcImgBuf0 = (uint64_t) in_ivas_frame->paddr[0] + ((stride*y_cord) + x_cord);
		msPtr->msc_srcImgBuf1 = (uint64_t) in_ivas_frame->paddr[1] + xlnx_multiscaler_align(((stride*y_cord/2) + x_cord), MS_ALIGN); /* plane 2 */
		msPtr->msc_srcImgBuf2 = (uint64_t) 0;

		msPtr->msc_dstImgBuf0 = (uint64_t) out_ivas_frame->paddr[0];
		msPtr->msc_dstImgBuf1 = (uint64_t) 0; /* plane 2*/
		msPtr->msc_dstImgBuf2 = (uint64_t) 0;

		msPtr->msc_widthIn = xlnx_multiscaler_align(roi_data.roi[chan_id - roi_data.nobj].width, MS_ALIGN);
		msPtr->msc_heightIn = roi_data.roi[chan_id - roi_data.nobj].height;
		msPtr->msc_inPixelFmt = XV_MULTI_SCALER_Y_UV8_420;
		msPtr->msc_strideIn = stride;
		msPtr->msc_widthOut = out_ivas_frame->props.width;
		msPtr->msc_heightOut = out_ivas_frame->props.height;
		msPtr->msc_lineRate = (uint32_t) ((float) ((msPtr->msc_heightIn * STEP_PRECISION) + ((msPtr->msc_heightOut) / 2)) / (float) msPtr->msc_heightOut);
		msPtr->msc_pixelRate = (uint32_t) ((float) (((msPtr->msc_widthIn) * STEP_PRECISION) + ((msPtr->msc_widthOut) / 2)) / (float) msPtr->msc_widthOut);
		msPtr->msc_outPixelFmt = XV_MULTI_SCALER_BGR8;

		msPtr->msc_strideOut = xlnx_multiscaler_align((out_ivas_frame->props.width * 3), MS_ALIGN);

		msPtr->msc_blkmm_hfltCoeff = 0;
		msPtr->msc_blkmm_vfltCoeff = 0;
		if (chan_id == (2*roi_data.nobj - 1))
			msPtr->msc_nxtaddr = 0;
		else
			msPtr->msc_nxtaddr = kpriv->msPtr[chan_id + 1]->paddr[0];
        	out_ivas_frame->app_priv = NULL;
        	ivas_free_buffer (handle, out_ivas_frame); 
		out_ivas_frame = NULL;
	}
}

static ivas_ms_roi ivas_ms_parse_roi (GstIvasMeta *ivas_meta) {
	IvasObjectMetadata *xva_obj = NULL;
	ivas_ms_roi roi_data;
	int nobjects = 0, i;

	roi_data.nobj = ivas_meta ? g_list_length(ivas_meta->xmeta.objects) : 0; /* Number of outputs */

	for (i = 0 ; i < roi_data.nobj; i++) {
		if (ivas_meta)
			xva_obj = (IvasObjectMetadata *) g_list_nth_data (ivas_meta->xmeta.objects, nobjects++);

		if (xva_obj) {
			roi_data.roi[i].y_cord = (uint32_t)xva_obj->bbox_meta.ymin;
			roi_data.roi[i].x_cord = (uint32_t)xva_obj->bbox_meta.xmin;
			roi_data.roi[i].height = (uint32_t)(xva_obj->bbox_meta.ymax - xva_obj->bbox_meta.ymin);
			roi_data.roi[i].width = (uint32_t)(xva_obj->bbox_meta.xmax - xva_obj->bbox_meta.xmin);
		}
	}
	return roi_data;
}

int32_t xlnx_kernel_start (IVASKernel *handle, int start /*unused */,
		IVASFrame *input[MAX_NUM_OBJECT], IVASFrame *output[MAX_NUM_OBJECT])
{
	int ret;
	uint32_t value = 0;
	IVASFrame *in_ivas_frame;
	GstIvasMeta *ivas_meta = NULL;
	ivasMSKernelPriv *kpriv = (ivasMSKernelPriv *)handle->kernel_priv;
	ivas_ms_roi roi_data;

	in_ivas_frame = input[0];

	/* get metadata from input */
	ivas_meta = gst_buffer_get_ivas_meta ((GstBuffer*)in_ivas_frame->app_priv);
	if (ivas_meta == NULL) {
		goto done;
	} else if (g_list_length(ivas_meta->xmeta.objects) > MAX_CHANNELS/2) {
		printf("can't process more than %d objects\n", MAX_CHANNELS/2);
		return FALSE;
	}
	roi_data = ivas_ms_parse_roi (ivas_meta);
	if(roi_data.nobj == 0)
		goto done;
	/* set descriptor */
	xlnx_multiscaler_descriptor_create (handle, input, output, roi_data, ivas_meta);

	/* prgram registers */
	value = 2*roi_data.nobj; /* Number of outputs */
	ivas_register_write (handle, &value, sizeof (uint32_t), XV_MULTI_SCALER_CTRL_ADDR_HWREG_NUM_OUTS_DATA);
	value = kpriv->msPtr[0]->paddr[0];
	ivas_register_write (handle, &value, sizeof (uint32_t), XV_MULTI_SCALER_CTRL_ADDR_HWREG_START_ADDR_DATA);
	/* submit kernel command */
	ret = ivas_kernel_start (handle);
	if (ret < 0) {
		printf("ERROR: IVAS MS: failed to issue execute command");
		return FALSE;
	}

#if PROFILING
	uint64_t delta_us;
	struct timespec tstart, end;
	clock_gettime(CLOCK_MONOTONIC_RAW, &tstart);
#endif
	/* wait for kernel completion */
	ret = ivas_kernel_done (handle, MULTI_SCALER_TIMEOUT);
	if (ret < 0) {
		printf("ERROR: IVAS MS: failed to receive response from kernel");
		return FALSE;
	}
#if PROFILING
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	delta_us = (end.tv_sec - tstart.tv_sec) * 1000000 + (end.tv_nsec - tstart.tv_nsec) / 1000;
	printf ("MS %lu\n", delta_us);
#endif
done:
	return TRUE;
}

int32_t xlnx_kernel_init (IVASKernel *handle)
{
	uint8_t chan_id;

	ivasMSKernelPriv *kpriv = (ivasMSKernelPriv*) calloc (1, sizeof (ivasMSKernelPriv));
	if (!kpriv) {
		printf("ERROR: IVAS MS: failed to allocate kernelprivate memory");
		return -1;
	}

	for (chan_id = 0; chan_id < MAX_CHANNELS; chan_id++) {
		kpriv->msPtr[chan_id] = ivas_alloc_buffer (handle, DESC_SIZE, IVAS_INTERNAL_MEMORY, NULL);
		if (!kpriv->msPtr[chan_id]) {
			printf("ERROR: IVAS MS: failed to allocate descriptor memory");
			return -1;
		}
	}

	handle->kernel_priv = (void*)kpriv;
	handle->is_multiprocess = 1;

	return 0;
}

uint32_t xlnx_kernel_deinit (IVASKernel *handle)
{
	uint8_t chan_id;
	ivasMSKernelPriv *kpriv = (ivasMSKernelPriv *)handle->kernel_priv;

	for (chan_id = 0; chan_id < MAX_CHANNELS; chan_id++)
		ivas_free_buffer (handle, kpriv->msPtr[chan_id]);

	free (kpriv);
	handle->kernel_priv = NULL;

	return 0;
}

int32_t xlnx_kernel_done(IVASKernel *handle)
{
	/* dummy */
	return 0;
}

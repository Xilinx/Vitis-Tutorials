/*
* Copyright (C) 2020 – 2021 Xilinx, Inc.  All rights reserved.
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
#ifndef __IVAS_KERNEL_H__
#define __IVAS_KERNEL_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <stdint.h>
#include <jansson.h>

/* Update of this file by the user is not encouraged */
#define MAX_NUM_OBJECT 512
#define MAX_EXEC_WAIT_RETRY_CNT 10
#define VIDEO_MAX_PLANES 4

typedef enum  {
  IVAS_UNKNOWN_MEMORY,
  IVAS_FRAME_MEMORY,
  IVAS_INTERNAL_MEMORY,
} IVASMemoryType;

#ifdef XLNX_PCIe_PLATFORM
typedef enum {
  IVAS_SYNC_DATA_TO_DEVICE,
  IVAS_SYNC_DATA_FROM_DEVICE
} IVASSyncDataFlag;
#endif

typedef enum {
  IVAS_VMFT_UNKNOWN = 0,
  IVAS_VFMT_RGBX8,
  IVAS_VFMT_YUVX8,
  IVAS_VFMT_YUYV8,
  IVAS_VFMT_ABGR8,
  IVAS_VFMT_RGBX10,
  IVAS_VFMT_YUVX10,
  IVAS_VFMT_Y_UV8,
  IVAS_VFMT_Y_UV8_420,
  IVAS_VFMT_RGB8,
  IVAS_VFMT_YUVA8,
  IVAS_VFMT_YUV8,
  IVAS_VFMT_Y_UV10,
  IVAS_VFMT_Y_UV10_420,
  IVAS_VFMT_Y8,
  IVAS_VFMT_Y10,
  IVAS_VFMT_ARGB8,
  IVAS_VFMT_BGRX8,
  IVAS_VFMT_UYVY8,
  IVAS_VFMT_BGR8,
  IVAS_VFMT_RGBX12,
  IVAS_VFMT_RGB16
} IVASVideoFormat;

typedef struct _ivas_kernel IVASKernel;
typedef struct _ivas_frame IVASFrame;
typedef struct _ivas_frame_props IVASFrameProps;

typedef int32_t (*IVASBufAllocCBFunc) (IVASKernel *handle, IVASFrame *ivas_frame, void* user_data);
typedef void (*IVASBufFreeCBFunc) (IVASKernel *handle, IVASFrame *ivas_frame, void* user_data);


typedef int32_t (*IVASKernelInit) (IVASKernel *handle);
typedef int32_t (*IVASKernelDeInit) (IVASKernel *handle);
typedef int32_t (*IVASKernelStartFunc) (IVASKernel *handle, int32_t start, IVASFrame *input[MAX_NUM_OBJECT], IVASFrame *output[MAX_NUM_OBJECT]);
typedef int32_t (*IVASKernelDoneFunc) (IVASKernel *handle);

struct _ivas_frame_props {
  uint32_t width;
  uint32_t height;
  uint32_t stride;
  IVASVideoFormat fmt;
};

struct _ivas_frame {
  uint32_t bo[VIDEO_MAX_PLANES];
  void *vaddr[VIDEO_MAX_PLANES];
  uint64_t paddr[VIDEO_MAX_PLANES];
  uint32_t size[VIDEO_MAX_PLANES];
  void *meta_data;
  IVASFrameProps props;

  void *app_priv;
  IVASMemoryType mem_type;

  uint32_t n_planes;
};

typedef enum
{
  IVAS_PAD_RIGID,
  IVAS_PAD_FLEXIBLE,
  IVAS_PAD_DEFAULT
} padsnature;

typedef struct caps
{
  uint8_t range_height;
  uint32_t lower_height;
  uint32_t upper_height;

  uint8_t range_width;
  uint32_t lower_width;
  uint32_t upper_width;

  uint8_t num_fmt;
  IVASVideoFormat *fmt;
} kernelcaps;

typedef struct kernelpad
{
  uint8_t nu_caps;
  kernelcaps **kcaps;
} kernelpads;

typedef struct ivaspads
{
  padsnature nature;
  uint8_t nu_sinkpad;
  uint8_t nu_srcpad;
  kernelpads **sinkpads;
  kernelpads **srcpads;
} ivaspads;

typedef struct buffer {
  unsigned int bo;
  void* user_ptr;
  uint64_t phy_addr;
  unsigned int size;
} xrt_buffer;

struct _ivas_kernel {
  void *xcl_handle;
  uint32_t cu_idx;
  json_t *kernel_config;
  json_t *kernel_dyn_config;
  void *kernel_priv;
  xrt_buffer *ert_cmd_buf;
  size_t min_offset;
  size_t max_offset;
  IVASBufAllocCBFunc alloc_func;
  IVASBufFreeCBFunc free_func;
  void *cb_user_data;
  ivaspads *padinfo;
#ifdef XLNX_PCIe_PLATFORM
  uint32_t is_softkernel;
#endif
  uint8_t is_multiprocess;
};


IVASFrame* ivas_alloc_buffer (IVASKernel *handle, uint32_t size, IVASMemoryType mem_type, IVASFrameProps *props);
void ivas_free_buffer (IVASKernel *handle, IVASFrame *ivas_frame);
void ivas_register_write (IVASKernel *handle, void *src, size_t size, size_t offset);
void ivas_register_read (IVASKernel *handle, void *src, size_t size, size_t offset);
int32_t ivas_kernel_start (IVASKernel *handle);
int32_t ivas_kernel_done (IVASKernel *handle, int32_t timeout);

#ifdef XLNX_PCIe_PLATFORM

int32_t ivas_sync_data (IVASKernel *handle, IVASSyncDataFlag flag, IVASFrame *frame);
#endif
#endif

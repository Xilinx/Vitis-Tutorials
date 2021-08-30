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

/* Update of this file by the user is not encouraged */
#include <ivas/ivas_kernel.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#ifdef XLNX_PCIe_PLATFORM
#include <xrt.h>
#include <ert.h>
#include <xclhal2.h>
#else
#include <xrt/xrt.h>
#include <xrt/ert.h>
#include <xrt/xclhal2.h>
#endif
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

#undef DUMP_REG                 // dump reg_map just before sending ert cmd

enum
{
  LOG_LEVEL_ERROR,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_INFO,
  LOG_LEVEL_DEBUG
};

static int log_level = LOG_LEVEL_WARNING;

#define LOG_MESSAGE(level, ...) {\
  do {\
    char *str; \
    if (level == LOG_LEVEL_ERROR)\
      str = (char*)"ERROR";\
    else if (level == LOG_LEVEL_WARNING)\
      str = (char*)"WARNING";\
    else if (level == LOG_LEVEL_INFO)\
      str = (char*)"INFO";\
    else if (level == LOG_LEVEL_DEBUG)\
      str = (char*)"DEBUG";\
    if (level <= log_level) {\
      printf("[%s:%d] %s: ", __func__, __LINE__, str);\
      printf(__VA_ARGS__);\
      printf("\n");\
    }\
  } while (0); \
}


static int
alloc_xrt_buffer (xclDeviceHandle handle, IVASFrame * frame,
    enum xclBOKind bo_kind, unsigned flags)
{
  frame->bo[0] = xclAllocBO (handle, frame->size[0], bo_kind, flags);
  if (frame->bo[0] == NULLBO) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, "failed to allocate Device BO...");
    return -1;
  }

  frame->vaddr[0] = xclMapBO (handle, frame->bo[0], true);
  if (frame->vaddr[0] == NULL) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, "failed to map BO...");
    xclFreeBO (handle, frame->bo[0]);
    return -1;
  }

  if (bo_kind != XCL_BO_SHARED_VIRTUAL) {
    struct xclBOProperties p;
    if (xclGetBOProperties (handle, frame->bo[0], &p)) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, "failed to get physical address...");
      munmap (frame->vaddr[0], frame->size[0]);
      xclFreeBO (handle, frame->bo[0]);
      return -1;
    }
    frame->paddr[0] = p.paddr;
  }

  frame->meta_data = NULL;
  frame->app_priv = NULL;
  frame->n_planes = 1;

  LOG_MESSAGE (LOG_LEVEL_DEBUG,
      "allocated xrt buffer : bo = %d, paddr = %p, vaddr = %p and size = %d",
      frame->bo[0], (void *) frame->paddr[0], frame->vaddr[0], frame->size[0]);

  return 0;
}

static void
free_xrt_buffer (xclDeviceHandle handle, IVASFrame * frame)
{
  if (!handle || !frame) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, "invalid arguments : handle %p, frame %p",
        handle, frame);
    return;
  }
  if (frame->vaddr[0] && frame->size[0])
    munmap (frame->vaddr[0], frame->size[0]);
  if (handle && frame->bo[0] > 0)
    xclFreeBO (handle, frame->bo[0]);
  memset (frame, 0x00, sizeof (IVASFrame));
}

IVASFrame *
ivas_alloc_buffer (IVASKernel * handle, uint32_t size, IVASMemoryType mem_type,
    IVASFrameProps * props)
{
  IVASFrame *frame = NULL;
  int iret = -1;

  if (!handle) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, "invalid arguments : handle %p", handle);
    goto error;
  }

  frame = (IVASFrame *) calloc (1, sizeof (IVASFrame));
  if (!frame) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, "failed to allocate ivas_frame");
    goto error;
  }
  frame->mem_type = mem_type;

  if (mem_type == IVAS_INTERNAL_MEMORY) {
    frame->size[0] = size;

    iret = alloc_xrt_buffer (handle->xcl_handle, frame, XCL_BO_DEVICE_RAM, 0);
    if (iret < 0) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, "failed to allocate internal memory");
      goto error;
    }
  } else {
    if (!props || !props->width || !props->height
        || (props->fmt == IVAS_VMFT_UNKNOWN)) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, "invalid arguments for properties");
      goto error;
    }

    memcpy (&(frame->props), props, sizeof (IVASFrameProps));
    if (!handle->alloc_func) {
      LOG_MESSAGE (LOG_LEVEL_ERROR,
          "app did not set alloc_func callback function");
      goto error;
    }

    iret = handle->alloc_func (handle, frame, handle->cb_user_data);
    if (iret < 0) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, "failed to allocate frame memory");
      goto error;
    }
  }
  return frame;

error:
  if (frame) {
    ivas_free_buffer (handle, frame);
    free (frame);
  }
  return NULL;
}

void
ivas_free_buffer (IVASKernel * handle, IVASFrame * ivas_frame)
{
  if (ivas_frame->mem_type == IVAS_INTERNAL_MEMORY) {
    free_xrt_buffer (handle, ivas_frame);
  } else {
    if (!handle->free_func) {
      LOG_MESSAGE (LOG_LEVEL_ERROR,
          "app did not set free_func callback function");
    } else {
      handle->free_func (handle, ivas_frame, handle->cb_user_data);
    }
  }
  free (ivas_frame);
}

void
ivas_register_write (IVASKernel * handle, void *src, size_t size, size_t offset)
{
  if (handle->is_multiprocess) {
    struct ert_start_kernel_cmd *ert_cmd =
        (struct ert_start_kernel_cmd *) (handle->ert_cmd_buf->user_ptr);
    uint32_t *src_array = (uint32_t *) src;
    size_t cur_min = offset;
    size_t cur_max = offset + size;
    int32_t entries = size / sizeof (uint32_t);
    int32_t start = offset / sizeof (uint32_t);
    int32_t i;

    for (i = 0; i < entries; i++)
      ert_cmd->data[start + i] = src_array[i];

    if (cur_max > handle->max_offset)
      handle->max_offset = cur_max;

    if (cur_min < handle->min_offset)
      handle->min_offset = cur_min;
  } else {
      uint32_t *value = (uint32_t*)src;
      xclRegWrite((xclDeviceHandle )handle->xcl_handle, handle->cu_idx, offset, value[0]);
  }
  return;
}

void ivas_register_read (IVASKernel *handle, void *src, size_t size, size_t offset)
{
  uint32_t *value = (uint32_t*)src;
  xclRegRead((xclDeviceHandle )handle->xcl_handle, handle->cu_idx, offset, &(value[0]));
}

int32_t
ivas_kernel_start (IVASKernel * handle)
{
  struct ert_start_kernel_cmd *ert_cmd =
      (struct ert_start_kernel_cmd *) (handle->ert_cmd_buf->user_ptr);

  ert_cmd->state = ERT_CMD_STATE_NEW;

#ifdef XLNX_PCIe_PLATFORM
  ert_cmd->opcode = handle->is_softkernel ? ERT_SK_START: ERT_START_CU;
#else
  ert_cmd->opcode = ERT_START_CU;
#endif
  ert_cmd->cu_mask = 1 << handle->cu_idx;
  ert_cmd->count = (handle->max_offset >> 2) + 1;

#ifdef DUMP_REG
  {
    int i;
    for (i = 0; i < ert_cmd->count; i++)
      printf ("index 0x%x - 0x%x\n", i * sizeof (uint32_t), ert_cmd->data[i]);
  }
#endif

  if (xclExecBuf (handle->xcl_handle, handle->ert_cmd_buf->bo)) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, "failed to issue XRT command");
    return -1;
  }
  LOG_MESSAGE (LOG_LEVEL_DEBUG, "Submitted command to kernel");

  return 0;
}

int32_t
ivas_kernel_done (IVASKernel * handle, int32_t timeout)
{
  struct ert_start_kernel_cmd *ert_cmd =
      (struct ert_start_kernel_cmd *) (handle->ert_cmd_buf->user_ptr);
  int ret;
  int retry_count = MAX_EXEC_WAIT_RETRY_CNT;

  LOG_MESSAGE (LOG_LEVEL_DEBUG, "Going to wait for kernel command to finish");

  do {
    ret = xclExecWait (handle->xcl_handle, timeout);
    if (ret < 0) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, "ExecWait ret = %d. reason : %s", ret,
          strerror (errno));
      return -1;
    } else if (!ret) {
      LOG_MESSAGE (LOG_LEVEL_WARNING, "timeout...retry execwait");
      if (retry_count-- <= 0) {
        LOG_MESSAGE (LOG_LEVEL_ERROR,
            "max retry count %d reached..returning error",
            MAX_EXEC_WAIT_RETRY_CNT);
        return -1;
      }
    }
  } while (ert_cmd->state != ERT_CMD_STATE_COMPLETED);

  LOG_MESSAGE (LOG_LEVEL_DEBUG, "successfully completed kernel command");

  return 0;
}

#ifdef XLNX_PCIe_PLATFORM
int32_t
ivas_sync_data (IVASKernel *handle, IVASSyncDataFlag flag, IVASFrame *frame)
{
  int iret, plane_id;
  enum xclBOSyncDirection sync_flag = (flag == IVAS_SYNC_DATA_TO_DEVICE) ? XCL_BO_SYNC_BO_TO_DEVICE:IVAS_SYNC_DATA_FROM_DEVICE;

  for (plane_id = 0; plane_id < frame->n_planes; plane_id++) {
    LOG_MESSAGE(LOG_LEVEL_DEBUG, "plane %d syncing %s : bo = %d, size = %d",
        plane_id, sync_flag == XCL_BO_SYNC_BO_TO_DEVICE ? "to device" : "from device",
        frame->bo[plane_id], frame->size[plane_id]);

    iret = xclSyncBO(handle->xcl_handle, frame->bo[plane_id], sync_flag, frame->size[plane_id], 0);
    if (iret != 0) {
      LOG_MESSAGE(LOG_LEVEL_ERROR, "xclSyncBO failed %d, reason : %s", iret, strerror(errno));
      return iret;
    }
  }
  return 0;
}
#endif

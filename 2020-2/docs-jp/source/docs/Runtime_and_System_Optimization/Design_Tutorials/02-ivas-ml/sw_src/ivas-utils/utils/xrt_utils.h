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

#ifndef __XRT_UTILS_H__
#define __XRT_UTILS_H__

/* Update of this file by the user is not encouraged */
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>


#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ivas/ivas_kernel.h>


//#include "xclhal2.h"
#include <uuid/uuid.h>
#ifdef XLNX_PCIe_PLATFORM
#include <xrt.h>
#include <ert.h>
#else
#include <xrt/xrt.h>
#include <xrt/ert.h>
#endif
#define __COUNT                 (1024)
#define DATA_SIZE               (__COUNT * sizeof(int))

int alloc_xrt_buffer (xclDeviceHandle handle, unsigned int size, enum xclBOKind bo_kind, unsigned flags, xrt_buffer *buffer);
void free_xrt_buffer (xclDeviceHandle handle, xrt_buffer *buffer);
int download_xclbin ( const char *bit, unsigned deviceIndex, const char* halLog, xclDeviceHandle *handle, uuid_t *xclbinId);
int download_softkernel (const char *filename, const char *name, xclDeviceHandle handle, const int start_id, const int cu_count);
int configure_soft_kernel(xclDeviceHandle handle, const size_t sk_size, const char *sk_name,
    const uint64_t sk_paddr, const int start_idx, const int cu_count);
int is_softkernel_downloaded(const void *dev_handle, const uint32_t dev_index, const char *lock_file);
int send_softkernel_command (xclDeviceHandle handle, xrt_buffer *sk_buf, unsigned int *payload, unsigned int num_idx, unsigned int cu_mask, int timeout);
#endif

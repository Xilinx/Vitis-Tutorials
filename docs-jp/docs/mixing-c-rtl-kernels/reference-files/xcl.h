/*******************************************************************************
Vendor: Xilinx
Associated Filename: xcl.h
Purpose: SDAccel histogram equalization example
Revision History: December 6, 2015

*******************************************************************************
Copyright (C) 2015 XILINX, Inc.

This file contains confidential and proprietary information of Xilinx, Inc. and
is protected under U.S. and international copyright and other intellectual
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials
distributed herewith. Except as otherwise provided in a valid license issued to
you by Xilinx, and to the maximum extent permitted by applicable law:
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY,
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether
in contract or tort, including negligence, or under any other theory of
liability) for any loss or damage of any kind or nature related to, arising under
or in connection with these materials, including for any direct, or any indirect,
special, incidental, or consequential loss or damage (including loss of data,
profits, goodwill, or any type of loss or damage suffered as a result of any
action brought by a third party) even if such damage or loss was reasonably
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any
application requiring fail-safe performance, such as life-support or safety
devices or systems, Class III medical devices, nuclear facilities, applications
related to the deployment of airbags, or any other applications that could lead
to death, personal injury, or severe property or environmental damage
(individually and collectively, "Critical Applications"). Customer assumes the
sole risk and liability of any use of Xilinx products in Critical Applications,
subject only to applicable laws and regulations governing limitations on product
liability.

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT
ALL TIMES.

*******************************************************************************/


#pragma once

#include "xcl.h"
#include <CL/opencl.h>

typedef struct {
	cl_platform_id platform_id;
	cl_device_id device_id;
	cl_context context;
	cl_command_queue command_queue;
} xcl_world;

/* xcl_world_single
 *
 * Description:
 *   Setup an xcl_world for the case when there is a single
 *   device in the system.
 *
 * Inputs:
 *   device_type - the type of device (i.e. CL_DEVICE_TYPE_ACCELERATOR)
 *
 * Returns:
 *   A struct containing the platform_id, device_id, context, and command
 *   queue.
 */
xcl_world xcl_world_single(cl_device_type device_type, 
                           const char *target_vendor, 
                           const char *target_device);

/* xcl_release_world
 *
 * Description:
 *   Release memory used by xcl_world struct.
 *
 * Inputs:
 *   world - xcl_world to release memory from.
 */
void xcl_release_world(xcl_world world);

/* xcl_import_binary
 *
 * Description:
 *   Import precompiled program (as commonly created by the Xilinx OpenCL
 *   flow).
 *
 * Inputs:
 *   world - xcl_world to import into.
 *   krnl_file - file name of the kernel to import.
 *   krnl_name - name of kernel.
 *
 * Returns:
 *   An opencl kernel object that was created from krnl_name file.
 */
cl_kernel xcl_import_binary(xcl_world world, const char *krnl_file, const char *krnl_name);

/* xcl_import_source
 *
 * Description:
 *   Import opencl source code.
 *
 * Inputs:
 *   world - xcl_world to import into.
 *   krnl_file - file name of the kernel to import.
 *   krnl_name - name of kernel.
 *
 * Returns:
 *   An opencl kernel object that was created from krnl_name file.
 */
cl_kernel xcl_import_source(xcl_world world, const char *krnl_file, const char *krnl_name);

/* xcl_set_kernel_arg
 *
 * Description:
 *   Set kernel arguments
 *
 * Inputs:
 *   krnl - kernel to set values for
 *   num  - which kernel arg to set
 *   size - size of argument
 *   ptr  - address of value
 */
void xcl_set_kernel_arg(cl_kernel krnl, cl_uint num, size_t size, const void *ptr);

/* xcl_malloc
 *
 * Description:
 *   Allocate memory for a buffer on the FPGA device.  Exit program on
 *   error.
 *
 * Inputs:
 *   world - xcl_world of the device to create buffer on.
 *   flags - passed to clCreateBuffer.
 *   size  - buffer size in bytes (like malloc).
 */
cl_mem xcl_malloc(xcl_world world, cl_mem_flags flags, size_t size);

/* xcl_memcpy_to_device/xcl_memcpy_from_device
 *
 * Description:
 *   Copy memory from the host to the FPGA device (or vice a verse.)  The
 *   memory on the FPGA must be allocated with xcl_malloc (or the lower
 *   level opencl functions)
 *
 * Inputs:
 *   world - xcl_world to copy the buffer into.
 *   dest  - memory address on the FPGA to copy to.
 *   src   - memory address on the host to copy from.
 *   size  - number of bytes in src to copy to dest.
 */
void xcl_memcpy_to_device(xcl_world world, cl_mem dest, void* src,
                          size_t size);
void xcl_memcpy_from_device(xcl_world world, void* dest, cl_mem src,
                            size_t size);

/* xcl_run_kernel3d
 *
 * Description:
 *   Run a kernel with a 3 dimensional thread array. In this configuration,
 *   there will be x*y*z threads created with a rank in each dimension.
 *
 * Inputs:
 *   world - xcl_world to use for running the kernel.
 *   krnl  - application to run on the device.
 *   x     - number of threads in the x direction.
 *   y     - number of threads in the y direction.
 *   z     - number of threads in the z direction.
 *
 * Returns:
 *   For purposes of benchmarking, the return of this program is the length of
 *   time that the kernel took to run to completion.
 */
unsigned long xcl_run_kernel3d(xcl_world world, cl_kernel krnl,
                               size_t x, size_t y, size_t z);

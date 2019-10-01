/*******************************************************************************
Vendor: Xilinx
Associated Filename: xcl.c
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <math.h>

#include "xcl.h"

static void* smalloc(size_t size) {
	void* ptr;

	ptr = malloc(size);

	if (ptr == NULL) {
		printf("Error: Cannot allocate memory\n");
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

	return ptr;
}

static int load_file_to_memory(const char *filename, char **result) {
	unsigned int size;

	FILE *f = fopen(filename, "rb");
	if (f == NULL) {
		*result = NULL;
		printf("Error: Could not read file %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	*result = (char *) smalloc(sizeof(char)*(size+1));

	if (size != fread(*result, sizeof(char), size, f)) {
		free(*result);
		printf("Error: read of kernel failed\n");
		exit(EXIT_FAILURE);
	}

	fclose(f);
	(*result)[size] = 0;

	return size;
}

xcl_world xcl_world_single(cl_device_type device_type, const char *target_vendor, 
                           const char *target_device) {
	int err;
	xcl_world world;
	cl_uint num_platforms;

	err = clGetPlatformIDs(0, NULL, &num_platforms);
	if (err != CL_SUCCESS) {
		printf("Error: no platforms available or OpenCL install broken");
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

	cl_platform_id *platform_ids = (cl_platform_id *) malloc(sizeof(cl_platform_id) * num_platforms);

	if (platform_ids == NULL) {
		printf("Error: Out of Memory\n");
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

	err = clGetPlatformIDs(num_platforms, platform_ids, NULL);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to find an OpenCL platform!\n");
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

	int i;
        char cl_platform_vendor[1001];
        //find target vendor if target_vendor is specified
        if (target_vendor != NULL) {
                for(i = 0; i < num_platforms; i++) {
                        err = clGetPlatformInfo(platform_ids[i], CL_PLATFORM_VENDOR, 1000, (void *)cl_platform_vendor,NULL);
                        if (err != CL_SUCCESS) {
                                printf("Error: clGetPlatformInfo(CL_PLATFORM_VENDOR) failed!\n");
                                printf("Test failed\n");
                                exit(EXIT_FAILURE);
                        }
                        if ((target_vendor != NULL) && (strcmp(cl_platform_vendor, target_vendor) == 0)) {
                                printf("INFO: Selected platform %d from %s\n", i, cl_platform_vendor);
                                world.platform_id = platform_ids[i];
                                break;
                        }
                }
        } else {
                for(i = 0; i < num_platforms; i++) {
                        err = clGetDeviceIDs(platform_ids[i], device_type,
                                             1, &world.device_id, NULL);
                        if (err == CL_SUCCESS) {
                                world.platform_id = platform_ids[i];
                                break;
                        }
                }            
        }
	free(platform_ids);
	if (i == num_platforms) {
		printf("Error: Failed to find a platform\n");
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

        if (target_device != NULL) {
                //find target device
                cl_device_id devices[16];  // compute device id 
                cl_uint num_devices;
                char cl_device_name[100];
                err = clGetDeviceIDs(world.platform_id, CL_DEVICE_TYPE_ACCELERATOR,
                                     16, devices, &num_devices);
                if (err != CL_SUCCESS) {
                        printf("Error: Failed to create a device group!\n");
                        printf("Test failed\n");
                        exit(EXIT_FAILURE);
                }

                //iterate all devices to select the target device. 
                for (i=0; i<num_devices; i++) {
                        err = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 100, cl_device_name, 0);
                        if (err != CL_SUCCESS) {
                                printf("Error: Failed to get device name for device %d!\n", i);
                                printf("Test failed\n");
                                exit(EXIT_FAILURE);
                        }
                        //printf("CL_DEVICE_NAME %s\n", cl_device_name);
                        if (strcmp(cl_device_name, target_device) == 0) {
                                world.device_id = devices[i];
                                printf("INFO: Selected %s as the target device\n", cl_device_name);
                                break;
                        }
                }

                if (i == num_devices) {
                        printf("Error: Failed to find target device %s\n", target_device);
                        printf("Test failed\n");
                        exit(EXIT_FAILURE);
                }
        }

	world.context = clCreateContext(0, 1, &world.device_id,
	                                NULL, NULL, &err);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to create a compute context!\n");
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

	world.command_queue = clCreateCommandQueue(world.context,
	                                           world.device_id,
	                                           CL_QUEUE_PROFILING_ENABLE,
	                                           &err);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to create a command queue!\n");
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

	return world;
}

void xcl_release_world(xcl_world world) {
	clReleaseCommandQueue(world.command_queue);
	clReleaseContext(world.context);
}

cl_kernel xcl_import_binary(xcl_world world,
                            const char *krnl_file,
                            const char *krnl_name)
{
	int err;

	char *krnl_bin;
	const size_t krnl_size = load_file_to_memory(krnl_file, &krnl_bin);

	cl_program program = clCreateProgramWithBinary(world.context, 1,
	                                    &world.device_id, &krnl_size,
	                                    (const unsigned char**) &krnl_bin,
	                                    NULL, &err);
	if ((!program) || (err!=CL_SUCCESS)) {
		printf("Error: Failed to create compute program from binary %d!\n",
		       err);
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS) {
		size_t len;
		char buffer[2048];

		printf("Error: Failed to build program executable!\n");
		clGetProgramBuildInfo(program, world.device_id, CL_PROGRAM_BUILD_LOG,
		                      sizeof(buffer), buffer, &len);
		printf("%s\n", buffer);
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

	cl_kernel kernel = clCreateKernel(program, krnl_name, &err);
	if (!kernel || err != CL_SUCCESS) {
		printf("Error: Failed to create kernel for %s: %d\n", krnl_name, err);
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

	/* if program is released, then EnqueueNDRangeKernel fails with
	 * INVALID_KERNEL */
	/* clReleaseProgram(program); */
	free(krnl_bin);

	return kernel;
}

cl_kernel xcl_import_source(xcl_world world,
                            const char *krnl_file,
                            const char *krnl_name)
{
	int err;

	char *krnl_bin;
	load_file_to_memory(krnl_file, &krnl_bin);

	cl_program program = clCreateProgramWithSource(world.context, 1,
	                                               (const char**) &krnl_bin,
	                                               0, &err);
	if ((err!=CL_SUCCESS) || (!program))  {
		printf("Error: Failed to create compute program from binary %d!\n",
		       err);
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS) {
		size_t len;
		char buffer[2048];

		printf("Error: Failed to build program executable!\n");
		clGetProgramBuildInfo(program, world.device_id, CL_PROGRAM_BUILD_LOG,
		                      sizeof(buffer), buffer, &len);
		printf("%s\n", buffer);
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

	cl_kernel kernel = clCreateKernel(program, krnl_name, &err);
	if (!kernel || err != CL_SUCCESS) {
		printf("Error: Failed to create kernel for %s: %d\n", krnl_name, err);
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

	/* if program is released, then EnqueueNDRangeKernel fails with
	 * INVALID_KERNEL */
	/* clReleaseProgram(program); */
	free(krnl_bin);

	return kernel;
}

void xcl_set_kernel_arg(cl_kernel krnl, cl_uint num, size_t size, const void *ptr) {
	int err = clSetKernelArg(krnl, num, size, ptr);

	if(err != CL_SUCCESS) {
		printf("Error: Failed to set kernel arg\n");
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}
}

cl_mem xcl_malloc(xcl_world world, cl_mem_flags flags, size_t size) {
	cl_mem mem = clCreateBuffer(world.context, flags, size, NULL, NULL);

	if (!mem) {
		printf("Error: Failed to allocate device memory!\n");
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

	return mem;
}

void xcl_memcpy_to_device(xcl_world world, cl_mem dest, void* src,
                          size_t size) {
	int err = clEnqueueWriteBuffer(world.command_queue, dest, CL_TRUE, 0, size,
	                               src, 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to write to source array a!\n");
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}
}

void xcl_memcpy_from_device(xcl_world world, void* dest, cl_mem src,
                            size_t size) {
	int err = clEnqueueReadBuffer(world.command_queue, src, CL_TRUE, 0, size,
	                              dest, 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to read output array! %d\n", err);
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}
}

unsigned long xcl_run_kernel3d(xcl_world world, cl_kernel krnl,
                               size_t x, size_t y, size_t z) {
	size_t size[3] = {x, y, z};
	cl_event event;
	unsigned long start, stop;

	int err = clEnqueueNDRangeKernel(world.command_queue, krnl, 3,
	                                 NULL, size, size, 0, NULL, &event);
	if( err != CL_SUCCESS) {
		printf("Error: failed to execute kernel! %d\n", err);
		printf("Test failed\n");
		exit(EXIT_FAILURE);
	}

	clFinish(world.command_queue);

	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START,
	                        sizeof(unsigned long), &start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END,
	                        sizeof(unsigned long), &stop, NULL);

	return stop - start;
}

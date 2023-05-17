/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#ifndef __APIHANDLE_H__
#define __APIHANDLE_H__

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include "CL/opencl.h"


class ApiHandle {

  void* smalloc(size_t size) {
    void* ptr;
    
    ptr = malloc(size);
    
    if (ptr == NULL) {
      printf("Error: Cannot allocate memory\n");
      exit(EXIT_FAILURE);
    }
    
    return ptr;
  }


  int load_file_to_memory(const char *filename, char **result) {
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

  cl_context       m_context;
  cl_program       m_program;
  cl_device_id     m_device_id;
  cl_kernel        m_kernel;
  cl_command_queue m_queue;
  
public:

  cl_command_queue& getQueue()  { return m_queue;  }
  cl_context&       getContext(){ return m_context; }
  cl_kernel&        getKernel() { return m_kernel; }
  
  ApiHandle(char* binaryName, bool oooQueue) {
    // *********** OpenCL Host Code Setup **********

    // Connect to first platform
    int err;
    char cl_platform_vendor[1001];
    char cl_platform_name[1001];
    char cl_device_name[1001];
    
    cl_uint platform_count;
    
    // Get number of platforms
    err = clGetPlatformIDs(0, nullptr, &platform_count);
    if(err !=CL_SUCCESS) {
    	std::cout << "PlatformID error" << std::endl;
    	exit(1);
    }
    
    // get all platforms
    cl_platform_id platform_id;         // platform id
    std::vector<cl_platform_id> platforms(platform_count);
    clGetPlatformIDs(platform_count, platforms.data(), nullptr);
    
    bool found = false;
    for (int p = 0; p < (int)platform_count; ++p) {  
      platform_id = platforms[p];
      clGetPlatformInfo(platform_id,CL_PLATFORM_VENDOR,1000,
			(void *)cl_platform_vendor,
			NULL);
      clGetPlatformInfo(platform_id,CL_PLATFORM_NAME,1000,
			(void *)cl_platform_name,
			NULL);
      if(!strcmp(cl_platform_vendor,"Xilinx")) {
	found = true;
	break;
      }
    }
    if (!found){
      std::cout << "Platform Not Found" << std::endl;
      exit(err);
    }
    
    err = clGetDeviceIDs(platform_id,
			 CL_DEVICE_TYPE_ACCELERATOR,
			 1,
			 &m_device_id,
			 NULL);
    if (err != CL_SUCCESS) {
      std::cout << "FAILED TEST - Device" << std::endl;
      exit(err);
    }
    
    m_context = clCreateContext(0, 1, &m_device_id, NULL, NULL, &err);
    if (err != CL_SUCCESS) {
      std::cout << "FAILED TEST - Context" << std::endl;
      exit(err);
    }
    
    clGetDeviceInfo(m_device_id, CL_DEVICE_NAME, 1000, (void*)cl_device_name, NULL);
    
    std::cout << "DEVICE: " << cl_device_name << std::endl;
    
    std::cout << "Loading Bitstream: " << binaryName << std::endl; 
    char *krnl_bin;
    size_t krnl_size;
    krnl_size = load_file_to_memory(binaryName, &krnl_bin);
    
    std::cout << "INFO: Loaded file" << std::endl;
    
    m_program = clCreateProgramWithBinary(m_context, 1,
					  (const cl_device_id* ) &m_device_id,
					  &krnl_size,
					  (const unsigned char**) &krnl_bin,
					  NULL, &err);

    std::cout << "Create Kernel: pass" << std::endl;
    m_kernel = clCreateKernel(m_program, "pass", &err);
    if (err != CL_SUCCESS) {
      std::cout << "FAILED TEST - Kernel Creation" << std::endl;
      exit(err);
    }

    if(oooQueue) {
      std::cout << "Create Out of Order Queue" << std::endl;
      m_queue = clCreateCommandQueue(m_context,
				     m_device_id, 
				     CL_QUEUE_PROFILING_ENABLE |
				     CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
				     &err);
    } else {
      std::cout << "Create Sequential Queue" << std::endl;
      m_queue = clCreateCommandQueue(m_context,
				     m_device_id, 
				     CL_QUEUE_PROFILING_ENABLE,
				     &err);
    }      
    if (err != CL_SUCCESS) {
      std::cout << "FAILED TEST - Command Queue Creation" << std::endl;
      exit(err);
    }

    std::cout << "Setup Complete" << std::endl;
  }
  
  ~ApiHandle() {
    clReleaseProgram(m_program);
    clReleaseKernel(m_kernel);
    clReleaseCommandQueue(m_queue);
    clReleaseContext(m_context);
  }
};

#endif

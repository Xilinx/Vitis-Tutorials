/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include "xclbin_helper.h"

cl_platform_id getVendorPlatform(std::string Vendor) 
{
  cl_uint platform_count;
  cl_platform_id platform_id;

  // Get number of platforms
  clGetPlatformIDs(0, nullptr, &platform_count);

  // Get all platforms
  std::vector<cl_platform_id> platforms(platform_count);
  clGetPlatformIDs(platform_count, platforms.data(), nullptr);

  char platform_vendor[1000];
  char platform_name[1000];

  // Look for first platform matching Vendor name
  bool found = false;
  for (int p = 0; p < (int)platform_count; ++p) {  
    platform_id = platforms[p];
    clGetPlatformInfo(platform_id, CL_PLATFORM_VENDOR, 1000, (void *)platform_vendor,NULL);
    clGetPlatformInfo(platform_id, CL_PLATFORM_NAME  , 1000, (void *)platform_name,  NULL);
    std::cout <<"\n Debug platform vendor"<<platform_vendor<<std::endl;
    std::cout <<"\n Debug platform Name"<<platform_name<<std::endl;
    if(Vendor.compare(platform_vendor) == 0) {
      found = true;
      break;
    }
  }
  
  if (!found){
    std::cout << "ERROR: failed to find "  << Vendor << " platform\n";
	exit(1);
  }

  return platform_id;
}

cl_device_id getDeviceId(cl_platform_id platform_id) 
{
  int err;
  cl_device_id device_id;
  err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ACCELERATOR, 1, &device_id, NULL);
  if (err != CL_SUCCESS) {
    std::cout << "ERROR: failed to get device_id\n";
	exit(1);
  }
  return device_id;
}

cl_context createContext(cl_device_id device_id) 
{
  int err;
  cl_context context;

  context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  if (err != CL_SUCCESS) {
    std::cout << "ERROR: Failed to create a compute context" << std::endl;
    exit(1);
  }
  return context;
}

cl_program createProgram(const char* filename, cl_context context, cl_device_id device_id)
{
  int err;
  cl_program program;

  std::ifstream fs(filename, std::ifstream::binary);

  if(!fs) {
    std::cout << "ERROR: Could not open " << filename << std::endl;
    exit(1);
  }

  std::string bin;
  fs.seekg(0, std::ios::end);
  bin.reserve(1+fs.tellg());
  fs.seekg(0, std::ios::beg);
  bin.assign(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());
  bin += '\0';

  size_t bin_size = bin.length()-1;
  const unsigned char* bin_ptr  = (const unsigned char*) bin.c_str();

  program = clCreateProgramWithBinary(context, 1, &device_id, &bin_size, &bin_ptr, NULL, &err);
  if ( !program || err != CL_SUCCESS) {
    std::cout << "Error: Failed to create compute program from binary: " << err << std::endl;
    exit(1);
  }

  err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (err != CL_SUCCESS) {
    std::cout << "Error: Failed to build program executable!" << std::endl;
	exit(1);
  }
  
  return program;
}


void load_xclbin_file(const char* filename, cl_context &context, cl_device_id &device_id, cl_program &program)
{
  cl_platform_id platform_id;

  platform_id = getVendorPlatform("Xilinx");
  device_id   = getDeviceId(platform_id);
  context     = createContext(device_id);
  program     = createProgram(filename, context, device_id);
}


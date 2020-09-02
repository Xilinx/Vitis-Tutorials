/**********
Copyright (c) 2020, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/

#pragma once

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY 1
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

// OCL_CHECK doesn't work if call has templatized function call
#define OCL_CHECK(error, call)                                                 \
  call;                                                                        \
  if (error != CL_SUCCESS) {                                                   \
    printf("%s:%d Error calling " #call ", error code is: %d\n", __FILE__,     \
           __LINE__, error);                                                   \
    exit(EXIT_FAILURE);                                                        \
  }

#include <CL/cl2.hpp>
#include <CL/cl_ext_xilinx.h>
#include <fstream>
#include <iostream>
// When creating a buffer with user pointer (CL_MEM_USE_HOST_PTR), under the
// hood
// User ptr is used if and only if it is properly aligned (page aligned). When
// not
// aligned, runtime has no choice but to create its own host side buffer that
// backs
// user ptr. This in turn implies that all operations that move data to and from
// device incur an extra memcpy to move data to/from runtime's own host buffer
// from/to user pointer. So it is recommended to use this allocator if user wish
// to
// Create Buffer/Memory Object with CL_MEM_USE_HOST_PTR to align user buffer to
// the
// page boundary. It will ensure that user buffer will be used when user create
// Buffer/Mem Object with CL_MEM_USE_HOST_PTR.
template <typename T> struct aligned_allocator {
  using value_type = T;

  aligned_allocator() {}

  aligned_allocator(const aligned_allocator &) {}

  template <typename U> aligned_allocator(const aligned_allocator<U> &) {}

  T *allocate(std::size_t num) {
    void *ptr = nullptr;

#if defined(_WINDOWS)
    {
      ptr = _aligned_malloc(num * sizeof(T), 4096);
      if (ptr == NULL) {
        std::cout << "Failed to allocate memory" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
#else
    {
      if (posix_memalign(&ptr, 4096, num * sizeof(T)))
        throw std::bad_alloc();
    }
#endif
    return reinterpret_cast<T *>(ptr);
  }
  void deallocate(T *p, std::size_t num) {
#if defined(_WINDOWS)
    _aligned_free(p);
#else
    free(p);
#endif
  }
};

namespace xcl {
std::vector<cl::Device> get_xil_devices();
std::vector<cl::Device> get_devices(const std::string &vendor_name);
std::vector<unsigned char>
read_binary_file(const std::string &xclbin_file_name);
bool is_emulation();
bool is_hw_emulation();
bool is_xpr_device(const char *device_name);
class Stream {
public:
  static decltype(&clCreateStream) createStream;
  static decltype(&clReleaseStream) releaseStream;
  static decltype(&clReadStream) readStream;
  static decltype(&clWriteStream) writeStream;
  static decltype(&clPollStreams) pollStreams;
  static void init(const cl_platform_id &platform) {
    void *bar =
        clGetExtensionFunctionAddressForPlatform(platform, "clCreateStream");
    createStream = (decltype(&clCreateStream))bar;
    bar = clGetExtensionFunctionAddressForPlatform(platform, "clReleaseStream");
    releaseStream = (decltype(&clReleaseStream))bar;
    bar = clGetExtensionFunctionAddressForPlatform(platform, "clReadStream");
    readStream = (decltype(&clReadStream))bar;
    bar = clGetExtensionFunctionAddressForPlatform(platform, "clWriteStream");
    writeStream = (decltype(&clWriteStream))bar;
    bar = clGetExtensionFunctionAddressForPlatform(platform, "clPollStreams");
    pollStreams = (decltype(&clPollStreams))bar;
  }
};
class P2P {
public:
  static decltype(&xclGetMemObjectFd) getMemObjectFd;
  static decltype(&xclGetMemObjectFromFd) getMemObjectFromFd;
  static void init(const cl_platform_id &platform) {
    void *bar =
        clGetExtensionFunctionAddressForPlatform(platform, "xclGetMemObjectFd");
    getMemObjectFd = (decltype(&xclGetMemObjectFd))bar;
    bar = clGetExtensionFunctionAddressForPlatform(platform, "xclGetMemObjectFromFd");
    getMemObjectFromFd = (decltype(&xclGetMemObjectFromFd))bar;
}
};
class Ext {
public:
  static decltype(&xclGetComputeUnitInfo) getComputeUnitInfo;
  static void init(const cl_platform_id &platform) {
    void *bar =
        clGetExtensionFunctionAddressForPlatform(platform, "xclGetComputeUnitInfo");
    getComputeUnitInfo = (decltype(&xclGetComputeUnitInfo))bar;
}
};
}

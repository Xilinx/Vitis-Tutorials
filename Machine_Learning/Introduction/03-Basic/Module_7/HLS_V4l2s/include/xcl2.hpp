/*
 * Copyright 2020 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#pragma once

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY 1
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
//OCL_CHECK doesn't work if call has templatized function call
#define OCL_CHECK(error,call)                                       \
    call;                                                           \
    if (error != CL_SUCCESS) {                                      \
      printf("%s:%d Error calling " #call ", error code is: %d\n",  \
              __FILE__,__LINE__, error);                            \
      exit(EXIT_FAILURE);                                           \
    }                                       

#include <CL/cl2.hpp>
#include <iostream>
#include <fstream>
#include <CL/cl_ext_xilinx.h>
#include "line_exception.hpp"
// When creating a buffer with user pointer (CL_MEM_USE_HOST_PTR), under the hood
// User ptr is used if and only if it is properly aligned (page aligned). When not 
// aligned, runtime has no choice but to create its own host side buffer that backs
// user ptr. This in turn implies that all operations that move data to and from 
// device incur an extra memcpy to move data to/from runtime's own host buffer 
// from/to user pointer. So it is recommended to use this allocator if user wish to
// Create Buffer/Memory Object with CL_MEM_USE_HOST_PTR to align user buffer to the
// page boundary. It will ensure that user buffer will be used when user create 
// Buffer/Mem Object with CL_MEM_USE_HOST_PTR.
template <typename T>
struct aligned_allocator
{
  using value_type = T;
  T* allocate(std::size_t num)
  {
    void* ptr = nullptr;
    if (posix_memalign(&ptr,4096,num*sizeof(T)))
      throw std::bad_alloc();
    return reinterpret_cast<T*>(ptr);
  }
  void deallocate(T* p, std::size_t num)
  {
    free(p);
  }
};

namespace xcl {

   class XilinxOcl
    {
    private:
        bool is_initialized = false;
        cl::Device device_0;
        cl::Context context;
        cl::Program program;

     std::vector<cl::Device> find_xilinx_devices();   
    public:
        XilinxOcl(){};
        ~XilinxOcl(){};

        void initialize(std::string xclbin_file_name);

        cl::CommandQueue get_command_queue(bool in_order         = false,
                                           bool enable_profiling = false);
        cl::Kernel get_kernel(std::string kernel_name);
        cl::Buffer create_buffer(size_t size, cl_mem_flags flags);
        cl::Buffer create_buffer_in_bank(int bank, size_t size, cl_mem_flags flags);
        cl::Buffer create_sub_buffer(cl::Buffer buf_in,
                                     size_t origin,
                                     size_t size,
                                     cl_mem_flags flags);
        int get_fd_for_buffer(cl::Buffer buf);
        cl::Buffer get_buffer_from_fd(int fd);
        size_t get_buffer_size(cl::Buffer buf);
    };





  std::vector<cl::Device> get_xil_devices();
  std::vector<cl::Device> get_devices(const std::string& vendor_name);
  std::vector<unsigned char> read_binary_file(const std::string &xclbin_file_name); 
  bool is_emulation ();
  bool is_hw_emulation ();
  bool is_xpr_device (const char *device_name);
    class Stream{
      public:
        static decltype(&clCreateStream) createStream;
        static decltype(&clReleaseStream) releaseStream;
        static decltype(&clReadStream) readStream;
        static decltype(&clWriteStream) writeStream;
        static decltype(&clPollStreams) pollStreams;
        static void init(const cl_platform_id& platform) {
            void *bar = clGetExtensionFunctionAddressForPlatform(platform, "clCreateStream");
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
}

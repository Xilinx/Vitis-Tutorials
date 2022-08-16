#ifndef XILINX_OCL_H_
#define XILINX_OCL_H

#pragma once

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY 1

#include "line_exception.hpp"

#include <CL/cl2.hpp> //"/opt/intel/opencl-1.2-4.4.0.117/include/CL/cl.h"
#include <CL/cl_ext_xilinx.h>
#include <fstream>
#include <iostream>
#include <string>

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
    T *allocate(std::size_t num)
    {
        void *ptr = nullptr;
        if (posix_memalign(&ptr, 4096, num * sizeof(T)))
            throw std::bad_alloc();
        return reinterpret_cast<T *>(ptr);
    }
    void deallocate(T *p, std::size_t num)
    {
        free(p);
    }
};

namespace xilinx {
namespace example_utils {
class XilinxOclHelper
{
private:
    bool is_initialized = false;
    cl::Device device;
    cl::Context context;
    cl::Program program;

    std::vector<cl::Device> find_xilinx_devices();

public:
    XilinxOclHelper();
    ~XilinxOclHelper();

    void initialize(std::string xclbin_file_name);

    cl::CommandQueue get_command_queue(bool in_order         = false,
                                       bool enable_profiling = false);
    cl::Kernel get_kernel(std::string kernel_name);
    cl::Buffer create_buffer(size_t size, cl_mem_flags flags);
    cl::Buffer create_buffer_in_bank(int bank, size_t size, cl_mem_flags flags);
    int get_fd_for_buffer(cl::Buffer buf);
    cl::Buffer get_buffer_from_fd(int fd);
    const cl::Context &get_context();
};
} // namespace example_utils
} // namespace xilinx
#endif

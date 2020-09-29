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

#include "xilinx_ocl.hpp"

#include <unistd.h>

namespace swm {
    std::vector<cl::Device> XilinxOcl::find_xilinx_devices()
    {
        size_t i;
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);

        cl::Platform platform;
        for (i = 0; i < platforms.size(); i++) {
            platform                  = platforms[i];
            std::string platform_name = platform.getInfo<CL_PLATFORM_NAME>();
            if (platform_name == "Xilinx") {
                break;
            }
        }
        if (i == platforms.size()) {
            throw_lineexception("Unable to find Xilinx OpenCL devices");
        }

        // Get ACCELERATOR devices
        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices);
        return devices;
    }

    void XilinxOcl::initialize(std::string xclbin_file_name)
    {
        // Find Xilinx OpenCL devices
        std::vector<cl::Device> devices = find_xilinx_devices();

        // Initialize our OpenCL context
        device  = devices[0];
        context = cl::Context(device);

        // Load the XCLBIN
        if (access(xclbin_file_name.c_str(), R_OK) != 0) {
            throw_lineexception("Specified XCLBIN not found");
        }

        std::ifstream xclbin(xclbin_file_name.c_str(), std::ifstream::binary);
        xclbin.seekg(0, xclbin.end);
        unsigned int nb = xclbin.tellg();
        xclbin.seekg(0, xclbin.beg);
        char *buf = new char[nb];
        xclbin.read(buf, nb);
        cl::Program::Binaries bins;
        bins.push_back({buf, nb});

        // TODO: Don't automatically assume that device 0 is the correct one
        devices.resize(1);

        // Program the device
        program = cl::Program(context, devices, bins);

        is_initialized = true;
    }

    cl::Kernel XilinxOcl::get_kernel(std::string kernel_name)
    {
        if (!is_initialized) {
            throw_lineexception("Attempted to get kernel without initializing OCL");
        }

        cl_int error;

        OCL_CHECK(error, cl::Kernel krnl(program, kernel_name.c_str(), &error));
        return krnl;
    }

    cl::CommandQueue XilinxOcl::get_command_queue(bool in_order, bool enable_profiling)
    {
        if (!is_initialized) {
            throw_lineexception("Attempted to get command queue without initializing OCL");
        }

        cl_command_queue_properties props = 0;
        cl_int err;

        if (!in_order) {
            props |= CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
        }
        if (enable_profiling) {
            props |= CL_QUEUE_PROFILING_ENABLE;
        }

        OCL_CHECK(err, cl::CommandQueue q(context, device, props, &err));
        return q;
    }

    cl::Buffer XilinxOcl::create_buffer(size_t size, cl_mem_flags flags)
    {
        if (!is_initialized) {
            throw_lineexception("Attempted to create buffer before initialization");
        }
        cl_int err;

        OCL_CHECK(err, cl::Buffer buf(context, flags, size, NULL, &err));
        return buf;
    }

    cl::Buffer XilinxOcl::create_buffer_in_bank(int bank, size_t size, cl_mem_flags flags)
    {
        if (!is_initialized) {
            throw_lineexception("Attempted to create buffer before initialization");
        }

        cl_int err;
        cl_mem_ext_ptr_t bank_ext;
        bank_ext.flags = bank | XCL_MEM_TOPOLOGY;
        bank_ext.obj   = NULL;
        bank_ext.param = 0;

        OCL_CHECK(err, cl::Buffer buf(context, flags | CL_MEM_EXT_PTR_XILINX, size, &bank_ext, &err));
        return buf;
    }

    cl::Buffer XilinxOcl::create_sub_buffer(cl::Buffer buf_in, size_t origin, size_t size, cl_mem_flags flags)
    {
        if (!is_initialized) {
            throw_lineexception("Attempted to create sub-buffer before initialization");
        }

        cl_int err;

        cl_buffer_region reg;
        reg.origin = origin;
        reg.size   = size;

        cl::Buffer subBuf = buf_in.createSubBuffer(flags, CL_BUFFER_CREATE_TYPE_REGION, &reg, &err);

        if (err != CL_SUCCESS) {
            throw_lineexception_errno("ERROR Calling create_sub_buffer", err);
        }

        return subBuf;
    }

    int XilinxOcl::get_fd_for_buffer(cl::Buffer buf)
    {
        int fd;
        xclGetMemObjectFd(buf(), &fd);
        return fd;
    }

    cl::Buffer XilinxOcl::get_buffer_from_fd(int fd)
    {
        cl::Buffer buffer;
        xclGetMemObjectFromFd(context(), device(), 0, fd, &buffer());
        return buffer;
    }

    size_t XilinxOcl::get_buffer_size(cl::Buffer buf)
    {
        size_t size;
        clGetMemObjectInfo(buf.get(), CL_MEM_SIZE, sizeof(size_t), &size, NULL);
        return size;
    }

    XilinxOcl::XilinxOcl()
    {
    }

    XilinxOcl::~XilinxOcl()
    {
    }

} // namespace swm

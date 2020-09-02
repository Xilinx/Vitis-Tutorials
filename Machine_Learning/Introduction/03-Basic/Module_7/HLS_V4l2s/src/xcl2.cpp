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

#include "xcl2.hpp"
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

namespace xcl
{
    std::vector<cl::Device> XilinxOcl::find_xilinx_devices()
    {
        size_t i;
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);

        cl::Platform platform;
        for (i = 0; i < platforms.size(); i++)
        {
            platform = platforms[i];
            std::string platform_name = platform.getInfo<CL_PLATFORM_NAME>();
            if (platform_name == "Xilinx")
            {
                break;
            }
        }
        if (i == platforms.size())
        {
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
        device_0 = devices[0];
        context = cl::Context(device_0);
      
        // Load the XCLBIN
        if (access(xclbin_file_name.c_str(), R_OK) != 0)
        {
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
        if (!is_initialized)
        {
            throw_lineexception("Attempted to get kernel without initializing OCL");
        }

        cl_int error;

        OCL_CHECK(error, cl::Kernel krnl(program, kernel_name.c_str(), &error));
        return krnl;
    }

    cl::CommandQueue XilinxOcl::get_command_queue(bool in_order, bool enable_profiling)
    {
        if (!is_initialized)
        {
            throw_lineexception("Attempted to get command queue without initializing OCL");
        }

        cl_command_queue_properties props = 0;
        cl_int err;

        if (!in_order)
        {
            props |= CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
        }
        if (enable_profiling)
        {
            props |= CL_QUEUE_PROFILING_ENABLE;
        }

        OCL_CHECK(err, cl::CommandQueue q(context, device_0, props, &err));
        return q;
    }
    cl::Buffer XilinxOcl::create_buffer(size_t size, cl_mem_flags flags)
    {
        if (!is_initialized)
        {
            throw_lineexception("Attempted to create buffer before initialization");
        }
        cl_int err;

        OCL_CHECK(err, cl::Buffer buf(context, flags, size, NULL, &err));
        return buf;
    }

    cl::Buffer XilinxOcl::create_buffer_in_bank(int bank, size_t size, cl_mem_flags flags)
    {
        if (!is_initialized)
        {
            throw_lineexception("Attempted to create buffer before initialization");
        }

        cl_int err;
        cl_mem_ext_ptr_t bank_ext;
        bank_ext.flags = bank | XCL_MEM_TOPOLOGY;
        bank_ext.obj = NULL;
        bank_ext.param = 0;

        OCL_CHECK(err, cl::Buffer buf(context, flags | CL_MEM_EXT_PTR_XILINX, size, &bank_ext, &err));
        return buf;
    }

    cl::Buffer XilinxOcl::create_sub_buffer(cl::Buffer buf_in, size_t origin, size_t size, cl_mem_flags flags)
    {
        if (!is_initialized)
        {
            throw_lineexception("Attempted to create sub-buffer before initialization");
        }

        cl_int err;

        cl_buffer_region reg;
        reg.origin = origin;
        reg.size = size;

        cl::Buffer subBuf = buf_in.createSubBuffer(flags, CL_BUFFER_CREATE_TYPE_REGION, &reg, &err);

        if (err != CL_SUCCESS)
        {
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
        xclGetMemObjectFromFd(context(), device_0(), 0, fd, &buffer());
        return buffer;
    }
    size_t XilinxOcl::get_buffer_size(cl::Buffer buf)
    {
        size_t size;
        clGetMemObjectInfo(buf.get(), CL_MEM_SIZE, sizeof(size_t), &size, NULL);
        return size;
    }

    std::vector<cl::Device> get_devices(const std::string &vendor_name)
    {
        size_t i;
        cl_int err;
        std::vector<cl::Platform> platforms;
        OCL_CHECK(err, err = cl::Platform::get(&platforms));
        cl::Platform platform;
        for (i = 0; i < platforms.size(); i++)
        {
            platform = platforms[i];
            OCL_CHECK(err,
                      std::string platformName =
                          platform.getInfo<CL_PLATFORM_NAME>(&err));
            if (platformName == vendor_name)
            {
                std::cout << "Found Platform" << std::endl;
                std::cout << "Platform Name: " << platformName.c_str() << std::endl;
                break;
            }
        }
        if (i == platforms.size())
        {
            std::cout << "Error: Failed to find Xilinx platform" << std::endl;
            exit(EXIT_FAILURE);
        }
        //Getting ACCELERATOR Devices and selecting 1st such device
        std::vector<cl::Device> devices;
        OCL_CHECK(err,
                  err = platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices));
        return devices;
    }

    std::vector<cl::Device> get_xil_devices() { return get_devices("Xilinx"); }

    std::vector<unsigned char> read_binary_file(const std::string &xclbin_file_name)
    {
        std::cout << "INFO: Reading " << xclbin_file_name << std::endl;

        if (access(xclbin_file_name.c_str(), R_OK) != 0)
        {
            printf("ERROR: %s xclbin not available please build\n",
                   xclbin_file_name.c_str());
            exit(EXIT_FAILURE);
        }
        //Loading XCL Bin into char buffer
        std::cout << "Loading: '" << xclbin_file_name.c_str() << "'\n";
        std::ifstream bin_file(xclbin_file_name.c_str(), std::ifstream::binary);
        bin_file.seekg(0, bin_file.end);
        auto nb = bin_file.tellg();
        bin_file.seekg(0, bin_file.beg);
        std::vector<unsigned char> buf;
        buf.resize(nb);
        bin_file.read(reinterpret_cast<char *>(buf.data()), nb);
        return buf;
    }

    bool is_emulation()
    {
        bool ret = false;
        char *xcl_mode = getenv("XCL_EMULATION_MODE");
        if (xcl_mode != NULL)
        {
            ret = true;
        }
        return ret;
    }

    bool is_hw_emulation()
    {
        bool ret = false;
        char *xcl_mode = getenv("XCL_EMULATION_MODE");
        if ((xcl_mode != NULL) && !strcmp(xcl_mode, "hw_emu"))
        {
            ret = true;
        }
        return ret;
    }

    bool is_xpr_device(const char *device_name)
    {
        const char *output = strstr(device_name, "xpr");

        if (output == NULL)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}; // namespace xcl

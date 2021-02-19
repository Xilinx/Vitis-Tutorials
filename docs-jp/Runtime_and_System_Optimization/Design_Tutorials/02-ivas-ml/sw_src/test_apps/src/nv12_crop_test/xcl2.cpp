/**********
Copyright (c) 2018, Xilinx, Inc.
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
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/

#include "xcl2.hpp"

#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

int load_file_to_memory(const char *filename, char **result)
{
    uint size = 0;
    FILE *f   = fopen(filename, "rb");
    if (f == NULL) {
        *result = NULL;
        return -1; // -1 means file opening fail
    }
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    *result = (char *)malloc(size + 1);
    if (size != fread(*result, sizeof(char), size, f)) {
        free(*result);
        return -2; // -2 means file reading fail
    }
    fclose(f);
    (*result)[size] = 0;
    return size;
}

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
        clGetPlatformInfo(platform_id, CL_PLATFORM_VENDOR, 1000, (void *)platform_vendor, NULL);
        clGetPlatformInfo(platform_id, CL_PLATFORM_NAME, 1000, (void *)platform_name, NULL);
        if (Vendor.compare(platform_vendor) == 0) {
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "ERROR: failed to find " << Vendor << " platform\n";
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

cl_program createProgram(const char *filename, cl_context context, cl_device_id device_id)
{
    int err;
    cl_program program;

    unsigned char *kernelbinary;
    printf("INFO: loading xclbin %s\n", filename);
    int size        = load_file_to_memory(filename, (char **)&kernelbinary);
    size_t size_var = size;

    program = clCreateProgramWithBinary(context, 1, &device_id, &size_var, (const unsigned char **)&kernelbinary, NULL, &err);
    if (!program || err != CL_SUCCESS) {
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


void load_xclbin_file(const char *filename, cl_context &context, cl_device_id &device_id, cl_program &program)
{
    cl_platform_id platform_id;
    platform_id = getVendorPlatform("Xilinx");
    device_id   = getDeviceId(platform_id);
    context     = createContext(device_id);
    program     = createProgram(filename, context, device_id);
}

namespace xcl {
    std::vector<cl::Device> get_devices(const std::string &vendor_name)
    {
        size_t i;
        cl_int err;
        std::vector<cl::Platform> platforms;
        OCL_CHECK(err, err = cl::Platform::get(&platforms));
        cl::Platform platform;
        for (i = 0; i < platforms.size(); i++) {
            platform = platforms[i];
            OCL_CHECK(err,
                      std::string platformName =
                          platform.getInfo<CL_PLATFORM_NAME>(&err));
            if (platformName == vendor_name) {
                std::cout << "Found Platform" << std::endl;
                std::cout << "Platform Name: " << platformName.c_str() << std::endl;
                break;
            }
        }
        if (i == platforms.size()) {
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

        if (access(xclbin_file_name.c_str(), R_OK) != 0) {
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
        bool ret       = false;
        char *xcl_mode = getenv("XCL_EMULATION_MODE");
        if (xcl_mode != NULL) {
            ret = true;
        }
        return ret;
    }

    bool is_hw_emulation()
    {
        bool ret       = false;
        char *xcl_mode = getenv("XCL_EMULATION_MODE");
        if ((xcl_mode != NULL) && !strcmp(xcl_mode, "hw_emu")) {
            ret = true;
        }
        return ret;
    }

    bool is_xpr_device(const char *device_name)
    {
        const char *output = strstr(device_name, "xpr");

        if (output == NULL) {
            return false;
        }
        else {
            return true;
        }
    }
}; // namespace xcl

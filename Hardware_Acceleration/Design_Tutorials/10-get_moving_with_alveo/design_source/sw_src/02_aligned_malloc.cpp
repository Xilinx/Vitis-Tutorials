/**********
Copyright (c) 2019, Xilinx, Inc.
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


#include "event_timer.hpp"

#include <iostream>
#include <memory>
#include <string>

// Xilinx OpenCL and XRT includes
#include "xilinx_ocl_helper.hpp"

#define BUFSIZE (1024 * 1024 * 6)

void vadd_sw(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t size)
{
    for (int i = 0; i < size; i++) {
        c[i] = a[i] + b[i];
    }
}

int main(int argc, char *argv[])
{
    // Initialize an event timer we'll use for monitoring the application
    EventTimer et;

    std::cout << "-- Example 2: Vector Add with Aligned Allocation --" << std::endl
              << std::endl;

    // Initialize the runtime (including a command queue) and load the
    // FPGA image
    std::cout << "Loading alveo_examples.xclbin to program the Alveo board" << std::endl
              << std::endl;
    et.add("OpenCL Initialization");

    // This application will use the first Xilinx device found in the system
    xilinx::example_utils::XilinxOclHelper xocl;
    xocl.initialize("alveo_examples.xclbin");

    cl::CommandQueue q = xocl.get_command_queue();
    cl::Kernel krnl    = xocl.get_kernel("vadd");
    et.finish();

    /// New code for example 01
    std::cout << "Running kernel test with aligned virtual buffers" << std::endl;

    et.add("Allocating memory buffer");
    uint32_t *a, *b, *c, *d = NULL;
    int ret;
    ret = posix_memalign((void **)&a, 4096, BUFSIZE * sizeof(uint32_t));
    ret |= posix_memalign((void **)&b, 4096, BUFSIZE * sizeof(uint32_t));
    ret |= posix_memalign((void **)&c, 4096, BUFSIZE * sizeof(uint32_t));
    ret |= posix_memalign((void **)&d, 4096, BUFSIZE * sizeof(uint32_t));
    if (ret != 0) {
        std::cout << "Error allocating aligned memory!" << std::endl;
        exit(1);
    }

    et.finish();

    et.add("Populating buffer inputs");
    for (int i = 0; i < BUFSIZE; i++) {
        a[i] = i;
        b[i] = 2 * i;
    }
    et.finish();

    // For comparison, let's have the CPU calculate the result
    et.add("Software VADD run");
    vadd_sw(a, b, d, BUFSIZE);
    et.finish();

    // Map our user-allocated buffers as OpenCL buffers using a shared
    // host pointer
    et.add("Map host buffers to OpenCL buffers");
    std::vector<cl::Memory> inBufVec, outBufVec;
    cl::Buffer a_to_device(xocl.get_context(),
                           static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR),
                           BUFSIZE * sizeof(int),
                           a,
                           NULL);
    cl::Buffer b_to_device(xocl.get_context(),
                           static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR),
                           BUFSIZE * sizeof(int),
                           b,
                           NULL);
    cl::Buffer c_from_device(xocl.get_context(),
                             static_cast<cl_mem_flags>(CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR),
                             BUFSIZE * sizeof(int),
                             c,
                             NULL);
    et.finish();

    // Set vadd kernel arguments
    et.add("Set kernel arguments");
    krnl.setArg(0, a_to_device);
    krnl.setArg(1, b_to_device);
    krnl.setArg(2, c_from_device);
    krnl.setArg(3, BUFSIZE);

    // Send the buffers down to the Alveo card
    et.add("Memory object migration enqueue");
    cl::Event event_sp;
    q.enqueueMigrateMemObjects({a_to_device, b_to_device}, 0, NULL, &event_sp);
    clWaitForEvents(1, (const cl_event *)&event_sp);

    et.add("OCL Enqueue task");

    q.enqueueTask(krnl, NULL, &event_sp);
    et.add("Wait for kernel to complete");
    clWaitForEvents(1, (const cl_event *)&event_sp);

    // Migrate memory back from device
    et.add("Read back computation results");
    q.enqueueMigrateMemObjects({c_from_device}, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &event_sp);
    clWaitForEvents(1, (const cl_event *)&event_sp);
    et.finish();

    // Verify the results
    bool verified = true;
    for (int i = 0; i < BUFSIZE; i++) {
        if (c[i] != d[i]) {
            verified = false;
            std::cout << "ERROR: software and hardware vadd do not match: "
                      << c[i] << "!=" << d[i] << " at position " << i << std::endl;
            break;
        }
    }

    if (verified) {
        std::cout
            << std::endl
            << "Simple malloc vadd example complete!"
            << std::endl
            << std::endl;
    }
    else {
        std::cout
            << std::endl
            << "Simple malloc vadd example complete! (with errors)"
            << std::endl
            << std::endl;
    }

    std::cout << "--------------- Key execution times ---------------" << std::endl;

    q.finish();

    free(a);
    free(b);
    free(c);
    free(d);

    et.print();
}

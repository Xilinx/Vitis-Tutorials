/**
* Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: X11
*/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <iomanip>

#include "xrt/xrt_kernel.h"

#define DATA_SIZE 4*1024

int main(int argc, char** argv) {

    // Data Transfer from Device2 ---> Device1
    
    auto device1_bdf= "0000:65:00.1"; // Device1 BDF, P2P should be enabled for this device
    auto device2_bdf= "0000:b3:00.1"; // Device2 BDF, This device doing DMA-READ or DMA-WRITE, P2P enablement is not required

    // We are loading a simple increment kernel on device2
    std::string xclbin_file = "kernel.xclbin";
    auto device2 = xrt::device(device2_bdf);
    auto uuid = device2.load_xclbin(xclbin_file);
    auto krnl = xrt::kernel(device2, uuid, "increment");

    // For device1 we are just transferring the data not doing any operation
    // But we need to load any kernel so that all the memory banks are visible by runtime
    // You can run xbutil validate memory bandwidth test that loads a pre-built kernel 
    // and hence all memory banks become visible by XRT
    // ... sample command 
    // xbutil validate --device 0000:65:00.1 -r mem-bw

    auto device1 = xrt::device(device1_bdf);

    // All Buffer allocations
    size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

    auto in1 = xrt::bo(device1, vector_size_bytes, xrt::bo::flags::normal, 0);
    auto in1_map = in1.map<int*>();

    auto out2 = xrt::bo(device2, in1_map, vector_size_bytes, krnl.group_id(1));
    auto out2_map = out2.map<int*>();

    auto in2 = xrt::bo(device2, vector_size_bytes, krnl.group_id(0));
    auto in2_map = in2.map<int*>();

    // Input Data
    int increment_value = 10;
    for (int i = 0; i < DATA_SIZE; ++i) {
        in2_map[i] = i;
    }


    in2.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    std::cout << "\n Execution of the kernel on device2\n";
    auto run = krnl(in2, out2, increment_value , DATA_SIZE);
    run.wait();

    int loop = 1024;
    std::chrono::high_resolution_clock::time_point t_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < loop; i++) {
        out2.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        in1.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    }
    std::chrono::high_resolution_clock::time_point t_end = std::chrono::high_resolution_clock::now();

    // Calculations
    unsigned long t_duration = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count();
    double msduration = (double)t_duration;
    double secduration = t_duration/(double)1000000;
    double gbpersec = ((loop*vector_size_bytes)/secduration) / ((double)1024*1024*1024);
    std::cout << "Buffer = " << vector_size_bytes << " Iterations = " << loop
                  << "\nThroughput= " << std::setprecision(2) << std::fixed << gbpersec << "GB/s";



    std::cout << "\nTEST PASSED\n";
    return 0;
}

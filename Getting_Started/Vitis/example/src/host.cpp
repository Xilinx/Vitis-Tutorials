#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

//#include "cmdlineparser.h"
#include <iostream>
#include <cstring>

// XRT includes
#include "xrt/xrt_bo.h"
#include <experimental/xrt_xclbin.h>
#include "xrt/xrt_device.h"
#include "xrt/xrt_kernel.h"

#define DATA_SIZE 4096

int main(int argc, char** argv) {

    std::cout << "argc = " << argc << std::endl;
	for(int i=0; i < argc; i++){
	    std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
	}

    // Read settings
    std::string binaryFile = "./vadd.xclbin";
    int device_index = 0;

    std::cout << "Open the device" << device_index << std::endl;
    auto device = xrt::device(device_index);
    std::cout << "Load the xclbin " << binaryFile << std::endl;
    auto uuid = device.load_xclbin("./vadd.xclbin");

    size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

    //auto krnl = xrt::kernel(device, uuid, "vadd");
    auto krnl = xrt::kernel(device, uuid, "vadd", xrt::kernel::cu_access_mode::exclusive);

    std::cout << "Allocate Buffer in Global Memory\n";
    auto boIn1 = xrt::bo(device, vector_size_bytes, krnl.group_id(0)); //Match kernel arguments to RTL kernel
    auto boIn2 = xrt::bo(device, vector_size_bytes, krnl.group_id(1));
    auto boOut = xrt::bo(device, vector_size_bytes, krnl.group_id(2));

    // Map the contents of the buffer object into host memory
    auto bo0_map = boIn1.map<int*>();
    auto bo1_map = boIn2.map<int*>();
    auto bo2_map = boOut.map<int*>();
    std::fill(bo0_map, bo0_map + DATA_SIZE, 0);
    std::fill(bo1_map, bo1_map + DATA_SIZE, 0);
    std::fill(bo2_map, bo2_map + DATA_SIZE, 0);

    // Create the test data
    int bufReference[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; ++i) {
        bo0_map[i] = i;
        bo1_map[i] = i;
        bufReference[i] = bo0_map[i] + bo1_map[i]; //Generate check data for validation
    }

    // Synchronize buffer content with device side
    std::cout << "synchronize input buffer data to device global memory\n";
    boIn1.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    boIn2.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    std::cout << "Execution of the kernel\n";
    auto run = krnl(boIn1, boIn2, boOut, DATA_SIZE); //DATA_SIZE=size
    run.wait();

    // Get the output;
    std::cout << "Get the output data from the device" << std::endl;
    boOut.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    // Validate results
    if (std::memcmp(bo2_map, bufReference, DATA_SIZE))
        throw std::runtime_error("Value read back does not match reference");

    std::cout << "TEST PASSED\n";
    return 0;
}

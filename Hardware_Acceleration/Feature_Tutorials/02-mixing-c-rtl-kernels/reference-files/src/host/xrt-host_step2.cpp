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

    std::string binaryFile;
    std::cout << "argc = " << argc << std::endl;
    for(int i=0; i < argc; i++){
	std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    }
    if (argc > 1) {
	binaryFile = argv[1];
    } else {
	binaryFile = "./krnl_vadd.hw_emu.xclbin";
    }

    // Read settings
    int device_index = 0;
    std::cout << "Open the device" << device_index << std::endl;
    auto device = xrt::device(device_index);
    std::cout << "Load the xclbin " << binaryFile << std::endl;
    auto uuid = device.load_xclbin(binaryFile);

    size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

    auto krnl = xrt::kernel(device, uuid, "krnl_vadd");
    auto krnlRTL = xrt::kernel(device, uuid, "rtl_kernel_wizard_0");

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

    // Create the test data for krnl
    int bufReference[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; ++i) {
      //std::cout << "DataSize = " << i << " iteration  " << std::endl;
        bo0_map[i] = i;
        bo1_map[i] = i;
        bufReference[i] = bo0_map[i] + bo1_map[i] + 1; //Generate check data for validation, +1 for krnlRTL
    }

    // Synchronize buffer content with device side
    std::cout << "synchronize input buffer data to device global memory\n";
    boIn1.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    boIn2.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    std::cout << "Execution of the vadd kernel\n";
    auto run1 = krnl(boIn1, boIn2, boOut, DATA_SIZE); //DATA_SIZE=size
    run1.wait();
    std::cout << "Execution of the RTL  kernel\n";
    auto run2 = krnlRTL(boOut); 
    run2.wait();

    // Get the output;
    std::cout << "Get the output data from the device" << std::endl;
    boOut.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    // Validate results
    if (std::memcmp(bo2_map, bufReference, vector_size_bytes))
        throw std::runtime_error("Value read back does not match reference");

    std::cout << "TEST PASSED\n";
    return 0;
}

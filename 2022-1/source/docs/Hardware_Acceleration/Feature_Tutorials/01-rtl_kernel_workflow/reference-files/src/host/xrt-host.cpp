/**
* Copyright (C) 2020 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/

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
    //std::string binaryFile = argv[1];
    std::string binaryFile = "./vadd.xclbin";
    //int device_index = (int)argv[2];
    int device_index = 0;

    std::cout << "Open the device" << device_index << std::endl;
    auto device = xrt::device(device_index);
    std::cout << "Load the xclbin " << binaryFile << std::endl;
    //auto uuid = device.load_xclbin(binaryFile);
    auto uuid = device.load_xclbin("./vadd.xclbin");

    size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

    //auto krnl = xrt::kernel(device, uuid, "vadd");
    auto krnl = xrt::kernel(device, uuid, "Vadd_A_B", xrt::kernel::cu_access_mode::exclusive);

    std::cout << "Allocate Buffer in Global Memory\n";
    auto boA = xrt::bo(device, vector_size_bytes, krnl.group_id(1)); //Match kernel arguments to RTL kernel
    auto boB = xrt::bo(device, vector_size_bytes, krnl.group_id(2));

    // Map the contents of the buffer object into host memory
    auto bo0_map = boA.map<int*>();
    auto bo1_map = boB.map<int*>();
    std::fill(bo0_map, bo0_map + DATA_SIZE, 0);
    std::fill(bo1_map, bo1_map + DATA_SIZE, 0);

    // Create the test data
    int bufReference[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; ++i) {
        bo0_map[i] = i;
        bo1_map[i] = i;
        bufReference[i] = bo0_map[i] + bo1_map[i]; //Generate check data for validation
    }

    // Synchronize buffer content with device side
    std::cout << "synchronize input buffer data to device global memory\n";

    boA.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    boB.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    std::cout << "Execution of the kernel\n";
    auto run = krnl(DATA_SIZE, boA, boB); //DATA_SIZE=size
    run.wait();

    // Get the output;
    std::cout << "Get the output data from the device" << std::endl;
    boB.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    // Validate results
    if (std::memcmp(bo1_map, bufReference, DATA_SIZE))
        throw std::runtime_error("Value read back does not match reference");

    std::cout << "TEST PASSED\n";
    return 0;
}

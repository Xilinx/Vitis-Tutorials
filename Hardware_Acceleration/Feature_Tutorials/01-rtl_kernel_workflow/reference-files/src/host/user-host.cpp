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
#include "xrt/xrt_device.h"
#include <experimental/xrt_xclbin.h>
#include <experimental/xrt_ip.h>

#define DATA_SIZE 4096
//#define DATA_SIZE 32
//Define Control Protocol
#define IP_START 0x1
#define IP_IDLE 0x4
#define USER_OFFSET 0x0


int main(int argc, char** argv) {

    std::cout << "argc = " << argc << std::endl;
	for(int i=0; i < argc; i++){
	    std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
	}

    // Read settings
    std::string binaryFile = argv[1];
    //std::string binaryFile = "./vadd.xclbin";
    auto xclbin = xrt::xclbin(binaryFile);
    int device_index = 0;

    std::cout << "Open the device" << device_index << std::endl;
    auto device = xrt::device(device_index);
    std::cout << "Load the xclbin " << binaryFile << std::endl;
    auto uuid = device.load_xclbin(binaryFile);
 
    size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

    xrt::xclbin::mem mem_used;
    auto ip = xrt::ip(device, uuid, "Vadd_A_B");

    std::cout << "Fetch compute Units" << std::endl;
    std::vector<xrt::xclbin::ip> cu;
    for (auto& kernel : xclbin.get_kernels()) {
        if (kernel.get_name() == "Vadd_A_B") {
            cu = kernel.get_cus();
        }
    }

    if (cu.empty()) throw std::runtime_error("IP krnl_vadd_rtl not found in the provided xclbin");

    std::cout << "Determine memory index\n";
    for (auto& mem : xclbin.get_mems()) {
        if (mem.get_used()) {
            mem_used = mem;
            std::cout << "Mem  Used Index: " << mem_used.get_index() << std::endl;
            break;
        }
    }

    std::cout << "Allocate Buffer in Global Memory\n";
    //auto boA = xrt::bo(device, vector_size_bytes, krnl.group_id(1)); //Match kernel arguments to RTL kernel
    //auto boB = xrt::bo(device, vector_size_bytes, krnl.group_id(2));
    auto boA = xrt::bo(device, vector_size_bytes, mem_used.get_index());
    auto boB = xrt::bo(device, vector_size_bytes, mem_used.get_index());

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

    std::cout << "loaded the data" << std::endl;
    uint64_t buf_addr[2];
    // Get the buffer physical address
    buf_addr[0] = boA.address();
    buf_addr[1] = boB.address();

    // Synchronize buffer content with device side
    std::cout << "synchronize input buffer data to device global memory\n";
    boA.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    boB.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    //std::cout << "Execution of the kernel\n";
    //auto run = krnl(DATA_SIZE, boA, boB); //DATA_SIZE=size
    //run.wait();

    std::cout << "INFO: Setting IP Data" << std::endl;
    auto args = cu[0].get_args();

    std::cout << "Setting the Scalar Register \"size\"" << std::endl;
    ip.write_register(args[0].get_offset(), DATA_SIZE);

    std::cout << "Setting the Register \"A\" (Input Address)" << std::endl;
    ip.write_register(args[1].get_offset(), buf_addr[0]);
    ip.write_register(args[1].get_offset() + 4, buf_addr[0] >> 32);

    std::cout << "Setting the Register \"B\" (Input Address)" << std::endl;
    ip.write_register(args[2].get_offset(), buf_addr[1]);
    ip.write_register(args[2].get_offset() + 4, buf_addr[1] >> 32);

    uint32_t axi_ctrl = IP_START;
    std::cout << "INFO: IP Start" << std::endl;
    //axi_ctrl = IP_START;
    ip.write_register(USER_OFFSET, axi_ctrl);

    // Wait until the IP is DONE
    int i = 0;
    //axi_ctrl = 0;
    while (axi_ctrl != IP_IDLE) {
    //while ((axi_ctrl & IP_IDLE) != IP_IDLE) {
        axi_ctrl = ip.read_register(USER_OFFSET);
        i = i + 1;
        std::cout << "Read Loop iteration: " << i << " and Axi Control = " << axi_ctrl << "\n";
        if (i > 100000) {
	  axi_ctrl = IP_IDLE;
          ip.write_register(USER_OFFSET, axi_ctrl);
        }
    }

    std::cout << "INFO: IP Done" << std::endl;
    // Get the output;
    std::cout << "Get the output data from the device" << std::endl;
    boB.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    // Validate results
    if (std::memcmp(bo1_map, bufReference, DATA_SIZE))
        throw std::runtime_error("Value read back does not match reference");

    std::cout << "TEST PASSED\n";
    return 0;
}

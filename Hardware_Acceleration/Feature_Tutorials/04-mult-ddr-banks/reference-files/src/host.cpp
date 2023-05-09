/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include <cstring>
#include <iostream>

// XRT includes
#include "experimental/xrt_bo.h"
#include "experimental/xrt_device.h"
#include "experimental/xrt_kernel.h"

#define DATA_SIZE 4096

int main(int argc, char **argv) {
  std::string xclbin_file_name = argv[1];

  // The host code assumes there is a single device and opening a device by
  // device index 0. If there are multiple devices then this device index needed
  // to be adjusted. The user can get the list of the devices and device indices
  // by xbtuil examine command.
  unsigned int device_index = 0;
  std::cout << "Open the device" << device_index << std::endl;
  auto device = xrt::device(device_index);
  
  std::cout << "Load the xclbin " << xclbin_file_name << std::endl;
  auto uuid = device.load_xclbin(xclbin_file_name);

  size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

  auto krnl = xrt::kernel(device, uuid, "vadd");

  std::cout << "Allocate Buffer in Global Memory\n";
  auto bo0 = xrt::bo(device, vector_size_bytes, krnl.group_id(0));
  auto bo1 = xrt::bo(device, vector_size_bytes, krnl.group_id(1));
  auto bo_out = xrt::bo(device, vector_size_bytes, krnl.group_id(2));

  // Map the contents of the buffer object into host memory
  auto bo0_map = bo0.map<int *>();
  auto bo1_map = bo1.map<int *>();
  auto bo_out_map = bo_out.map<int *>();
  std::fill(bo0_map, bo0_map + DATA_SIZE, 0);
  std::fill(bo1_map, bo1_map + DATA_SIZE, 0);
  std::fill(bo_out_map, bo_out_map + DATA_SIZE, 0);

  // Create the test data
  int bufReference[DATA_SIZE];
  for (int i = 0; i < DATA_SIZE; ++i) {
    bo0_map[i] = i;
    bo1_map[i] = i;
    bufReference[i] = bo0_map[i] + bo1_map[i];
  }

  // Synchronize buffer content with device side
  std::cout << "synchronize input buffer data to device global memory\n";

  bo0.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  bo1.sync(XCL_BO_SYNC_BO_TO_DEVICE);

  std::cout << "Execution of the kernel\n";
  auto run = krnl(bo0, bo1, bo_out, DATA_SIZE);
  run.wait();

  // Get the output;
  std::cout << "Get the output data from the device" << std::endl;
  bo_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

  // Validate our results
  if (std::memcmp(bo_out_map, bufReference, DATA_SIZE))
    throw std::runtime_error("Value read back does not match reference");

  std::cout << "TEST PASSED\n";
  return 0;
}

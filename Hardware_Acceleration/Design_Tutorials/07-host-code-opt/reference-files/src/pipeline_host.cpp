/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include <iostream>
#include <vector>
#include <chrono>
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include "CL/opencl.h"

#include "ApiHandle.h"
#include "Task.h"

int main(int argc, char* argv[]) {

  // -- Environment / Usage Check -------------------------------------------

  char *xcl_mode = getenv("XCL_EMULATION_MODE");

  if (argc != 2) {
    printf("\nUsage: %s "
	   "./xclbin/pass.<emulation_mode>.<dsa>.xclbin ",
	   argv[0]);
    return EXIT_FAILURE;
  }
  char*        binaryName   = argv[1];

  // -- Common Parameters ---------------------------------------------------

  unsigned int numBuffers               = 10;
  bool         oooQueue                 = false;
  unsigned int processDelay             = 1;
  unsigned int bufferSize               = 8 << 11;

  // -- Setup ---------------------------------------------------------------

  
  ApiHandle api(binaryName, oooQueue);

  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << " Total number of buffers: " << numBuffers   << std::endl;
  std::cout << "              BufferSize: " << bufferSize   << std::endl;
  std::cout << "        Bits per Element: " << 512          << std::endl;
  std::cout << "      Bytes per Transfer: " << bufferSize*512/8 << std::endl;
  std::cout << "            processDelay: " << processDelay << std::endl;
  std::cout << std::boolalpha;
  std::cout << "      Out of Order Queue: " << oooQueue << std::endl;
  std::cout << std::noboolalpha;
  std::cout << std::endl;

  std::vector<Task> tasks(numBuffers, Task(bufferSize, processDelay));
  
  std::cout << "Running FPGA" << std::endl;
  auto fpga_begin = std::chrono::high_resolution_clock::now();

  // -- Execution -----------------------------------------------------------
  
  for(unsigned int i=0; i < numBuffers; i++) {
    tasks[i].run(api);
  }
  clFinish(api.getQueue());
  
  // -- Testing -------------------------------------------------------------

  auto fpga_end = std::chrono::high_resolution_clock::now();

  bool outputOk = true;
  for(unsigned int i=0; i < numBuffers; i++) {
    outputOk = tasks[i].outputOk() && outputOk;
  }
  if(!outputOk) {
    std::cout << "FAIL: Output Corrupted" << std::endl;
    return 1;
  }

  // -- Performance Statistics ----------------------------------------------

  if (xcl_mode == NULL) {
    std::chrono::duration<double> fpga_duration = fpga_end - fpga_begin;

    double total = (double) bufferSize * numBuffers * 512 / (1024.0*1024.0);
    std::cout << std::endl;
    std::cout << "          Total data: " << total << " MBits" << std::endl;
    std::cout << "           FPGA Time: " << fpga_duration.count()
	      << " s" << std::endl;
    std::cout << "     FPGA Throughput: " 
	      << total / fpga_duration.count() 
	      << " MBits/s" << std::endl;
    std::cout << "FPGA PCIe Throughput: " 
	      << (2*total) / fpga_duration.count() 
	      << " MBits/s" << std::endl;
  }
  std::cout << "\nPASS: Simulation" << std::endl;

 return 0;
}

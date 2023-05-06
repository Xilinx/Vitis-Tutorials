// Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT


#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <xrt/xrt_device.h>
#include <xrt/xrt_kernel.h>
#include <xrt/xrt_graph.h>
#include <xrt/xrt_bo.h>

#include "input.h"
#include "golden.h"

#include "fir_graph.cpp"

#define  TYPE_DATA int16_t

static const uint32_t ITERATIONS  = 0;

static const char*    STR_ERROR   = "ERROR:   ";
static const char*    STR_FAILED  = "FAILED:  ";
static const char*    STR_PASSED  = "PASSED:  ";
static const char*    STR_INFO    = "INFO:    ";
static const char*    STR_USAGE   = "USAGE:   ";

int main(int argc, char* argv[]) {

  // Check command line
  if(argc < 2) {
    std::cout << STR_USAGE << argv[0] <<" <xclbin> <iterations>" << std::endl;
    return EXIT_FAILURE;
  }

  // Calculating samples / bsize
  uint32_t iterations = ITERATIONS;

  if(argc >= 3) {
    iterations = atoi(argv[2]);
  }

  /*
  //for AIE project here are the settings in words size
  #define INPUT_SIZE  (8192  ) //rows of complex data: Re + jIm
  #define OUTPUT_SIZE (8192/2) //rows of complex data: Re + jIm
  */
  int wSizeIn  = 8192;
  int bSizeIn  = wSizeIn  * sizeof(TYPE_DATA) * 2;
  int wSizeOut = wSizeIn  / 2;
  int bSizeOut = wSizeOut * sizeof(TYPE_DATA) * 2;

  std::cout << STR_INFO << "iterations    = " << iterations << std::endl;
  std::cout << STR_INFO << "words SizeIn  = " << wSizeIn    << std::endl;
  std::cout << STR_INFO << "words SizeOut = " << wSizeOut   << std::endl;
  std::cout << STR_INFO << "bytes SizeIn  = " << bSizeIn    << std::endl;
  std::cout << STR_INFO << "bytes SizeOut = " << bSizeOut   << std::endl;

  // Loading the xclbin
  char* xclbinFilename = argv[1];

  unsigned int dev_index = 0;
  auto my_device = xrt::device(dev_index);
  std::cout << STR_PASSED << "auto my_device = xrt::device(" << dev_index << ")" << std::endl;

  auto xclbin_uuid = my_device.load_xclbin(xclbinFilename);
  std::cout << STR_PASSED << "auto xclbin_uuid = my_device.load_xclbin(" << xclbinFilename << ")" << std::endl;

  // Starting the input mm2s kernels
  auto in_0 = xrt::kernel(my_device, xclbin_uuid, "mm2s:{mm2s_1}");
  std::cout << STR_PASSED << "auto in_0 = xrt::kernel(my_device, xclbin_uuid, \"mm2s:{mm2s_1}\")" << std::endl;

  auto in_0_bo = xrt::bo(my_device, bSizeIn, XCL_BO_FLAGS_NONE, in_0.group_id(0));
  std::cout << STR_PASSED << "auto in_0_bo = xrt::bo(my_device, bSizeIn, XCL_BO_FLAGS_NONE, in_0.group_id(0) (=" << in_0.group_id(0) << "))" << std::endl;

  auto in_0_bo_mapped = in_0_bo.map<TYPE_DATA*>();
  std::cout << STR_PASSED << "auto in_0_bo_mapped = = in_0_bo.map<TYPE_DATA*>()" << std::endl;

  //setting input data
  memcpy(in_0_bo_mapped, cint16input, bSizeIn);
  std::cout << STR_PASSED << "memcpy(in_0_bo_mapped, cint16input, bSizeIn)" << std::endl;

  in_0_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  std::cout << STR_PASSED << "in_0_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE)" << std::endl;

  auto in_0_run = in_0(in_0_bo, nullptr, bSizeIn);
  std::cout << STR_PASSED << "auto in_0_run = in_0(in_0_bo, nullptr, " << bSizeIn << ")" << std::endl;

  // Starting the output s2mm kernels
  auto out_0 = xrt::kernel(my_device, xclbin_uuid, "s2mm:{s2mm_1}");
  std::cout << STR_PASSED << "auto out_0 = xrt::kernel(my_device, xclbin_uuid, \"s2mm:{s2mm_1}\")" << std::endl;

  auto out_0_bo = xrt::bo(my_device, bSizeOut, XCL_BO_FLAGS_NONE, out_0.group_id(0));
  std::cout << STR_PASSED << "auto out_0_bo = xrt::bo(my_device, bSizeOut, XCL_BO_FLAGS_NONE, out.group_id(0) (=" << out_0.group_id(0) << "))" << std::endl;

  auto out_0_bo_mapped = out_0_bo.map<TYPE_DATA*>();
  std::cout << STR_PASSED << "auto out_0_bo_mapped = out_0_bo.map<TYPE_DATA*>()" << std::endl;
  memset(out_0_bo_mapped, 0x0, bSizeOut);

  auto out_0_run = out_0(out_0_bo, nullptr, bSizeOut);
  std::cout << STR_PASSED << "auto out_0_run = out(out_0_bo, nullptr, " << bSizeOut << ")" << std::endl;

  // Starting the AIE kernel
  auto my_graph  = xrt::graph(my_device, xclbin_uuid, "filter");
  std::cout << STR_PASSED << "auto my_graph  = xrt::graph(my_device, xclbin_uuid, \"filter\")" << std::endl;

  my_graph.reset();
  std::cout << STR_PASSED << "my_graph.reset()" << std::endl;

  my_graph.run(iterations);
  std::cout << STR_PASSED << "my_graph.run(" << iterations << ")" << std::endl;

  // Waiting for all kernels to end
  std::cout << std::endl << STR_INFO << "Waiting for kernels to end..." << std::endl << std::endl;

  in_0_run.wait();
  std::cout << STR_PASSED << "in_0_run.wait()" << std::endl;

//  my_graph.end();
//  std::cout << STR_PASSED << "my_graph.end()" << std::endl;

  out_0_run.wait();
  std::cout << STR_PASSED << "out_0_run.wait()" << std::endl;

  // Retrieving the results
  out_0_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
  std::cout << STR_PASSED << "out_0_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE)" << std::endl;

  // Verifying the results
  int error_cnt = 0;
  std::cout << std::endl << STR_INFO << "Veryfing output data vs. golden ones " << std::endl << std::endl;
  for(int i = 0; i < (wSizeOut * 2) - 8; i++) {
    if(out_0_bo_mapped[8+i] != golden[i]) {
	    printf("Error found @ %d, %d != %d\n", i, out_0_bo_mapped[i], golden[i]);
	    error_cnt++;
    }
  }

  std::cout << std::endl << ((error_cnt>0) ? STR_FAILED : STR_PASSED) << argv[0] << std::endl << std::endl;

  return ((error_cnt>0) ? EXIT_FAILURE : EXIT_SUCCESS );
}

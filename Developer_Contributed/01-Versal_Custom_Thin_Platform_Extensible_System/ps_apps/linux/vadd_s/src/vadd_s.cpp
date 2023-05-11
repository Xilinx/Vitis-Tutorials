//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <xrt/xrt_device.h>
#include <xrt/xrt_kernel.h>
#include <xrt/xrt_bo.h>

#define  TYPE_DATA uint16_t

static const uint32_t SAMPLES     = 256;

static const char*    STR_ERROR   = "ERROR:   ";
static const char*    STR_FAILED  = "FAILED:  ";
static const char*    STR_PASSED  = "PASSED:  ";
static const char*    STR_INFO    = "INFO:    ";
static const char*    STR_USAGE   = "USAGE:   ";

int main(int argc, char* argv[]) {

  // Check command line
  if(argc < 2) {
    std::cout << STR_USAGE << argv[0] <<" <xclbin> <samples>" << std::endl;
    return EXIT_FAILURE;
  }

  // Calculating samples / bsize
  uint32_t samples = SAMPLES;

  if(argc >= 3) {
    samples = atoi(argv[2]);
  }

  int bsize = samples * sizeof(TYPE_DATA);

  std::cout << STR_INFO << "samples = " << samples << std::endl;
  std::cout << STR_INFO << "bsize   = " << bsize   << std::endl;

  // Loading the xclbin
  char* xclbinFilename = argv[1];

  unsigned int dev_index = 0;
  auto my_device = xrt::device(dev_index);
  std::cout << STR_PASSED << "auto my_device = xrt::device(" << dev_index << ")" << std::endl;

  auto xclbin_uuid = my_device.load_xclbin(xclbinFilename);
  std::cout << STR_PASSED << "auto xclbin_uuid = my_device.load_xclbin(" << xclbinFilename << ")" << std::endl;

  // Starting the input mm2s kernels
  auto in_0 = xrt::kernel(my_device, xclbin_uuid, "mm2s_vadd_s:{mm2s_vadd_s_1}");
  std::cout << STR_PASSED << "auto in_0 = xrt::kernel(my_device, xclbin_uuid, \"mm2s_vadd_s:{mm2s_vadd_s_1}\")" << std::endl;

  auto in_1 = xrt::kernel(my_device, xclbin_uuid, "mm2s_vadd_s:{mm2s_vadd_s_2}");
  std::cout << STR_PASSED << "auto in_1 = xrt::kernel(my_device, xclbin_uuid, \"mm2s_vadd_s:{mm2s_vadd_s_2}\")" << std::endl;

  auto in_0_bo = xrt::bo(my_device, bsize, XCL_BO_FLAGS_NONE, in_0.group_id(0));
  auto in_1_bo = xrt::bo(my_device, bsize, XCL_BO_FLAGS_NONE, in_1.group_id(0));  
  std::cout << STR_PASSED << "auto in_01_bo = xrt::bo(my_device, bsize, XCL_BO_FLAGS_NONE, in_01.group_id(0) (=" << in_0.group_id(0) << "))" << std::endl;

  auto in_0_bo_mapped = in_0_bo.map<TYPE_DATA*>();
  auto in_1_bo_mapped = in_1_bo.map<TYPE_DATA*>();
  std::cout << STR_PASSED << "auto in_01_bo_mapped = = in_01_bo.map<TYPE_DATA*>()" << std::endl;

  //setting input data
  for(uint32_t i = 0 ; i < samples; i++){
    in_0_bo_mapped[i] = 10 + i;
    in_1_bo_mapped[i] = 20 + i;
  }

  in_0_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  in_1_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  std::cout << STR_PASSED << "in_01_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE)" << std::endl;

  auto in_0_run = in_0(in_0_bo, nullptr, samples);
  auto in_1_run = in_1(in_1_bo, nullptr, samples);
  std::cout << STR_PASSED << "auto in_01_run = in_01(in_01_bo, nullptr, " << samples << ")" << std::endl;

  // Starting the output s2mm kernels
  auto out = xrt::kernel(my_device, xclbin_uuid, "s2mm_vadd_s:{s2mm_vadd_s_1}");
  std::cout << STR_PASSED << "auto out = xrt::kernel(my_device, xclbin_uuid, \"s2mm_vadd_s:{s2mm_vadd_s_1}\")" << std::endl;

  auto out_bo = xrt::bo(my_device, bsize, XCL_BO_FLAGS_NONE, out.group_id(0));
  std::cout << STR_PASSED << "auto out_bo = xrt::bo(my_device, bsize, XCL_BO_FLAGS_NONE, out.group_id(0) (=" << out.group_id(0) << "))" << std::endl;

  auto out_bo_mapped = out_bo.map<TYPE_DATA*>();
  std::cout << STR_PASSED << "auto out_bo_mapped = out_bo.map<TYPE_DATA*>()" << std::endl;

  auto out_run = out(out_bo, nullptr, samples);
  std::cout << STR_PASSED << "auto out_run = out(out_bo, nullptr, " << samples << ")" << std::endl;

  // Starting the DUT kernel
  auto dut = xrt::kernel(my_device, xclbin_uuid, "vadd_s:{vadd_s_1}");
  std::cout << STR_PASSED << "dut = xrt::kernel(my_device, xclbin_uuid, \"vadd_s:{vadd_s_1}\")" << std::endl;

  auto dut_run = dut(samples, nullptr, nullptr, nullptr);
  std::cout << STR_PASSED << "dut_run = dut(" << samples << ", nullptr, nullptr, nullptr)" << std::endl;

  // Waiting for all kernels to end
  std::cout << std::endl << STR_INFO << "Waiting for kernels to end..." << std::endl << std::endl;

  in_0_run.wait();
  std::cout << STR_PASSED << "in_0_run.wait()" << std::endl;

  in_1_run.wait();
  std::cout << STR_PASSED << "in_1_run.wait()" << std::endl;

  dut_run.wait();
  std::cout << STR_PASSED << "dut_run.wait()" << std::endl;

  out_run.wait();
  std::cout << STR_PASSED << "out_run.wait()" << std::endl;

  // Retrieving the results
  out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
  std::cout << STR_PASSED << "out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE)" << std::endl;

  // Verifying the results
  int error_cnt = 0;

  for(uint32_t i = 0; i < samples; i++) {
    TYPE_DATA golden = in_0_bo_mapped[i] + in_1_bo_mapped[i];
    if(out_bo_mapped[i] != golden) {
      std::cout << STR_ERROR << i << "; Expected Result: " << golden << "; Kernel Result: " << out_bo_mapped[i] << std::endl;
      ++error_cnt;
    }
  }

  std::cout << std::endl << (error_cnt ? STR_FAILED : STR_PASSED) << argv[0] << std::endl << std::endl;

  return (error_cnt ? EXIT_SUCCESS : EXIT_FAILURE);
}


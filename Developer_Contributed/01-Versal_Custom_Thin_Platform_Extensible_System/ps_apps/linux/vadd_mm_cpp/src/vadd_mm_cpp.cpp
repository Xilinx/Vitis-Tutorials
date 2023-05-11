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

static const int VADD_WORD_SIZE   = 4096;
static const int VADD_BYTE_SIZE   = VADD_WORD_SIZE * sizeof(unsigned int);

static const char*    STR_ERROR   = "ERROR:   ";
static const char*    STR_FAILED  = "FAILED:  ";
static const char*    STR_PASSED  = "PASSED:  ";
static const char*    STR_INFO    = "INFO:    ";
static const char*    STR_USAGE   = "USAGE:   ";

int main(int argc, char* argv[]) {

  //TARGET_DEVICE macro needs to be passed from gcc command line
  if(argc != 2) {
    std::cout << STR_USAGE << argv[0] <<" <xclbin>" << std::endl;
    return EXIT_FAILURE;
  }

  char* xclbinFilename = argv[1];

  unsigned int dev_index = 0;
  auto my_device = xrt::device(dev_index);
  std::cout << STR_PASSED << "auto my_device = xrt::device(" << dev_index << ")" << std::endl;

  auto xclbin_uuid = my_device.load_xclbin(xclbinFilename);
  std::cout << STR_PASSED << "auto xclbin_uuid = my_device.load_xclbin(" << xclbinFilename << ")" << std::endl;

  auto my_vadd = xrt::kernel(my_device, xclbin_uuid, "vadd_mm:{vadd_mm_1}");
  std::cout << STR_PASSED << "auto my_vadd = xrt::kernel(my_device, xclbin_uuid, \"vadd_mm:{vadd_mm_1}\")" << std::endl;

  auto my_vadd_input0 = xrt::bo(my_device, VADD_BYTE_SIZE, XCL_BO_FLAGS_NONE, my_vadd.group_id(0));
  auto my_vadd_input1 = xrt::bo(my_device, VADD_BYTE_SIZE, XCL_BO_FLAGS_NONE, my_vadd.group_id(1));  
  auto my_vadd_output = xrt::bo(my_device, VADD_BYTE_SIZE, XCL_BO_FLAGS_NONE, my_vadd.group_id(2));
  std::cout << STR_PASSED << "auto my_vadd_i/oX = xrt::bo(my_device, VADD_BYTE_SIZE, XCL_BO_FLAGS_NONE, my_vadd.group_id(X) (=" << my_vadd.group_id(0) << "))" << std::endl;

  auto my_vadd_input0_mapped = my_vadd_input0.map<unsigned int*>();
  auto my_vadd_input1_mapped = my_vadd_input1.map<unsigned int*>();
  auto my_vadd_output_mapped = my_vadd_output.map<unsigned int*>();
 std::cout << STR_PASSED << "auto my_vadd_i/oX_mapped = = my_vadd_i/oX.map<unsigned int*>()" << std::endl;

  //setting input data
  for(int i = 0 ; i < VADD_WORD_SIZE; i++){
    my_vadd_input0_mapped[i] = 10 + i;
    my_vadd_input1_mapped[i] = 20 + i;
  }

  my_vadd_input0.sync(XCL_BO_SYNC_BO_TO_DEVICE, VADD_BYTE_SIZE, 0);
  my_vadd_input1.sync(XCL_BO_SYNC_BO_TO_DEVICE, VADD_BYTE_SIZE, 0);
  std::cout << STR_PASSED << "my_vadd_inputX.sync(XCL_BO_SYNC_BO_TO_DEVICE, VADD_BYTE_SIZE, 0)" << std::endl;

  auto my_vadd_run = my_vadd(my_vadd_input0, my_vadd_input1, my_vadd_output, VADD_BYTE_SIZE);
  std::cout << STR_PASSED << "auto my_vadd_run = my_vadd(my_vadd_input0, my_vadd_input1, my_vadd_output, VADD_BYTE_SIZE)" << std::endl;

  // Waiting for all kernels to end
  std::cout << std::endl << STR_INFO << "Waiting kernel to end..." << std::endl << std::endl;

  my_vadd_run.wait();

  my_vadd_output.sync(XCL_BO_SYNC_BO_FROM_DEVICE, VADD_BYTE_SIZE, 0);
  std::cout << STR_PASSED << "my_vadd_output.sync(XCL_BO_SYNC_BO_FROM_DEVICE, VADD_BYTE_SIZE, 0)" << std::endl;

  int match = 1;
  for(int i = 0; i < VADD_WORD_SIZE; i++) {
    unsigned int result = my_vadd_input0_mapped[i] + my_vadd_input1_mapped[i];
    if(my_vadd_output_mapped[i] != result) {
      std::cout << STR_ERROR << "[" << i << "]; Expected Result: " << result << "; Kernel Result: " << my_vadd_output_mapped[i] << std::endl;
      match = 0;
    }
  }

  std::cout << std::endl << (match ? STR_PASSED : STR_FAILED) << argv[0] << std::endl << std::endl;

  return (match ? EXIT_FAILURE : EXIT_SUCCESS);
}

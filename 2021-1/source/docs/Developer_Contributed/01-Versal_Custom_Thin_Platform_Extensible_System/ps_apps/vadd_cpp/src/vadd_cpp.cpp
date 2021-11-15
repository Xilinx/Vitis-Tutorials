// /**********
// © Copyright 2020 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// **********/

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <xrt/xrt_device.h>
#include <xrt/xrt_kernel.h>
#include <xrt/xrt_bo.h>

static const int VADD_WORD_SIZE = 4096;
static const int VADD_BYTE_SIZE = VADD_WORD_SIZE * sizeof(unsigned int);

int main(int argc, char* argv[]) {

  //TARGET_DEVICE macro needs to be passed from gcc command line
  if(argc != 2) {
    std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
    return EXIT_FAILURE;
  }

  char* xclbinFilename = argv[1];

  unsigned int dev_index = 0;
  auto my_device = xrt::device(dev_index);
  std::cout << "Passed: auto my_device = xrt::device(" << dev_index << ")" << std::endl;

  auto xclbin_uuid = my_device.load_xclbin(xclbinFilename);
  std::cout << "Passed: auto xclbin_uuid = my_device.load_xclbin(" << xclbinFilename << ")" << std::endl;

  auto my_vadd = xrt::kernel(my_device, xclbin_uuid, "vadd");
  std::cout << "Passed: auto my_vadd = xrt::kernel(my_device, xclbin_uuid, \"vadd\")" << std::endl;

  auto my_vadd_arg0 = my_vadd.group_id(0);
  auto my_vadd_arg1 = my_vadd.group_id(1);
  auto my_vadd_arg2 = my_vadd.group_id(2);

  auto my_vadd_input0 = xrt::bo(my_device, VADD_BYTE_SIZE, XCL_BO_FLAGS_NONE, my_vadd_arg0);
  auto my_vadd_input1 = xrt::bo(my_device, VADD_BYTE_SIZE, XCL_BO_FLAGS_NONE, my_vadd_arg1);  
  auto my_vadd_output = xrt::bo(my_device, VADD_BYTE_SIZE, XCL_BO_FLAGS_NONE, my_vadd_arg2);

  auto my_vadd_input0_mapped = my_vadd_input0.map<unsigned int*>();
  auto my_vadd_input1_mapped = my_vadd_input1.map<unsigned int*>();
  auto my_vadd_output_mapped = my_vadd_output.map<unsigned int*>();

  //setting input data
  for(int i = 0 ; i < VADD_WORD_SIZE; i++){
    my_vadd_input0_mapped[i] = 10 + i;
    my_vadd_input1_mapped[i] = 20 + i;
  }

  my_vadd_input0.sync(XCL_BO_SYNC_BO_TO_DEVICE, VADD_BYTE_SIZE, 0);
  my_vadd_input1.sync(XCL_BO_SYNC_BO_TO_DEVICE, VADD_BYTE_SIZE, 0);

  auto my_vadd_run = my_vadd(my_vadd_input0, my_vadd_input1, my_vadd_output, VADD_BYTE_SIZE);

  my_vadd_run.wait();

  my_vadd_output.sync(XCL_BO_SYNC_BO_FROM_DEVICE, VADD_BYTE_SIZE, 0);

  int match = 1;
  for(int i = 0; i < VADD_WORD_SIZE; i++) {
    unsigned int result = my_vadd_input0_mapped[i] + my_vadd_input1_mapped[i];
    if(my_vadd_output_mapped[i] != result) {
      std::cout << "VADD ERROR: " << i << "; Expected Result: " << result << "; Kernel Result: " << my_vadd_output_mapped[i] << std::endl;
      match = 0;
    }
  }

  std::cout << "VADD TEST " << (match ? "PASSED" : "FAILED") << std::endl;

  return (match ? EXIT_FAILURE : EXIT_SUCCESS);
}

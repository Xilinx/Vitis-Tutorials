//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

#include "datamove_app.cpp"
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <xrt/xrt_device.h>
#include <xrt/xrt_kernel.h>
#include <experimental/xrt_aie.h>
#include <experimental/xrt_graph.h>
#include <experimental/xrt_ip.h>

static const char*    STR_PASSED = "PASSED:  ";
static const char*    STR_USAGE  = "USAGE:   ";

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

  auto my_rtl_ip = xrt::ip(my_device, xclbin_uuid, "subtractor:{subtractor_0}");
  std::cout << STR_PASSED << "auto my_rtl_ip = xrt::ip(my_device, xclbin_uuid, \"subtractor:{subtractor_0}\")" << std::endl;

  auto my_graph  = xrt::graph(my_device, xclbin_uuid, "mygraph_top");
  std::cout << STR_PASSED << "auto my_graph  = xrt::graph(my_device, xclbin_uuid, \"mygraph_top\")" << std::endl;

  my_graph.reset();
  std::cout << STR_PASSED << "my_graph.reset()" << std::endl;

  my_graph.run(0);
  std::cout << STR_PASSED << "my_graph.run()" << std::endl;

  for (int i = 0; i < 20; i++) {
    sleep(1);
    std::cout << "Poll subtractor register" << std::endl;
    std::cout << "  Value Reg0:  " << std::hex << ( my_rtl_ip.read_register(0x00) ) << std::endl;
    std::cout << "  Value Reg1:  " << std::hex << ( my_rtl_ip.read_register(0x04) ) << std::endl;
    std::cout << "  Value Reg2:  " << std::hex << ( my_rtl_ip.read_register(0x08) ) << std::endl;
    std::cout << "  Value Reg3:  " << std::hex << ( my_rtl_ip.read_register(0x0c) ) << std::endl;
  }

  my_graph.end(1000);
  std::cout << STR_PASSED << "my_graph.end()" << std::endl;
  std::cout << std::endl << STR_PASSED << argv[0] << std::endl << std::endl;
}

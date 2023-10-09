//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <iostream>
#include "pfa1008_graph.h"

// Instantiate AIE graph:
pfa1008_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  std::cout << "*** pfa1008_app ***" << std::endl;

  aie_dut.init();
  aie_dut.run(8);
  aie_dut.end();

  return 0;
}


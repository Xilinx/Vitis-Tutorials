//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "ifft4096_graph.h"



// Instantiate AIE graph:
ifft4096_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(8); // One transform per graph iteration
  aie_dut.end();

  return 0;
}

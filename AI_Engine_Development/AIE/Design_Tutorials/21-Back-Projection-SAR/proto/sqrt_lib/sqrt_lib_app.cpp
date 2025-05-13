//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "sqrt_lib_graph.h"

// Instantiate AIE graph:
sqrt_lib_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(512); // Need 512 graph iterations to compute 1 SAR image per radar pulse
  aie_dut.end();

  return 0;
}

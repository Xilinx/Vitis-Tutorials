//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "sortcpp_fp16_graph.h"

// Instantiate AIE graph:
sortcpp_fp16_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(24);
  aie_dut.end();

  return 0;
}

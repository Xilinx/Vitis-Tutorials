//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "m16_ssr8_graph.h"

// Instantiate AIE graph:
m16_ssr8_graph aie_dut;

// Initialize and run the graph:
int main(int argc, char **argv)
{
  aie_dut.init();
  aie_dut.run(2);
  aie_dut.end();

  return 0;
}

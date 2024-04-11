/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "all2_graph.h"

// Instantiate AIE graph:
all2_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(4); // Run this many graph iterations
  aie_dut.end();

  return 0;
}

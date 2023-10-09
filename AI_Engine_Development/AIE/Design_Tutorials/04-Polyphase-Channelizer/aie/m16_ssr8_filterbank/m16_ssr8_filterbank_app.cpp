//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "m16_ssr8_filterbank_only_graph.h"

// Instantiate AIE graph:
m16_ssr8_filterbank_only_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(2); // Need 2 graph iterations since Nsamp_i/NSAMP = 16K/8192 = 2
  aie_dut.end();

  return 0;
}

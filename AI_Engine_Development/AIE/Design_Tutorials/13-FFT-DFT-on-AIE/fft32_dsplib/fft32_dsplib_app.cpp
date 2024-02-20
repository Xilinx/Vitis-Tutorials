//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <iostream>
#include "fft32_dsplib_graph.h"

// Instantiate AIE graph:
typedef fft32_dsplib_graph<18,0> TT_GRAPH;
TT_GRAPH aie_dut;

// Initialize and run the graph:
int main(void)
{
  std::cout << "*** my_app *** Nports: "            << TT_GRAPH::Nports << std::endl;
  std::cout << "*** my_app *** TP_PARALLEL_POWER: " << TT_GRAPH::TP_PARALLEL_POWER << std::endl;

  aie_dut.init();
  aie_dut.run(512/TT_GRAPH::REPEAT);
  aie_dut.end();

  return 0;
}


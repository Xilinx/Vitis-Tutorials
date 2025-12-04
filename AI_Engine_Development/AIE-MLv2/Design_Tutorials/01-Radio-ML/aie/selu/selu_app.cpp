//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <fstream>
#include <iostream>
#include "selu_graph.h"

class dut_graph : public graph {
public:
  static constexpr unsigned NSAMP = 1024;
  selu_graph<NSAMP>   dut;
  input_plio          sig_i;
  output_plio         sig_o;

  dut_graph( void )
  {
    sig_i =  input_plio::create("PLIO_i_0",plio_64_bits,"data/sig_i.txt");
    sig_o = output_plio::create("PLIO_o_0",plio_64_bits,"data/sig_o.txt");
    connect<>(sig_i.out[0],dut.sig_i);
    connect<>(dut.sig_o,sig_o.in[0]);
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(8);
  aie_dut.end();

  return 0;
}

//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "dft16_graph.h"

using namespace adf;

template<int X, int Y>
class dut_graph : public graph {
  dft16_graph dft16;
public:
  input_plio  sig_i;
  output_plio sig_o;
  dut_graph( void )
  {
    sig_i =  input_plio::create("PLIO_i",plio_64_bits,"data/sig_i.txt");
    sig_o = output_plio::create("PLIO_o",plio_64_bits,"data/sig_o.txt");
    connect<stream>(sig_i.out[0],dft16.sig_i);
    connect<stream>(dft16.sig_o,sig_o.in[0]);
    location<kernel>(dft16.kk0) = tile(X+3,Y+0);
    location<kernel>(dft16.kk1) = tile(X+4,Y+0);
  }
};

// Instantiate AIE graph:
dut_graph<18,0> aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(8);
  aie_dut.end();

  return 0;
}

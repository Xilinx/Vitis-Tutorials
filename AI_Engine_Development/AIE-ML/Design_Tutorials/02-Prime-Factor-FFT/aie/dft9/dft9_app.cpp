//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "dft9_graph.h"

using namespace adf;

template<unsigned X, unsigned Y>
class dut_graph : public graph {
  dft9_graph dft9;
public:
  input_plio  sig_i;
  output_plio sig_o;
  dut_graph( void )
  {
    sig_i =  input_plio::create("PLIO_i",plio_64_bits,"data/sig_i.txt");
    sig_o = output_plio::create("PLIO_o",plio_64_bits,"data/sig_o.txt");
    connect<stream>(sig_i.out[0],dft9.sig_i);
    connect<stream>(dft9.sig_o,sig_o.in[0]);
    location<kernel>(dft9.kk0) = tile(X+0,Y+1);
    location<kernel>(dft9.kk1) = tile(X+1,Y+1);
    location<kernel>(dft9.kk2) = tile(X+2,Y+1);
    location<kernel>(dft9.kk3) = tile(X+3,Y+1);
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

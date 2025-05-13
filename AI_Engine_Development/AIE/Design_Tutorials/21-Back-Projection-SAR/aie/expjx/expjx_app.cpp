//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "expjx_graph.h"

class dut_graph : public graph {
public:
  input_plio            sig_i;
  output_plio           sig_o;
  expjx_graph<256,1024> dut;
  dut_graph(void) {
    sig_i  =  input_plio::create("PLIO_sig_i",  plio_64_bits, "data/sig_i.txt" );
    sig_o  = output_plio::create("PLIO_sig_o",  plio_64_bits, "data/sig_o.txt" );
    connect<>(sig_i.out[0], dut.sig_i);
    connect<>(dut.sig_o,    sig_o.in[0]);
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(1); // Run one radar pulse
  aie_dut.end();

  return 0;
}

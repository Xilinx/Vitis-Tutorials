//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "diff3dsq_graph.h"

class dut_graph : public graph {
public:
  diff3dsq_graph<256,1024> dut;
  input_plio               sig_i;
  output_plio              sig_o;
  input_port               coord_i;
  dut_graph(void) {
    sig_i =  input_plio::create("PLIO_i", plio_64_bits, "data/sig_i.txt" );
    sig_o = output_plio::create("PLIO_o", plio_64_bits, "data/sig_o.txt" );
    connect<>(sig_i.out[0], dut.sig_i);
    connect<>(dut.sig_o, sig_o.in[0]);
    connect<>(coord_i,dut.coord_i);
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
#include "antenna_coord.h"
  aie_dut.init();
  aie_dut.update(aie_dut.coord_i, aperture, diff3dsq<256,1024>::NCOORD); // Set aperture coordinates
  aie_dut.run(4);                                                        // Run for 4 radar pulses
  aie_dut.end();

  return 0;
}

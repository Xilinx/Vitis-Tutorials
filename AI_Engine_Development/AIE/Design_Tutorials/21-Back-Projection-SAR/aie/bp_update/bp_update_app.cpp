//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "bp_update_graph.h"

class dut_graph : public graph {
public:
  input_plio                phase_i;
  input_plio                image_i;
  input_plio                interp_real_i;
  input_plio                interp_imag_i;
  output_plio               image_o;
  bp_update_graph<256,1024> dut;
  dut_graph(void) {
    image_i       =  input_plio::create("PLIO_image_i",      plio_64_bits, "data/image_i.txt");
    phase_i       =  input_plio::create("PLIO_phase_i",      plio_64_bits, "data/phase_i.txt");
    interp_real_i =  input_plio::create("PLIO_interp_real_i",plio_64_bits, "data/interp_real_i.txt");
    interp_imag_i =  input_plio::create("PLIO_interp_imag_i",plio_64_bits, "data/interp_imag_i.txt");
    image_o       = output_plio::create("PLIO_image_o",      plio_64_bits, "data/image_o.txt");
    connect<>(interp_real_i.out[0], dut.interp_real_i);
    connect<>(interp_imag_i.out[0], dut.interp_imag_i);
    connect<>(phase_i.out[0],       dut.phase_i);
    connect<>(image_i.out[0],       dut.image_i);
    connect<>(dut.image_o,          image_o.in[0] );
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(1); // Run a single radar pulse
  aie_dut.end();

  return 0;
}

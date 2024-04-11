//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "fft32_r2_graph.h"

class dut_graph : public graph {
public:
  input_plio  sig_i;
  output_plio sig_o;
  fft32_r2_graph dut;
  dut_graph( void )
  {
    sig_i =  input_plio::create("PLIO_i", plio_64_bits, "data/sig0_i.txt" );
    sig_o = output_plio::create("PLIO_o", plio_64_bits, "data/sig0_o.txt" );
    connect( sig_i.out[0], dut.sig_i   );
    connect( dut.sig_o,    sig_o.in[0] );
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(512/fft32_r2_kernel::REPEAT);
  aie_dut.end();

  return 0;
}

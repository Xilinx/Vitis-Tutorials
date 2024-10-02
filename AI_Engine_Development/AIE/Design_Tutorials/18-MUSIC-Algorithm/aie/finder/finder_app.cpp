//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "finder_graph.h"

class dut_graph : public graph {
public:
  finder_graph dut;
  input_plio   sig_i;
  output_plio  sig_o;

  dut_graph( void )
  {
    sig_i =  input_plio::create("PLIO_i", plio_64_bits, "data/sig_i.txt" );
    sig_o = output_plio::create("PLIO_o", plio_64_bits, "data/sig_o.txt" );
    connect<>( sig_i.out[0], dut.sig_i );
    connect<>( dut.sig_o, sig_o.in[0] );

    // Placements: ROW_0, column: 12, 13, ..., 27, for a total of 16 tiles
    static constexpr unsigned COL_START = 12;
    for (unsigned i = 0, col = COL_START; i < NUM_FINDER_KERNELS; ++i, ++col) {
      adf::location<adf::kernel>(dut.finder_kernel[i]) = adf::tile(col, ROW_0);
      adf::location<adf::stack>(dut.finder_kernel[i]) = adf::location<adf::kernel>(dut.finder_kernel[i]);
    }
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(4);
  aie_dut.end();

  return 0;
}

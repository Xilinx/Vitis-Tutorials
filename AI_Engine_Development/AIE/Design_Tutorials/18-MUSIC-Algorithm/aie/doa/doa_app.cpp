//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "doa_graph.h"

class dut_graph : public graph {
public:
  doa_graph   dut;
  input_plio  sig_i;
  output_plio sig_o;

  dut_graph( void )
  {
    sig_i =  input_plio::create("PLIO_i", plio_64_bits, "data/sig_i.txt" );
    sig_o = output_plio::create("PLIO_o", plio_64_bits, "data/sig_o.txt" );
    connect<>( sig_i.out[0], dut.sig_i );
    connect<>( dut.sig_o, sig_o.in[0] );

    // Placements: ROW_0, column: 12, 13, ..., 42, for a total of 31 tiles
    //             ROW_1, column: 42, 41, ..., 11, for a total of 32 tiles
    //             ROW_0, column: 11 for 1 tile
    static constexpr unsigned COL_START = 12;
    for (unsigned i = 0, col = COL_START; i < 31; ++i, ++col) {
      adf::location<adf::kernel>(dut.doa_kernel[i]) = adf::tile(col, ROW_0);
      adf::location<adf::stack >(dut.doa_kernel[i]) = adf::location<adf::kernel>(dut.doa_kernel[i]);
    }
    for (unsigned i = 31, col = 12 + 31 - 1; i < 31 + 32; ++i, --col) {
      adf::location<adf::kernel>(dut.doa_kernel[i]) = adf::tile(col, ROW_1);
      adf::location<adf::stack >(dut.doa_kernel[i]) = adf::location<adf::kernel>(dut.doa_kernel[i]);
    }
    adf::location<adf::kernel>(dut.doa_kernel[NUM_DOA_KERNELS-1]) = adf::tile(COL_START-1, ROW_0);
    adf::location<adf::stack >(dut.doa_kernel[NUM_DOA_KERNELS-1]) = adf::location<adf::kernel>(dut.doa_kernel[NUM_DOA_KERNELS-1]);
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

//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "qrd_graph.h"

class dut_graph : public graph {
public:
  qrd_graph   qrd;
  input_plio  sig_i;
  output_plio sig_o;

  dut_graph( void )
  {
    sig_i =  input_plio::create("PLIO_i", plio_64_bits, "data/sig_i.txt" );
    sig_o = output_plio::create("PLIO_o", plio_64_bits, "data/sig_o.txt" );
    connect<>( sig_i.out[0], qrd.sig_i );
    connect<>( qrd.sig_o, sig_o.in[0] );

    // Placement:
    static constexpr unsigned COL_START = 12;
    for (unsigned i = 0, col = COL_START; i < qrd_graph::NUM_QRD_KERNELS; ++i, ++col) {
      adf::location<adf::kernel>(qrd.qrd_kernel[i]) = adf::tile(col, ROW_0);
      // adf::location<adf::stack >(qrd.qrd_kernel[i]) = adf::location<adf::kernel>(qrd.qrd_kernel[i]);
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

//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <iostream>
#include "sincos_test_graph.h"

using TT_DUT = sincos_test_graph;

// Dummy graph with PLIO:
class dut_graph : public graph {
public:
  TT_DUT dut;
  output_plio phase_o;
  output_plio twid_o;
  dut_graph(void)
  {
    std::string fname_o = "data/sig_o_0.txt";
    std::string pname_o = "PLIO_o_0";
    phase_o = output_plio::create("PLIO_phase",plio_64_bits,"data/phase_o.txt");
    twid_o  = output_plio::create("PLIO_twid", plio_64_bits,"data/twid_o.txt");
    connect( dut.phase_o,    phase_o.in[0] );
    connect( dut.twid_o,     twid_o.in[0] );
    runtime<ratio>(dut.k_kernel) = 0.9;
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(1);
  aie_dut.end();

  return 0;
}


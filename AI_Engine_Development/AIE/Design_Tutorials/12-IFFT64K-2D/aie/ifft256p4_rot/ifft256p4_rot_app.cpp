//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <iostream>
#include "ifft256p4_rot_graph.h"

using TT_DUT = ifft256p4_rot_graph<5,4>;

// Dummy graph with PLIO:
class dut_graph : public graph {
public:
  TT_DUT dut;
  input_plio  sig_i;
  output_plio sig_o;
  dut_graph(void)
  {
    std::string fname_i = "data/sig_i_0.txt";
    std::string fname_o = "data/sig_o_0.txt";
    std::string pname_i = "PLIO_i_0";
    std::string pname_o = "PLIO_o_0";
    sig_i =  input_plio::create(pname_i,plio_64_bits,fname_i);
    sig_o = output_plio::create(pname_o,plio_64_bits,fname_o);
    connect( sig_i.out[0],  dut.sig_i   );
    connect( dut.sig_o,     sig_o.in[0] );
    runtime<ratio>(dut.k_kernel) = 0.9;
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(60);
  aie_dut.end();

  return 0;
}


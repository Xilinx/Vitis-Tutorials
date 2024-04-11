//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "farrow_graph.h"

using namespace adf;

class dut_graph : public graph {
  farrow_graph farrow;
public:
  std::array< input_plio,1> sig_i;
  std::array< input_plio,1> del_i;
  std::array<output_plio,1> sig_o;
  dut_graph( void )
  {
#ifdef AIE_SIM_ONLY
    sig_i[0] =  input_plio::create("PLIO_i_0",plio_64_bits,"data/sig_i.txt");
    del_i[0] =  input_plio::create("PLIO_i_1",plio_64_bits,"data/del_i.txt");
    sig_o[0] = output_plio::create("PLIO_o_0",plio_64_bits,"data/sig_o.txt");
#else
    sig_i[0] =  input_plio::create("PLIO_i_0",plio_64_bits);
    del_i[0] =  input_plio::create("PLIO_i_1",plio_64_bits);
    sig_o[0] = output_plio::create("PLIO_o_0",plio_64_bits);
#endif
    connect<stream>(sig_i[0].out[0],farrow.sig_i[0]);
    connect<stream>(del_i[0].out[0],farrow.del_i[0]);
    connect<stream>(farrow.sig_o[0],sig_o[0].in[0]);
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

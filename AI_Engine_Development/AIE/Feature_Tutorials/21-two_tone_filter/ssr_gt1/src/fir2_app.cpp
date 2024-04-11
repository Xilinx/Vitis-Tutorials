/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "fir2_graph.h"

// Instantiate AIE graph:
class dut_graph : public graph {
public:
  std::array<input_plio, fir2::NPORTS_O> filt_i;
  std::array<output_plio,fir2::NPORTS_O> filt_o;

  fir2_graph<15,0> fir_dut;

  dut_graph(void) {
    for (int ii=0; ii < fir2::NPORTS_O; ii++) {
      std::string pname_i = "PLIO_fir_i_"+std::to_string(ii);
      std::string pname_o = "PLIO_fir_o_"+std::to_string(ii);
      std::string fname_i = "data/sig_i_"+std::to_string(ii)+".txt";
      std::string fname_o = "data/fir_o_"+std::to_string(ii)+".txt";
      filt_i[ii] =  input_plio::create(pname_i, plio_64_bits, fname_i);
      filt_o[ii] = output_plio::create(pname_o, plio_64_bits, fname_o);
      connect<stream>( filt_i[ii].out[0],  fir_dut.filt_i[ii] );
      connect<stream>( fir_dut.filt_o[ii], filt_o[ii].in[0]  );
    }
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(4); // Run this many graph iterations
  aie_dut.end();

  return 0;
}

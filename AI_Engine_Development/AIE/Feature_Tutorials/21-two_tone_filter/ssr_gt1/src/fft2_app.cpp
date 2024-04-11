/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "fft2_graph.h"

// Instantiate AIE graph:
class dut_graph : public graph {
public:
  std::array<input_plio, fft2::NPORTS_O> fft_i;
  std::array<output_plio,fft2::NPORTS_O> fft_o;

  fft2_graph<24,0> fft_dut;

  dut_graph(void) {
    for (int ii=0; ii < fft2::NPORTS_O; ii++) {
      std::string pname_i = "PLIO_fft_i_"+std::to_string(ii);
      std::string pname_o = "PLIO_fft_o_"+std::to_string(ii);
      std::string fname_i = "data/sig_i_"+std::to_string(ii)+".txt";
      std::string fname_o = "data/fft_o_"+std::to_string(ii)+".txt";
      fft_i[ii] =  input_plio::create(pname_i, plio_64_bits, fname_i);
      fft_o[ii] = output_plio::create(pname_o, plio_64_bits, fname_o);
      connect<stream>( fft_i[ii].out[0],  fft_dut.fft_i[ii] );
      connect<stream>( fft_dut.fft_o[ii], fft_o[ii].in[0]  );
    }
  }
};

dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(4); // Run this many graph iterations
  aie_dut.end();

  return 0;
}

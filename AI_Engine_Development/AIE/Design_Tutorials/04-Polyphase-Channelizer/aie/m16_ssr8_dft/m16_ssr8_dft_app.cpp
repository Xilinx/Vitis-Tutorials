//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "m16_ssr8_dft_graph.h"


class my_graph : public graph {
public:
  static constexpr unsigned DFT_X = 16;
  static constexpr unsigned DFT_Y = 0;
  std::array<input_plio,8>  dft_i;
  std::array<output_plio,8> dft_o;
  m16_ssr8_dft_graph dut;
  // Constructor:
  my_graph(void) {
    for (int ii=0; ii < 8; ii++) {
      std::string plio_i = "PLIO_dft_i_"+std::to_string(ii);
      std::string plio_o = "PLIO_dft_o_"+std::to_string(ii);
      std::string file_i = "data/sig"+std::to_string(ii)+"_i.txt";
      std::string file_o = "data/sig"+std::to_string(ii)+"_o.txt";
      dft_i[ii] =  input_plio::create(plio_i,plio_64_bits,file_i);
      dft_o[ii] = output_plio::create(plio_o,plio_64_bits,file_o);
      connect<stream>(dft_i[ii].out[0], dut.sig_i[ii]);
      connect<stream>(dut.sig_o[ii],    dft_o[ii].in[0]);
    }
    location<graph>(dut.dut_graphA) = area_group( {{aie_tile,DFT_X,DFT_Y+0,DFT_X+3,DFT_Y+0},{shim_tile,DFT_X,0,DFT_X+3,0}} );
    location<graph>(dut.dut_graphB) = area_group( {{aie_tile,DFT_X,DFT_Y+1,DFT_X+3,DFT_Y+1},{shim_tile,DFT_X,0,DFT_X+3,0}} );
    location<graph>(dut.dut_graphC) = area_group( {{aie_tile,DFT_X,DFT_Y+2,DFT_X+3,DFT_Y+2},{shim_tile,DFT_X,0,DFT_X+3,0}} );
    location<graph>(dut.dut_graphD) = area_group( {{aie_tile,DFT_X,DFT_Y+3,DFT_X+3,DFT_Y+3},{shim_tile,DFT_X,0,DFT_X+3,0}} );

    location<PLIO>(dft_i[0]) = shim(DFT_X+1);    location<PLIO>(dft_o[0]) = shim(DFT_X+3);
    location<PLIO>(dft_i[1]) = shim(DFT_X+1);    location<PLIO>(dft_o[1]) = shim(DFT_X+3);
    location<PLIO>(dft_i[2]) = shim(DFT_X+1);    location<PLIO>(dft_o[2]) = shim(DFT_X+0);
    location<PLIO>(dft_i[3]) = shim(DFT_X+1);    location<PLIO>(dft_o[3]) = shim(DFT_X+0);
    location<PLIO>(dft_i[4]) = shim(DFT_X+2);    location<PLIO>(dft_o[4]) = shim(DFT_X+3);
    location<PLIO>(dft_i[5]) = shim(DFT_X+2);    location<PLIO>(dft_o[5]) = shim(DFT_X+3);
    location<PLIO>(dft_i[6]) = shim(DFT_X+2);    location<PLIO>(dft_o[6]) = shim(DFT_X+0);
    location<PLIO>(dft_i[7]) = shim(DFT_X+2);    location<PLIO>(dft_o[7]) = shim(DFT_X+0);
  }
};


// Instantiate AIE graph:
my_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(2); // Need 2 graph iterations since Nsamp_i/NSAMP = 16K/8192 = 2
  aie_dut.end();

  return 0;
}

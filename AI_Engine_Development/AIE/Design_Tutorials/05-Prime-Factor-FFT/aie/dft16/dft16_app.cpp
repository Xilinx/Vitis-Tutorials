//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "dft16_graph.h"

using namespace adf;

template<int DFT16_X, int DFT16_Y>
class dut_graph : public graph {
  dft16_graph dft16;
public:
  std::array< input_plio,2> sig_i;
  std::array<output_plio,2> sig_o;
  dut_graph( void )
  {
    sig_i[0] =  input_plio::create("PLIO_i_0",plio_64_bits,"data/sig_i_0.txt");
    sig_i[1] =  input_plio::create("PLIO_i_1",plio_64_bits,"data/sig_i_1.txt");
    sig_o[0] = output_plio::create("PLIO_o_0",plio_64_bits,"data/sig_o_0.txt");
    sig_o[1] = output_plio::create("PLIO_o_1",plio_64_bits,"data/sig_o_1.txt");
    connect<stream>(sig_i[0].out[0],dft16.sig_i[0]);
    connect<stream>(sig_i[1].out[0],dft16.sig_i[1]);
    connect<stream>(dft16.sig_o[0],sig_o[0].in[0]);
    connect<stream>(dft16.sig_o[1],sig_o[1].in[0]);

    location<kernel>(dft16.k_tile0) = tile(DFT16_X+1,DFT16_Y+0);
    location<kernel>(dft16.k_tile1) = tile(DFT16_X+2,DFT16_Y+1);
    location<kernel>(dft16.k_tile2) = tile(DFT16_X+1,DFT16_Y+2);
    location<kernel>(dft16.k_tile3) = tile(DFT16_X+0,DFT16_Y+1);
    location<kernel>(dft16.k_tile4) = tile(DFT16_X+1,DFT16_Y+1);

    location<stack>(dft16.k_tile0) = bank(DFT16_X+1,DFT16_Y+0,0);
    location<stack>(dft16.k_tile1) = bank(DFT16_X+2,DFT16_Y+0,0);
    location<stack>(dft16.k_tile2) = bank(DFT16_X+1,DFT16_Y+2,3);
    location<stack>(dft16.k_tile3) = bank(DFT16_X+0,DFT16_Y+1,0);
    location<stack>(dft16.k_tile4) = bank(DFT16_X+1,DFT16_Y+2,0);

    location<parameter>(dft16.k_tile0.param[0]) = bank(DFT16_X+1,DFT16_Y+0,1);
    location<parameter>(dft16.k_tile0.param[1]) = bank(DFT16_X+1,DFT16_Y+0,2);
    location<parameter>(dft16.k_tile1.param[0]) = bank(DFT16_X+2,DFT16_Y+1,2);
    location<parameter>(dft16.k_tile1.param[1]) = bank(DFT16_X+2,DFT16_Y+1,3);
    location<parameter>(dft16.k_tile2.param[0]) = bank(DFT16_X+0,DFT16_Y+2,2);
    location<parameter>(dft16.k_tile2.param[1]) = bank(DFT16_X+0,DFT16_Y+2,3);
    location<parameter>(dft16.k_tile3.param[0]) = bank(DFT16_X+0,DFT16_Y+1,1);
    location<parameter>(dft16.k_tile3.param[1]) = bank(DFT16_X+0,DFT16_Y+1,2);

    location<buffer>(dft16.k_tile4.in[0]) = { bank(DFT16_X+1,DFT16_Y+1,1), bank(DFT16_X+1,DFT16_Y+1,3) };
    location<buffer>(dft16.k_tile4.in[1]) = { bank(DFT16_X+2,DFT16_Y+1,0), bank(DFT16_X+2,DFT16_Y+1,1) };
    location<buffer>(dft16.k_tile4.in[2]) = { bank(DFT16_X+1,DFT16_Y+2,1), bank(DFT16_X+1,DFT16_Y+2,2) };
    location<buffer>(dft16.k_tile4.in[3]) = { bank(DFT16_X+1,DFT16_Y+1,0), bank(DFT16_X+1,DFT16_Y+1,2) };

  }
};

// Instantiate AIE graph:
dut_graph<18,0> aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(8);
  aie_dut.end();

  return 0;
}

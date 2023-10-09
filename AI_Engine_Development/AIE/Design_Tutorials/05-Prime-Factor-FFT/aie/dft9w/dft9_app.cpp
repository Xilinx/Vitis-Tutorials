//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "dft9_graph.h"

using namespace adf;

template<int DFT9_X, int DFT9_Y>
class dut_graph : public graph {
  dft9_graph dft9;
public:
  std::array< input_plio,2> sig_i;
  std::array<output_plio,2> sig_o;
  dut_graph( void )
  {
    sig_i[0] =  input_plio::create("PLIO_i_0",plio_64_bits,"data/sig_i_0.txt");
    sig_i[1] =  input_plio::create("PLIO_i_1",plio_64_bits,"data/sig_i_1.txt");
    sig_o[0] = output_plio::create("PLIO_o_0",plio_64_bits,"data/sig_o_0.txt");
    sig_o[1] = output_plio::create("PLIO_o_1",plio_64_bits,"data/sig_o_1.txt");
    connect<stream>(sig_i[0].out[0],dft9.sig_i[0]);
    connect<stream>(sig_i[1].out[0],dft9.sig_i[1]);
    connect<stream>(dft9.sig_o[0],sig_o[0].in[0]);
    connect<stream>(dft9.sig_o[1],sig_o[1].in[0]);

    location<kernel>(dft9.k_tile0) = tile(DFT9_X+1,DFT9_Y+1);
    location<kernel>(dft9.k_tile1) = tile(DFT9_X+0,DFT9_Y+2);
    location<kernel>(dft9.k_tile2) = tile(DFT9_X+1,DFT9_Y+0);
    location<kernel>(dft9.k_tile3) = tile(DFT9_X+0,DFT9_Y+1);

    location<stack>(dft9.k_tile0) = bank(DFT9_X+1,DFT9_Y+0,2);
    location<stack>(dft9.k_tile1) = bank(DFT9_X-1,DFT9_Y+2,3); // Outside 2x3 box
    location<stack>(dft9.k_tile2) = bank(DFT9_X+1,DFT9_Y+0,0);
    location<stack>(dft9.k_tile3) = bank(DFT9_X+0,DFT9_Y+1,2);

    location<parameter>(dft9.k_tile0.param[0]) = bank(DFT9_X+1,DFT9_Y+0,3);
    location<parameter>(dft9.k_tile1.param[0]) = bank(DFT9_X+0,DFT9_Y+1,3);
    location<parameter>(dft9.k_tile2.param[0]) = bank(DFT9_X+1,DFT9_Y+0,1);

    location<buffer>(dft9.k_tile0.in[0]) = { bank(DFT9_X+1,DFT9_Y+2,0),bank(DFT9_X+1,DFT9_Y+2,1) };
    location<buffer>(dft9.k_tile0.in[1]) = { bank(DFT9_X+1,DFT9_Y+2,2),bank(DFT9_X+1,DFT9_Y+2,3) };
    location<buffer>(dft9.k_tile1.in[0]) = { bank(DFT9_X+0,DFT9_Y+2,0),bank(DFT9_X+0,DFT9_Y+2,1) };
    location<buffer>(dft9.k_tile1.in[1]) = { bank(DFT9_X+0,DFT9_Y+2,2),bank(DFT9_X+0,DFT9_Y+2,3) };
    location<buffer>(dft9.k_tile2.in[0]) = { bank(DFT9_X+0,DFT9_Y+0,0),bank(DFT9_X+0,DFT9_Y+0,1) };
    location<buffer>(dft9.k_tile2.in[1]) = { bank(DFT9_X+0,DFT9_Y+0,2),bank(DFT9_X+0,DFT9_Y+0,3) };
    location<buffer>(dft9.k_tile3.in[0]) = { bank(DFT9_X+1,DFT9_Y+1,0),bank(DFT9_X+1,DFT9_Y+1,1) };
    location<buffer>(dft9.k_tile3.in[1]) = { bank(DFT9_X+0,DFT9_Y+1,0),bank(DFT9_X+0,DFT9_Y+1,1) };
    location<buffer>(dft9.k_tile3.in[2]) = { bank(DFT9_X+1,DFT9_Y+1,2),bank(DFT9_X+1,DFT9_Y+1,3) };
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

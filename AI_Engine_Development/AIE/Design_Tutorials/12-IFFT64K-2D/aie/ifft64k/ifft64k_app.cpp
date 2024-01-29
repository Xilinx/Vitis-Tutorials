//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "ifft64k_graph.h"

using TT_DUT = ifft64k_graph;

class my_graph : public graph {
public:
  static constexpr unsigned F_X = 18;
  static constexpr unsigned F_Y = 0;
  static constexpr unsigned B_X = 18;
  static constexpr unsigned B_Y = 1;
  std::array< input_plio,TT_DUT::NFRONT> front_i;
  std::array<output_plio,TT_DUT::NFRONT> front_o;
  std::array< input_plio,TT_DUT::NBACK>  back_i;
  std::array<output_plio,TT_DUT::NBACK>  back_o;
  TT_DUT dut;
  my_graph( void )
  {
    for ( unsigned ff=0; ff < TT_DUT::NFRONT; ff++) {
      std::string fname_i = "data/front_i_" + std::to_string(ff) + ".txt";
      std::string fname_o = "data/front_o_" + std::to_string(ff) + ".txt";
      std::string pname_i = "PLIO_front_i_" + std::to_string(ff);
      std::string pname_o = "PLIO_front_o_" + std::to_string(ff);
#ifdef AIE_SIM_ONLY
      front_i[ff] =  input_plio::create(pname_i,plio_64_bits,fname_i);
      front_o[ff] = output_plio::create(pname_o,plio_64_bits,fname_o);
#else
      front_i[ff] =  input_plio::create(pname_i,plio_64_bits);
      front_o[ff] = output_plio::create(pname_o,plio_64_bits);
#endif
      connect( front_i[ff].out[0],  dut.front_i[ff]   );
      connect( dut.front_o[ff],  front_o[ff].in[0] );
    }
    for ( unsigned bb=0; bb < TT_DUT::NBACK; bb++) {
      std::string fname_i = "data/back_i_" + std::to_string(bb) + ".txt";
      std::string fname_o = "data/back_o_" + std::to_string(bb) + ".txt";
      std::string pname_i = "PLIO_back_i_" + std::to_string(bb);
      std::string pname_o = "PLIO_back_o_" + std::to_string(bb);
#ifdef AIE_SIM_ONLY
      back_i[bb] =  input_plio::create(pname_i,plio_64_bits,fname_i);
      back_o[bb] = output_plio::create(pname_o,plio_64_bits,fname_o);
#else
      back_i[bb] =  input_plio::create(pname_i,plio_64_bits);
      back_o[bb] = output_plio::create(pname_o,plio_64_bits);
#endif
      connect( back_i[bb].out[0],  dut.back_i[bb]   );
      connect( dut.back_o[bb],     back_o[bb].in[0] );
    }
    location<kernel>(dut.g_front0.k_kernel) = tile(F_X+0,F_Y+0);
    location<kernel>(dut.g_front1.k_kernel) = tile(F_X+1,F_Y+0);
    location<kernel>(dut.g_front2.k_kernel) = tile(F_X+2,F_Y+0);
    location<kernel>(dut.g_front3.k_kernel) = tile(F_X+3,F_Y+0);
    location<kernel>(dut.g_front4.k_kernel) = tile(F_X+4,F_Y+0);

    location<kernel>(dut.g_back[0].k_kernel) = tile(B_X+0,B_Y+0);
    location<kernel>(dut.g_back[1].k_kernel) = tile(B_X+1,B_Y+0);
    location<kernel>(dut.g_back[2].k_kernel) = tile(B_X+2,B_Y+0);
    location<kernel>(dut.g_back[3].k_kernel) = tile(B_X+3,B_Y+0);
    location<kernel>(dut.g_back[4].k_kernel) = tile(B_X+4,B_Y+0);
  }
};


// Instantiate AIE graph:
my_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run( TT_DUT::NSAMP/TT_DUT::NFRONT ); // One transform
  aie_dut.end();

  return 0;
}

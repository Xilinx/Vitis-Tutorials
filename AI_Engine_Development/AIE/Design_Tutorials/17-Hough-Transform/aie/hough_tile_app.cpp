//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "hough_tile_graph.h"

std::vector<int> LOC_X( { 16, 17, 18, 19, 20, 21, 22, 23, 16, 17, 18, 19, 20, 21, 22, 23,
                          16, 17, 18, 19, 20, 21, 22, 23, 16, 17, 18, 19, 20, 21, 22, 23 } );
std::vector<int> LOC_Y( {  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,
                           2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  3 } );

class dut_graph : public graph {
public:
  // Image dimensions from Matlab picture:
  static constexpr int RR = 216;
  static constexpr int CC = 240;
  static constexpr int RHO_MAX = 322;
  static constexpr int NR = 4;
  static constexpr int NC = 8;
  static constexpr int NT = NR * NC;
  static constexpr int THETA_NUM = 128;

  // Graph
  hough_tile_graph<RR,CC,NR,NC,RHO_MAX> dut;

  // Ports
  std::array< input_plio,NT> sig_i;
  std::array<output_plio,NT> sig_o;

  // Constructor
  dut_graph( void ) : dut( LOC_X, LOC_Y )
  {
    for (int tt=0; tt < NT; tt++) {
      sig_i[tt] =  input_plio::create("PLIO_i_"+std::to_string(tt), plio_64_bits, "data/sig_i_"+std::to_string(tt)+".txt");
      sig_o[tt] = output_plio::create("PLIO_o_"+std::to_string(tt), plio_64_bits, "data/sig_o_"+std::to_string(tt)+".txt");
      connect<stream,stream>( sig_i[tt].out[0], dut.sig_i[tt] );
      connect<stream,stream>( dut.sig_o[tt],    sig_o[tt].in[0] );
    } // tt
  }
};

dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{

  aie_dut.init();
  aie_dut.run(1);
  aie_dut.end();

  return 0;
}

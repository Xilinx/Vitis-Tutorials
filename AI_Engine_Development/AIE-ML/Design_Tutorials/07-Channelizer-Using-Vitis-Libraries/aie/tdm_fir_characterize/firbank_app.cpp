//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "firbank_graph.h"

class dut_graph : public graph {
public:
  #include "firbank_init_taps.h"
  firbank_graph dut;
  std::array< input_plio,firbank_graph::TP_SSR> sig_i;
  std::array<output_plio,firbank_graph::TP_SSR> sig_o;
  int start_index = 2;
  dut_graph(void) : dut(TAPS_INIT_0)
  {
    for (unsigned ii=0; ii < firbank_graph::TP_SSR; ii+=4) {
      int xoff = ii >> 2;
      std::string file_i0 = "data/filterbank_i_" + std::to_string(ii) + ".txt";
      std::string file_o0 = "data/filterbank_o_" + std::to_string(ii) + ".txt";
      sig_i[ii] =  input_plio::create("PLIO_i_"+std::to_string(ii), plio_64_bits, file_i0 );
      sig_o[ii] = output_plio::create("PLIO_o_"+std::to_string(ii), plio_64_bits, file_o0 );

      connect<>(     sig_i[ii].out[0], dut.sig_i[ii] );
      connect<>( dut.sig_o[ii],            sig_o[ii].in[0] );

      std::string file_i1 = "data/filterbank_i_" + std::to_string(ii+1) + ".txt";
      std::string file_o1 = "data/filterbank_o_" + std::to_string(ii+1) + ".txt";
      sig_i[ii+1] =  input_plio::create("PLIO_i_"+std::to_string(ii+1), plio_64_bits, file_i1 );
      sig_o[ii+1] = output_plio::create("PLIO_o_"+std::to_string(ii+1), plio_64_bits, file_o1 );
      connect<>(     sig_i[ii+1].out[0], dut.sig_i[ii+1] );
      connect<>( dut.sig_o[ii+1],            sig_o[ii+1].in[0] );

      std::string file_i2 = "data/filterbank_i_" + std::to_string(ii+2) + ".txt";
      std::string file_o2 = "data/filterbank_o_" + std::to_string(ii+2) + ".txt";
      sig_i[ii+2] =  input_plio::create("PLIO_i_"+std::to_string(ii+2), plio_64_bits, file_i2 );
      sig_o[ii+2] = output_plio::create("PLIO_o_"+std::to_string(ii+2), plio_64_bits, file_o2 );
      connect<>(     sig_i[ii+2].out[0], dut.sig_i[ii+2] );
      connect<>( dut.sig_o[ii+2],            sig_o[ii+2].in[0] );

      std::string file_i3 = "data/filterbank_i_" + std::to_string(ii+3) + ".txt";
      std::string file_o3 = "data/filterbank_o_" + std::to_string(ii+3) + ".txt";
      sig_i[ii+3] =  input_plio::create("PLIO_i_"+std::to_string(ii+3), plio_64_bits, file_i3 );
      sig_o[ii+3] = output_plio::create("PLIO_o_"+std::to_string(ii+3), plio_64_bits, file_o3 );
      connect<>(     sig_i[ii+3].out[0], dut.sig_i[ii+3] );
      connect<>( dut.sig_o[ii+3],            sig_o[ii+3].in[0] );
    }
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

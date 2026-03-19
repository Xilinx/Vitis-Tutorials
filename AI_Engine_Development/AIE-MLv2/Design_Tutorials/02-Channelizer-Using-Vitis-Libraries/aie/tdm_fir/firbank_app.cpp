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
  std::array< input_plio,firbank_graph::NPORT_I> sig_i;
  std::array<output_plio,firbank_graph::NPORT_O> sig_o;
  int start_index = 2;
  dut_graph(void) : dut(TAPS_INIT_0)
  {
    for (unsigned ii=0; ii < firbank_graph::TP_SSR; ii+=4) {
      int xoff = ii >> 2;

      single_buffer(dut.tdmfir.graph_instance.m_firKernels[ii+0].in[0]);
      location<kernel>   (dut.tdmfir.graph_instance.m_firKernels[ii])                 =      tile(start_index+xoff,0);
      location<stack>    (dut.tdmfir.graph_instance.m_firKernels[ii])                 =      bank(start_index+xoff,0,3);
      location<parameter>(dut.tdmfir.graph_instance.m_firKernels[ii].param[0])        =      bank(start_index+xoff,0,3);
      location<parameter>(dut.tdmfir.graph_instance.m_firKernels[ii].param[1])        =   address(start_index+xoff,0,0x4C00);
      location<buffer>   (dut.tdmfir.graph_instance.m_firKernels[ii].in[0])           =      bank(start_index+xoff,0,0);
      location<buffer>   (dut.tdmfir.graph_instance.m_firKernels[ii].out[0])          = {    bank(start_index+xoff,0,1), bank(start_index+xoff,0,3) };

      single_buffer(dut.tdmfir.graph_instance.m_firKernels[ii+1].in[0]);
      location<kernel>   (dut.tdmfir.graph_instance.m_firKernels[ii+1])                 =      tile(start_index+xoff,1);
      location<stack>    (dut.tdmfir.graph_instance.m_firKernels[ii+1])                 =      bank(start_index+xoff,1,3);
      location<parameter>(dut.tdmfir.graph_instance.m_firKernels[ii+1].param[0])        =      bank(start_index+xoff,1,3);
      location<parameter>(dut.tdmfir.graph_instance.m_firKernels[ii+1].param[1])        =   address(start_index+xoff,1,0x4C00);
      location<buffer>   (dut.tdmfir.graph_instance.m_firKernels[ii+1].in[0])           =      bank(start_index+xoff,1,0);
      location<buffer>   (dut.tdmfir.graph_instance.m_firKernels[ii+1].out[0])          = {    bank(start_index+xoff,1,1), bank(start_index+xoff,1,3) };

      single_buffer(dut.tdmfir.graph_instance.m_firKernels[ii+2].in[0]);
      location<kernel>   (dut.tdmfir.graph_instance.m_firKernels[ii+2])                 =      tile(start_index+xoff,2);
      location<stack>    (dut.tdmfir.graph_instance.m_firKernels[ii+2])                 =      bank(start_index+xoff,2,3);
      location<parameter>(dut.tdmfir.graph_instance.m_firKernels[ii+2].param[0])        =      bank(start_index+xoff,2,3);
      location<parameter>(dut.tdmfir.graph_instance.m_firKernels[ii+2].param[1])        =   address(start_index+xoff,2,0x4C00);
      location<buffer>   (dut.tdmfir.graph_instance.m_firKernels[ii+2].in[0])           =      bank(start_index+xoff,2,0);
      location<buffer>   (dut.tdmfir.graph_instance.m_firKernels[ii+2].out[0])          = {    bank(start_index+xoff,2,1), bank(start_index+xoff,2,3) };

      single_buffer(dut.tdmfir.graph_instance.m_firKernels[ii+3].in[0]);
      location<kernel>   (dut.tdmfir.graph_instance.m_firKernels[ii+3])                 =      tile(start_index+xoff,3);
      location<stack>    (dut.tdmfir.graph_instance.m_firKernels[ii+3])                 =      bank(start_index+xoff,3,3);
      location<parameter>(dut.tdmfir.graph_instance.m_firKernels[ii+3].param[0])        =      bank(start_index+xoff,3,3);
      location<parameter>(dut.tdmfir.graph_instance.m_firKernels[ii+3].param[1])        =   address(start_index+xoff,3,0x4C00);
      location<buffer>   (dut.tdmfir.graph_instance.m_firKernels[ii+3].in[0])           =      bank(start_index+xoff,3,0);
      location<buffer>   (dut.tdmfir.graph_instance.m_firKernels[ii+3].out[0])          = {    bank(start_index+xoff,3,1), bank(start_index+xoff,3,3) };
    }
    for (unsigned ii=0; ii < firbank_graph::NPORT_I; ii++) {
      std::string file_i0 = "data/filterbank_i_" + std::to_string(ii) + ".csv";
      sig_i[ii] =  input_plio::create("PLIO_i_"+std::to_string(ii), plio_64_bits, file_i0 );
      connect<>(     sig_i[ii].out[0], dut.sig_i[ii] );
    }
    for (unsigned ii=0; ii < firbank_graph::NPORT_O; ii++) {
      std::string file_o0 = "data/filterbank_o_" + std::to_string(ii) + ".txt";
      sig_o[ii] = output_plio::create("PLIO_o_"+std::to_string(ii), plio_64_bits, file_o0 );
      connect<>( dut.sig_o[ii],            sig_o[ii].in[0] );
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

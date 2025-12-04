//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "max_pool1d_w2_graph.h"

class dut_graph : public graph {
  max_pool1d_w2_graph dut;
public:
  input_plio   data_i;
  output_plio  data_o;

  dut_graph( void )
  {
    data_i =  input_plio::create("PLIO_i_0",plio_64_bits,"data/data_i.txt");
    connect<>(data_i.out[0],dut.data_i);

    data_o = output_plio::create("PLIO_o_0",plio_64_bits,"data/data_o.txt");
    connect<>(dut.data_o,data_o.in[0]);
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

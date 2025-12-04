//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <fstream>
#include <iostream>
#include "conv1d_w1_graph.h"
#include "read_file.h"

class dut_graph : public graph {
public:
  conv1d_w1_graph     dut;
  input_plio          data_iq;
  port<direction::in> weights;
  port<direction::in> biases;
  output_plio         data_o;

  dut_graph( void )
  {
    data_iq =  input_plio::create("PLIO_iq_0",plio_64_bits,"data/data_iq.txt");
    connect<>(data_iq.out[0],dut.data_iq);

    connect<>(weights,dut.weights);
    connect<>(biases,dut.biases);

    data_o = output_plio::create("PLIO_o_0",plio_64_bits,"data/data_o.txt");
    connect<>(dut.data_o,data_o.in[0]);
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

bfloat16 weights_array[896];
bfloat16 biases_array[64];

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();

  // Read layer RTPs
  read_file<bfloat16,896>("data/weights-rtp.txt",weights_array);
  read_file<bfloat16,64> ("data/biases-rtp.txt",  biases_array);

  aie_dut.update(aie_dut.weights,weights_array,896);
  aie_dut.update(aie_dut.biases,biases_array,64);

  aie_dut.run(4);
  aie_dut.end();

  return 0;
}

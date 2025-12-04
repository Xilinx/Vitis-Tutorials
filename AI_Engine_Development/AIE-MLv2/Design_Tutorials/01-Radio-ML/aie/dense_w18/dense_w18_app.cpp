//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <fstream>
#include <iostream>
#include "dense_w18_graph.h"
#include "read_file.h"

class dut_graph : public graph {
public:
  dense_w18_graph     dut;
  input_plio          data_i;
  output_plio         data_o;
  port<direction::in> weights;
  port<direction::in> biases;

  dut_graph( void )
  {
    data_i =  input_plio::create("PLIO_i_0",plio_64_bits,"data/data_i.txt");
    connect<>(data_i.out[0],dut.data_i);

    connect<>(weights,dut.weights);
    connect<>(biases,dut.biases);

    data_o = output_plio::create("PLIO_o_0",plio_64_bits,"data/data_o.txt");
    connect<>(dut.data_o,data_o.in[0]);
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

bfloat16 weights_array[3072];
bfloat16 biases_array[24];


// Initialize and run the graph:
int main(void)
{
  aie_dut.init();

  // Read layer RTPs
  read_file<bfloat16,3072>("data/weights-rtp.txt",weights_array);
  read_file<bfloat16,24> ("data/biases-rtp.txt",  biases_array);

  aie_dut.update(aie_dut.weights,weights_array,3072);
  aie_dut.update(aie_dut.biases,biases_array,24);

  aie_dut.run(4);
  aie_dut.end();

  return 0;
}

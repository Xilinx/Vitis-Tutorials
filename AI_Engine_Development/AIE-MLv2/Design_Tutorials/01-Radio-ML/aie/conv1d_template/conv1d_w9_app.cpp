//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <fstream>
#include <iostream>
#include "conv1d_graph_template.h"
#include "read_file.h"

class dut_graph : public graph {
  conv1d_graph_template<64,64,7> dut;         // <NSAMP,NNODES,KERNEL_SIZE>
					       // NSAMP is number of samples from each node
					       // NNODES is number nodes
					       // KERNEL_SIZE is the number of weights per node
public:
  input_plio          data_i;
  port<direction::in> weights;
  port<direction::in> biases;
  output_plio         data_o;

  dut_graph( void )
  {
    data_i =  input_plio::create("PLIO_i_0",plio_64_bits,"data/data_i.txt");
    connect<stream>(data_i.out[0],dut.data_i);

    connect<>(weights,dut.weights);
    connect<>(biases,dut.biases);

    data_o = output_plio::create("PLIO_o_0",plio_64_bits,"data/data_o.txt");
    connect<stream>(dut.data_o,data_o.in[0]);
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

bfloat16 weights_array[conv1d_graph_template<64,64,7>::NWEIGHTS];
bfloat16 biases_array[conv1d_graph_template<64,64,7>::NBIASES];

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  
  // Read layer RTPs
  read_file<bfloat16,conv1d_graph_template<64,64,7>::NWEIGHTS>("data/weights-rtp.txt",weights_array);
  read_file<bfloat16,conv1d_graph_template<64,64,7>::NBIASES> ("data/biases-rtp.txt",  biases_array);

  aie_dut.update(aie_dut.weights,weights_array,conv1d_graph_template<64,64,7>::NWEIGHTS);
  aie_dut.update(aie_dut.biases,biases_array,conv1d_graph_template<64,64,7>::NBIASES);
  aie_dut.run(4);
  aie_dut.end();

  return 0;
}

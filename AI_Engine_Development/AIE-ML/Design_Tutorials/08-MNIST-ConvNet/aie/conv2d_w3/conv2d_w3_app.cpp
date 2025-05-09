//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "conv2d_w3_graph.h"

class dut_graph : public graph {
public:
  conv2d_w3_graph dut;
  input_plio   ifm_i;
  input_plio   wts_i;
  output_plio  ofm_o;

  dut_graph( void )
  {
    ifm_i =  input_plio::create("PLIO_i",plio_64_bits,"data/ifm_i.txt");
    wts_i =  input_plio::create("PLIO_w",plio_64_bits,"data/wts_i.txt");
    ofm_o = output_plio::create("PLIO_o",plio_64_bits,"data/ofm_o.txt");

    connect<>(wts_i.out[0],  dut.wts_i);
    connect<>(ifm_i.out[0],  dut.ifm_i);
    connect<>(dut.ofm_o,     ofm_o.in[0]);
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  // Use '-1' instead of '1' to workaround CR-1236063:
  aie_dut.run(-1);               // 1 iteration = 4 images
  aie_dut.end();

  return 0;
}

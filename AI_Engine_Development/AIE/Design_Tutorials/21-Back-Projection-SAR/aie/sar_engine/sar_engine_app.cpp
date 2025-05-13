//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "sar_engine_graph.h"

class dut_graph : public graph {
public:
  sar_engine_graph<256,1024,0> dut;
  input_gmio                   image_i;
  input_port                   coord_i;
  input_plio                   ifft_i;
  output_gmio                  image_o;
  dut_graph(void) {
    image_i =  input_gmio::create("GMIO_image_i",64,3200);
    image_o = output_gmio::create("GMIO_image_o",64,3200);
    ifft_i  =  input_plio::create("PLIO_ifft_i",plio_64_bits,"data/ifft_i.txt");
    connect<>(coord_i,dut.coord_i);
    connect<>(image_i.out[0],dut.image_i);
    connect<>(ifft_i.out[0],dut.ifft_i);
    connect<>(dut.image_o,image_o.in[0]);
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(1);
  aie_dut.end();

  return 0;
}

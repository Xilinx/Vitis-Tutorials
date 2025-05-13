//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "interp1_graph.h"
#include "dummy_async.h"

// This graph includes a dummy kernel to test input asynchronous buffer:
class dut_graph : public graph {
public:
  kernel                 kk;
  interp1_graph<16,1024> dut;
  input_plio             lut_i;
  input_plio             domain_i;
  output_plio            interp_o;

  dut_graph(void) {
    lut_i    =  input_plio::create("PLIO_lut_i",   plio_64_bits, "data/lut_i.txt" );
    domain_i =  input_plio::create("PLIO_domain_i",plio_64_bits, "data/domain_i.txt" );
    interp_o = output_plio::create("PLIO_interp_o",plio_64_bits, "data/interp_o.txt" );

    kk = kernel::create_object<dummy_async>();
    source(kk) = "dummy_async.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = 1;
    single_buffer(kk.in[0]);
    single_buffer(kk.out[0]);

    connect<>(lut_i.out[0], kk.in[0]);        dimensions(kk.in[0])  = { interp1<16,1024>::NLUT };
    connect<>(kk.out[0],dut.lut_i);           dimensions(kk.out[0]) = { interp1<16,1024>::NLUT };
    connect<>(domain_i.out[0],dut.domain_i);
    connect<>(dut.interp_o,interp_o.in[0]);
  }
};

dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(2);               // Run 2 radar pulses
  aie_dut.end();

  return 0;
}

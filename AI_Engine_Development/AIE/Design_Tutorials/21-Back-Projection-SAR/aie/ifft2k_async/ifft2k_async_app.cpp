//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <array>
#include "dummy_async.h"
#include "ifft2k_async_graph.h"

// This graph includes dummy kernel to test output asynchronous buffer:
class dut_graph : public graph {
public:
  std::array<kernel,2> kk;
  ifft2k_async_graph  dut;
  input_plio          sig_i;
  output_plio         real_o;
  output_plio         imag_o;

  dut_graph(void)
  {
    sig_i  =  input_plio::create("PLIO_i",plio_64_bits,"data/sig_i_0.txt");
    real_o = output_plio::create("PLIO_R",plio_64_bits,"data/real_o.txt");
    imag_o = output_plio::create("PLIO_I",plio_64_bits,"data/imag_o.txt");
    for (unsigned rr=0; rr < 2; rr++) {
      kk[rr] = kernel::create_object<dummy_async>();
      source(kk[rr]) = "dummy_async.cpp";
      runtime<ratio>(kk[rr]) = 0.9;
      repetition_count(kk[rr]) = 1;
      single_buffer(kk[rr].in[0]);
      single_buffer(kk[rr].out[0]);
      dimensions(kk[rr].in[0])  = { dummy_async::NSAMP };
      dimensions(kk[rr].out[0]) = { dummy_async::NSAMP };
    }
    connect<>( sig_i.out[0], dut.sig_i    );
    connect<>( dut.real_o,   kk[0].in[0]  );
    connect<>( dut.imag_o,   kk[1].in[0]  );
    connect<>( kk[0].out[0], real_o.in[0] );
    connect<>( kk[1].out[0], imag_o.in[0] );
    // connect<>( sig_i.out[0], kk[0].in[0]);
    // connect<>( sig_i.out[0], kk[1].in[0]);
    // connect<>( kk[0].out[0], real_o.in[0] );
    // connect<>( kk[1].out[0], imag_o.in[0] );
  }
};


// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(4); // One transform per graph iteration
  aie_dut.end();

  return 0;
}

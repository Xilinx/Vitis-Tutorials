//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "range_gen_graph.h"

static constexpr unsigned REPEAT = 32;
static constexpr unsigned NSAMP  = 1024;
static constexpr unsigned ID     = 4;

class dut_graph : public graph {
public:
  range_gen_graph<REPEAT,NSAMP,ID>  dut;
  output_plio                       sig_o;
  dut_graph(void) {
    sig_o = output_plio::create("PLIO_o", plio_64_bits, "data/sig_o.txt" );
    connect<>(dut.sig_o, sig_o.in[0]);
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  printf("NENGINE:    %d\n",range_gen<REPEAT,NSAMP,ID>::NENGINE);
  printf("RANGE_INC:  %f\n",range_gen<REPEAT,NSAMP,ID>::RANGE_INC);
  printf("RANGE_STEP: %f\n",range_gen<REPEAT,NSAMP,ID>::RANGE_STEP);
  printf("TOT_SAMP:   %d\n",range_gen<REPEAT,NSAMP,ID>::TOT_SAMP);
  printf("X_INIT:     %f\n",range_gen<REPEAT,NSAMP,ID>::X_INIT);
  aie_dut.init();
  aie_dut.run(1);
  aie_dut.end();

  return 0;
}

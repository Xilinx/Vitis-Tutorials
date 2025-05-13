//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "dR_comp_graph.h"

class dut_graph : public graph {
public:
  dR_comp_graph<256,1024>   dut;
  input_plio                sig_i;
  input_port                R0_range_i;
  output_plio               domain_o;
  output_plio               theta_o;
  dut_graph(void) {
    sig_i    =  input_plio::create("PLIO_i",  plio_64_bits,"data/sig_i.txt");
    domain_o = output_plio::create("PLIO_domain_o",plio_64_bits,"data/domain_o.txt");
    theta_o  = output_plio::create("PLIO_theta_o",plio_64_bits,"data/theta_o.txt");
    connect<>(sig_i.out[0], dut.sig_i);
    connect<>(R0_range_i,dut.R0_range_i);
    connect<>(dut.domain_o, domain_o.in[0]);
    connect<>(dut.theta_o, theta_o.in[0]);
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
#include "R0_range.h"
  aie_dut.init();
  aie_dut.update(aie_dut.R0_range_i,R0_range,dR_comp<256,1024>::NPULSE);
  aie_dut.run(1);               // Run a single radar pulse
  aie_dut.end();

  return 0;
}

//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "io_adapter_graph.h"

class dut_graph : public adf::graph {
public:
  io_adapter_graph              io_adapter;
  std::array<adf::input_plio,2> sig_i;
  adf::output_plio              sig_o;

  dut_graph(void)
  {
    // I/O:
    sig_i[0] =  adf::input_plio::create("PLIO_i_0", adf::plio_64_bits, "data/sig_i_0.txt");
    sig_i[1] =  adf::input_plio::create("PLIO_i_1", adf::plio_64_bits, "data/sig_i_1.txt");
    sig_o    = adf::output_plio::create("PLIO_o",   adf::plio_64_bits, "data/sig_o.txt" );

    // Connections to block-level graph:
    adf::connect<>(sig_i[0].out[0],  io_adapter.sig_i[0]);
    adf::connect<>(sig_i[1].out[0],  io_adapter.sig_i[1]);
    adf::connect<>(io_adapter.sig_o, sig_o.in[0]);

    // Placement:
    static constexpr unsigned IO_ADAPTER_COL_START = 11;
    adf::location<graph>(*this)  = adf::bounding_box(IO_ADAPTER_COL_START, ROW_0,IO_ADAPTER_COL_START, ROW_0);
    // adf::location<adf::kernel>(io_adapter.io_adapter_kernel) = adf::tile(IO_ADAPTER_COL_START, ROW_0);
    // adf::location<adf::stack>(io_adapter.io_adapter_kernel) = adf::location<adf::kernel>(io_adapter.io_adapter_kernel)
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

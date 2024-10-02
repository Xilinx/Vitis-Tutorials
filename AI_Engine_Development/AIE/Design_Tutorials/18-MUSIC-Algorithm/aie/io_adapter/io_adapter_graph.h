//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

#include "io_adapter.h"

// ------------------------------------------------------------
// Block Level Graph
// ------------------------------------------------------------

class io_adapter_graph : public adf::graph {
public:
  adf::kernel                   io_adapter_kernel;
  std::array<adf::input_port,2> sig_i;
  adf::output_port              sig_o;

  // Constructor:
  io_adapter_graph(void)
  {
    // Kernel:
    io_adapter_kernel = adf::kernel::create_object<IO_Adapter>();
    adf::runtime<ratio>(io_adapter_kernel) = 0.9;
    adf::source(io_adapter_kernel) = "io_adapter.cpp";

    // Connections:
    adf::connect(sig_i[0], io_adapter_kernel.in[0]);
    adf::connect(sig_i[1], io_adapter_kernel.in[1]);
    adf::connect(io_adapter_kernel.out[0], sig_o);

    // Dimensions:
    // adf::dimensions(io_adapter_kernel.in[0])  = {ROW * COL};  // dimensions N/A for streaming interface
    adf::dimensions(io_adapter_kernel.out[0]) = {ROW * COL};
  }
};



//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "diff3dsq.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class diff3dsq_graph : public graph {
public:
  kernel      kk;
  input_plio  sig_i;
  output_plio sig_o;

  // Constructor:
  diff3dsq_graph(void)
  {
#include "diff3dsq_pulses.h"
    sig_i =  input_plio::create("PLIO_i", plio_64_bits, "data/sig_i.txt" );
    sig_o = output_plio::create("PLIO_o", plio_64_bits, "data/sig_o.txt" );

    kk = kernel::create_object<diff3dsq>(std::vector<float>{radar_coord});
    source(kk) = "diff3dsq.cpp";
    runtime<ratio>(kk) = 0.9;

    connect<>( sig_i.out[0],      kk.in[0] );  dimensions(kk.in[0])  = { 3*diff3dsq::NSAMP };
    connect<>( kk.out[0],      sig_o.in[0] );  dimensions(kk.out[0]) = {   diff3dsq::NSAMP };
  }
};



//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "fmod_floor.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class fmod_floor_graph : public graph {
public:
  kernel      kk;
  input_plio  sig_i;
  output_plio sig_o;

  // Constructor:
  fmod_floor_graph(void)
  {
    sig_i  =  input_plio::create("PLIO_i", plio_64_bits, "data/sig_i.txt" );
    sig_o  = output_plio::create("PLIO_o", plio_64_bits, "data/sig_o.txt" );

    kk = kernel::create_object<fmod_floor>();
    source(kk) = "fmod_floor.cpp";
    runtime<ratio>(kk) = 0.9;

    connect<>( sig_i.out[0],  kk.in[0] );  dimensions(kk.in[0])  = { fmod_floor::NSAMP };
    connect<>( kk.out[0],  sig_o.in[0] );  dimensions(kk.out[0]) = { fmod_floor::NSAMP };
    // Place everything in the same tile:
    location<graph>(*this) = bounding_box(24,0,24,0);
  }
};



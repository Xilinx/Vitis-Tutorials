//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "bitonic_fp1024.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class bitonic_fp1024_graph : public graph {
private:
  kernel kk;
  input_plio sig_i;
  output_plio sig_o;
public:

  // Constructor:
  bitonic_fp1024_graph( void )
  {
    sig_i =  input_plio::create("PLIO_i", plio_64_bits, "data/sig_i.txt" );
    sig_o = output_plio::create("PLIO_o", plio_64_bits, "data/sig_o.txt" );

    kk = kernel::create_object<bitonic_fp1024>( std::vector<float>(bitonic_fp1024::SIZE) );

    source(kk) = "bitonic_fp1024.cpp";
    runtime<ratio>(kk) = 0.9;
    connect<>(sig_i.out[0], kk.in[0]);
    connect<>(kk.out[0],sig_o.in[0]);
  }
};



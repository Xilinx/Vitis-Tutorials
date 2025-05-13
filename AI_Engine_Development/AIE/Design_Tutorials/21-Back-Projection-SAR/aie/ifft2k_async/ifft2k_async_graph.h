//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <array>
#include <adf.h>

#include "ifft2k_async.h"
#include "ifft2k_async_twiddles_external.h"
#include "slope_offset.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
//
// Throughput:  842 MB/sec or ~100 Msps
// ------------------------------------------------------------

class ifft2k_async_graph : public graph {
public:
  kernel         ifft;            // IFFT
  kernel         lut;             // Slope/Offset LUT
  port<input>    sig_i;
  port<output>   real_o;
  port<output>   imag_o;

  ifft2k_async_graph()
  {
    ifft = kernel::create_object<ifft2k_async>( tw1024, tw512 );
    source(ifft) = "ifft2k_async.cpp";
    runtime<ratio>(ifft) = 0.9;
    single_buffer(ifft.in[0]);
    single_buffer(ifft.out[0]);
    repetition_count(ifft) = 1;
    dimensions(ifft.in[0])  = { ifft2k_async::NSAMP };
    dimensions(ifft.out[0]) = { ifft2k_async::NSAMP };

    lut = kernel::create_object<slope_offset>();
    source(lut) = "slope_offset.cpp";
    runtime<ratio>(lut) = 0.9;
    single_buffer(lut.in[0]);
    single_buffer(lut.out[0]);
    single_buffer(lut.out[1]);
    repetition_count(lut) = 1;
    dimensions(lut.in[0])   = {   slope_offset::NSAMP };
    dimensions(lut.out[0])  = { 2*slope_offset::NSAMP };
    dimensions(lut.out[1])  = { 2*slope_offset::NSAMP };

    connect<>( sig_i,       ifft.in[0] );
    connect<>( ifft.out[0], lut.in[0]  );
    connect<>( lut.out[0],  real_o     );
    connect<>( lut.out[1],  imag_o     );
  }
};



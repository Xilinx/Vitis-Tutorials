//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <complex>
#include <ap_fixed.h>
#include <hls_stream.h>

namespace image_buffer_8engine {
  static constexpr unsigned        NSAMP = 1024*32;          // # of <cfloat> samples in input image
  static constexpr unsigned       NPULSE = 586;
  static constexpr unsigned        DEPTH = NSAMP/2;           // Memory depth in 128-bit samples
  static constexpr unsigned        NBITS = 128;               // Size of PLIO bus on PL side @ 312.5 MHz
  static constexpr unsigned      NENGINE = 8;
  typedef ap_uint<NBITS>                 TT_DATA;             // Equals two 'cint32' samples
  typedef hls::stream<TT_DATA>           TT_STREAM;

  static_assert(NSAMP*NENGINE == 512*512,"Incompatible parameters in image_buffer_8engine()");
};

// Run:
void image_buffer_8engine_wrapper( image_buffer_8engine::TT_DATA    mem[image_buffer_8engine::DEPTH],
                                   unsigned                         NPULSE_USE,
                                   image_buffer_8engine::TT_STREAM& buff_to_aie,
                                   image_buffer_8engine::TT_STREAM& aie_to_buff );

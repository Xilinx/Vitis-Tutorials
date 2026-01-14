//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <complex>
#include <ap_fixed.h>
#include <hls_stream.h>
#include <hls_streamofblocks.h>

namespace split_1x16 {
  static constexpr unsigned    NSTREAM_I = 1;
  static constexpr unsigned    NSTREAM_O = 16;
  static constexpr unsigned     NSAMPLES = 64;
  static constexpr unsigned        DEPTH = NSAMPLES/4;  // Buffer depth in 128-bit words
  static constexpr unsigned        NBITS = 128;         // Size of PLIO bus on PL side @ 312.5 MHz
  typedef ap_uint<NBITS>                 TT_DATA;       // Equals four 'cint16' samples
  typedef ap_uint<NBITS/4>               TT_SAMPLE;     // Samples are 'cint16'
  typedef hls::stream<TT_DATA>           TT_STREAM;
  typedef TT_DATA                        TT_BLOCK[DEPTH];
};

// Run:
void split_1x16_wrapper( split_1x16::TT_STREAM& sig_i,
                         split_1x16::TT_STREAM sig_o[split_1x16::NSTREAM_O] );

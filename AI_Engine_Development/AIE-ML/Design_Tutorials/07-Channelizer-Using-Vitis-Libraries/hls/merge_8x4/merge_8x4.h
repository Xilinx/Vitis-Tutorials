//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <complex>
#include <ap_fixed.h>
#include <hls_stream.h>
#include <hls_streamofblocks.h>

namespace merge_8x4 {
  static constexpr unsigned    NSTREAM_I = 8;
  static constexpr unsigned    NSTREAM_O = 4;
  static constexpr unsigned     NSAMPLES = 16;
  static constexpr unsigned        DEPTH = NSAMPLES/2/4;// Buffer depth in 128-bit words
  static constexpr unsigned        NBITS = 128;         // Size of PLIO bus on PL side @ 312.5 MHz
  typedef ap_uint<NBITS>                 TT_DATA;       // Equals two 'cint32' samples
  typedef ap_uint<NBITS/2>               TT_SAMPLE;     // Samples are 'cint32'
  typedef hls::stream<TT_DATA>           TT_STREAM;
  typedef TT_DATA                        TT_BLOCK[DEPTH];
};

// Run:
void merge_8x4_wrapper( merge_8x4::TT_STREAM sig_i[merge_8x4::NSTREAM_I],
                        merge_8x4::TT_STREAM sig_o[merge_8x4::NSTREAM_O] );

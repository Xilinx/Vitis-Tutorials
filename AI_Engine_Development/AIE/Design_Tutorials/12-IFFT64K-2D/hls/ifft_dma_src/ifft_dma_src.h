//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <complex>
#include <ap_fixed.h>
#include <hls_stream.h>

namespace ifft_dma_src {
  static constexpr unsigned      NSTREAM = 5;
  static constexpr unsigned      EXTRA = 4;                   // # of extra zero-padded samples (to made divisible by NSTREAM)
  static constexpr unsigned      NFFT_1D = 256;
  static constexpr unsigned         NROW = 52;                // # of rows of transforms per bank
  static constexpr unsigned         NFFT = NFFT_1D*NFFT_1D;
  static constexpr unsigned        DEPTH = (NFFT_1D+EXTRA);   // Depth of each bank
  static constexpr unsigned        NBITS = 128;               // Size of PLIO bus on PL side @ 312.5 MHz
  typedef ap_uint<NBITS>                 TT_DATA;             // Equals two 'cint32' samples
  typedef ap_uint<NBITS/2>               TT_SAMPLE;           // Samples are 'cint32'
  typedef hls::stream<TT_DATA>           TT_STREAM;
};

// Run:
void ifft_dma_src_wrapper( ifft_dma_src::TT_DATA mem[ifft_dma_src::NFFT/2],
                           int loop_cnt,
                           ifft_dma_src::TT_STREAM sig_o[ifft_dma_src::NSTREAM] );

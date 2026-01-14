//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <complex>
#include <ap_fixed.h>
#include <hls_stream.h>

namespace channelizer_dma_snk {
  static constexpr unsigned      NSTREAM = 4;                             // Channelizer design needs to run at cint32@2 Gsps, corresponding to 4 HLS streams 128-bits@312.5
  static constexpr unsigned      NFFT_1D = 64;
  static constexpr unsigned         NFFT = NFFT_1D*NFFT_1D;
  static constexpr unsigned      NTRANSFORMS = 4;                         // 
  static constexpr unsigned        DEPTH = NFFT*NTRANSFORMS/NSTREAM/2;    // Two 'cint32' stored together.
  static constexpr unsigned        NBITS = 128;                           // Size of PLIO bus on PL side @ 312.5 MHz
  typedef ap_uint<NBITS>                 TT_DATA;                         // Equals two 'cint32' samples
  typedef ap_uint<NBITS/2>               TT_SAMPLE;                       // Samples are 'cint32'
  typedef hls::stream<TT_DATA>           TT_STREAM;
};

// Run:
void channelizer_dma_snk_wrapper( channelizer_dma_snk::TT_DATA mem[channelizer_dma_snk::DEPTH*channelizer_dma_snk::NSTREAM],
                                  int loop_sel,
                                  int loop_cnt,
                                  channelizer_dma_snk::TT_STREAM sig_i[channelizer_dma_snk::NSTREAM] );

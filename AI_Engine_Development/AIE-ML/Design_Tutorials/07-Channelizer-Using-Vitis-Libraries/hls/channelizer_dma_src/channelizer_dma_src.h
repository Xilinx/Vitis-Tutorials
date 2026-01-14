//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <complex>
#include <ap_fixed.h>
#include <hls_stream.h>

namespace channelizer_dma_src {
  static constexpr unsigned      NSTREAM = 2;                           // Channelizer design needs to run at cint16@2 Gsps, corresponding to 2 HLS streams 128-bits@312.5
  static constexpr unsigned         NCHAN = 4096;                       // Number of channels in channelizer
  static constexpr unsigned      NTRANSFORMS = 4;                       // 
  static constexpr unsigned        DEPTH = NTRANSFORMS*NCHAN/NSTREAM/4; // Four 'cint16' stored together.
  static constexpr unsigned        NBITS = 128;                         // Size of PLIO bus on PL side @ 312.5 MHz
  typedef ap_uint<NBITS>                 TT_DATA;                       // Equals four 'cint16' samples
  typedef ap_uint<NBITS/4>               TT_SAMPLE;                     // Samples are 'cint16'
  typedef hls::stream<TT_DATA>           TT_STREAM;
};

// Run:
void channelizer_dma_src_wrapper( channelizer_dma_src::TT_DATA mem[channelizer_dma_src::DEPTH*channelizer_dma_src::NSTREAM],
                                  int loop_cnt,
                                  channelizer_dma_src::TT_STREAM sig_o[channelizer_dma_src::NSTREAM] );
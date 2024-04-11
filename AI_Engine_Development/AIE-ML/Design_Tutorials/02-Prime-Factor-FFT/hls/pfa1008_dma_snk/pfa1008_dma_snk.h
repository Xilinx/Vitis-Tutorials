//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <complex>
#include <ap_fixed.h>
#include <hls_stream.h>


namespace pfa1008_dma_snk {
  static constexpr unsigned         NFFT = 1008;
  static constexpr unsigned        DEPTH = 1008; // 4 transforms * 1008-pt / 4-samples
  static constexpr unsigned        NBITS = 128;
  typedef ap_uint<NBITS>                 TT_DATA;
  typedef ap_uint<NBITS/4>               TT_SAMPLE;
  typedef hls::stream<TT_DATA>           TT_STREAM;
  typedef ap_uint<11>                    TT_ADDR;
};

// Run:
// Assume 312.5 MHz clock.
// We have 32-bit I/O's @ 1250 MHz AIE clock or 128-bit I/O's @ 312.5 MHz
void pfa1008_dma_snk_wrapper( pfa1008_dma_snk::TT_DATA mem[pfa1008_dma_snk::DEPTH],
                              int loop_sel,
                              int loop_cnt,
                              pfa1008_dma_snk::TT_STREAM& sig_i );

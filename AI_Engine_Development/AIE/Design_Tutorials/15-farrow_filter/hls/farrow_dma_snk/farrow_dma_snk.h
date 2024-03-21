//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <complex>
#include <ap_fixed.h>
#include <hls_stream.h>

namespace farrow_dma_snk {
  static constexpr unsigned              DEPTH = 1024; // 4 graph iterations * 1024 / 4-samples
  static constexpr unsigned              NBITS = 128;
  typedef ap_uint<NBITS>                 TT_DATA;
  typedef ap_uint<NBITS/4>               TT_SAMPLE;
  typedef hls::stream<TT_DATA>           TT_STREAM;
  typedef ap_uint<10>                    TT_ADDR; // # bits = ceil(log2(DEPTH))
};

// Run:
// Assume 312.5 MHz clock.
// We have 32-bit I/O's @ 1250 MHz AIE clock or 128-bit I/O's @ 312.5 MHz
void farrow_dma_snk_wrapper( farrow_dma_snk::TT_DATA mem[farrow_dma_snk::DEPTH],
                              int loop_sel,
                              int loop_cnt,
                              farrow_dma_snk::TT_STREAM& sig_i );
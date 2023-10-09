//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <complex>
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

namespace dma_stream_src {
  static constexpr int NSTREAM = 7;
  static constexpr int DEPTH   = 512;
  static constexpr int NBITS   = 128;
  typedef ap_uint<NBITS>                      TT_DATA;
  typedef hls::stream<TT_DATA>                TT_STREAM;
};

void dma_stream_src_wrapper( dma_stream_src::TT_DATA mem[dma_stream_src::NSTREAM*dma_stream_src::DEPTH],
                             int loop_cnt,
                             dma_stream_src::TT_STREAM sig_o[dma_stream_src::NSTREAM] );



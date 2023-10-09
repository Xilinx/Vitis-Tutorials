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

namespace dma_stream_snk {
  static constexpr int NSTREAM = 8;
  static constexpr int DEPTH   = 512;
  static constexpr int NBITS   = 128;
  typedef ap_uint<NBITS>                      TT_DATA;
  typedef hls::stream<TT_DATA>                TT_STREAM;
};

void dma_stream_snk_wrapper( dma_stream_snk::TT_DATA mem[dma_stream_snk::NSTREAM*dma_stream_snk::DEPTH],
                             int loop_sel,
                             int loop_cnt,
                             int dft_perm,
                             dma_stream_snk::TT_STREAM sig_i[dma_stream_snk::NSTREAM] );



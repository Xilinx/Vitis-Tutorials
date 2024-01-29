//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <complex>
#include <ap_fixed.h>
#include <hls_stream.h>

namespace pfa1008_permute_o {
  static constexpr unsigned        NFFT = 1008;
  static constexpr unsigned  NBITS_SAMP = 32;
  static constexpr unsigned   NBITS_AXI = 128;
  typedef ap_uint<NBITS_SAMP>            TT_DATA;
  typedef ap_uint<NBITS_AXI>             TT_AXI4;
  typedef hls::stream<TT_AXI4>           TT_STREAM;
  typedef ap_uint<10>                    TT_ADDR; // # bits = ceil(log2(NFFT)) = 10
  typedef ap_uint<8>                     TT_INDEX; // # bits = ceil(log2(NFFT/4)) = 8
};

// Run:
// Assume 312.5 MHz clock.
// We have 32-bit I/O's @ 1250 MHz AIE clock or 128-bit I/O's @ 312.5 MHz
// We get four samples per PL clock representing consecutive time samples as input to DFT-7
// This routine needs to be II=1

void pfa1008_permute_o_wrapper( pfa1008_permute_o::TT_STREAM& sig_i,
                                pfa1008_permute_o::TT_STREAM& sig_o );


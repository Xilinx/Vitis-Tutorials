//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <complex>
#include <ap_fixed.h>
#include <hls_stream.h>

class m16_ssr8_permute_fb_o {
public:
  static constexpr unsigned LATENCY = 2; // To align to 8-cycle AIE filterbank
  static constexpr unsigned M = 16;
  static constexpr unsigned NSTATE = 8; // Set based on P/Q and M
  static constexpr unsigned SSR = 8;
  static constexpr unsigned NBITS_SAMP = 32; // For cint16 samples
  static constexpr unsigned NBITS_AXI = 128;
  typedef ap_uint<NBITS_SAMP>     TT_DATA;
  typedef ap_uint<NBITS_AXI>      TT_AXI4;
  typedef hls::stream<TT_AXI4>    TT_STREAM;

private:
  void apply_permutation( TT_DATA (&data_i)[M], TT_DATA (&data_o)[M], ap_uint<3> state );
  void unpack_samples( TT_STREAM sig_i[SSR], TT_DATA (data_i_0)[M], TT_DATA (data_i_1)[M] );
  void align_latency( TT_DATA (&data_i_0)[M], TT_DATA (&data_i_1)[M],
                      TT_DATA (&data_o_0)[M], TT_DATA (&data_o_1)[M] );
public:
  // Constructor:
  m16_ssr8_permute_fb_o( void );

  // Run:
  void run( TT_STREAM sig_i[SSR], TT_STREAM sig_o[SSR] );
};

using TT_DUT = m16_ssr8_permute_fb_o;

void m16_ssr8_permute_fb_o_wrapper( TT_DUT::TT_STREAM sig_i[TT_DUT::SSR],
                                    TT_DUT::TT_STREAM sig_o[TT_DUT::SSR] );

//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Chandra Sekar Balakrishnan

#pragma once

#include <complex>
#include <ap_fixed.h>
#include <hls_stream.h>

class m16_ssr8_cyclic_shift {
public:
  static constexpr unsigned M = 16;
  static constexpr unsigned NSTATE = 8; //
  static constexpr unsigned SSR_I = 8;  //
  static constexpr unsigned SSR_O = 8;
  static constexpr unsigned FIFO = 4;
  static constexpr unsigned NBITS_SAMP = 32; // For cint16 samples
  static constexpr unsigned NBITS_AXI = 128;
  typedef ap_uint<NBITS_SAMP>       TT_DATA;
  typedef ap_uint<NBITS_AXI>        TT_AXI4;
  typedef hls::stream<TT_AXI4,FIFO> TT_STREAM;

  // Constructor:
  m16_ssr8_cyclic_shift( void );

private:
  void unpack_samples( TT_DATA (&data_i_0)[M], TT_DATA (&data_i_1)[M], TT_STREAM sig_i[SSR_I]);

  void cyclic_shift( TT_DATA (&data_i)[M], TT_DATA (&data_o)[M], ap_uint<3> fsm_state );

  void write_streams( TT_DATA (&data_p_0)[M], TT_DATA (&data_p_1)[M], TT_STREAM sig_o[SSR_O] );

public:
  // Run:
  // Assume 250 MHz clock.
  // We have eight 32-bit I/O's @ 1000 MHz AIE clock or eight 128-bit I/O's @ 250 MHz
  // We get four samples per PL clock representing consecutive time samples
  // This routine needs to be II=1
  void run( TT_STREAM sig_i[SSR_I],
            TT_STREAM sig_o[SSR_O] );
};

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

using TT_DUT = m16_ssr8_cyclic_shift;

void m16_ssr8_cyclic_shift_wrapper( TT_DUT::TT_STREAM sig_i[TT_DUT::SSR_I],
                                    TT_DUT::TT_STREAM sig_o[TT_DUT::SSR_O] );


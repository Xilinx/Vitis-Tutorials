//
// Copyright (C) 2023-2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//
// Author Derek Hagen

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>
#include "kernels.hpp"

// Constructor
template <typename TT_DATA>
my_stream_32b_opt_dm<TT_DATA>::my_stream_32b_opt_dm()
{
  // Drop samples before starting kernel to start fill fifos.
  // This is usually needed when using broadcast to several kernels.
  //int const smpl_drop = 4;
  //for (int i = 0; i < smpl_drop; i++)
  //{
  //  // Drop only on stream port 0
  //  get_ss(0);
  //}
}
// Run:
template <typename TT_DATA>
void __attribute__ ((noinline)) my_stream_32b_opt_dm<TT_DATA>::run(input_stream<TT_DATA> * __restrict sig_i, output_stream<TT_DATA> * __restrict sig_o )
{

  TT_DATA tmp_ping;
  TT_DATA tmp_pong;

  // For comparing scalar stream datamover.
  // Using two 32bit registers as ping pong. Manually unroll two times.
  while (true)
  chess_prepare_for_pipelining
  {
    tmp_ping = readincr(sig_i);
    writeincr(sig_o, tmp_pong);
    tmp_pong = readincr(sig_i);
    writeincr(sig_o, tmp_ping);
  }

}


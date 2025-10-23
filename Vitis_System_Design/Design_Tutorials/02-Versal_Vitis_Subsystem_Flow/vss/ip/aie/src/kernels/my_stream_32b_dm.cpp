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
my_stream_32b_dm<TT_DATA>::my_stream_32b_dm()
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
void __attribute__ ((noinline)) my_stream_32b_dm<TT_DATA>::run(input_stream<TT_DATA> * __restrict sig_i, output_stream<TT_DATA> * __restrict sig_o )
{

  TT_DATA tmp_data;

  // For comparing scalar stream datamover.
  // This will not be able to keep up with data due to register spilling.
  while (true)
  chess_prepare_for_pipelining
  {
    tmp_data = readincr(sig_i);
    writeincr(sig_o, tmp_data);
  }

}


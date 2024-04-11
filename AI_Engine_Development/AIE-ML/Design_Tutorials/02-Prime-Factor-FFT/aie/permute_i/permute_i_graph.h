//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

// Buffer access patterns:
#include "permute_i_bd.h"

using namespace permute_i;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class permute_i_graph : public graph {
public:
  typedef cint16 TT_DATA;
  static constexpr unsigned NFFT_1 = 7;
  static constexpr unsigned  NFFT7 = 7;
  static constexpr unsigned  NFFT9 = 9;
  static constexpr unsigned NFFT16 = 16;

  port<input>  sig_i;
  port<output> sig_o;

  // Declare Mem Tile buffer:
  shared_buffer<TT_DATA> MT_buff;

  // Constructor:
  permute_i_graph( void )
  {
    // Create buffer:
    MT_buff = shared_buffer<TT_DATA>::create({8,16,16},1,1);
    write_access(MT_buff.in[0]) = tiling(write_bd);
    read_access(MT_buff.out[0]) = tiling(read_bd);
    repetition_count(MT_buff) = 1;
    num_buffers(MT_buff) = 2;

    connect<>( sig_i,          MT_buff.in[0] );
    connect<>( MT_buff.out[0], sig_o         );
  }
};

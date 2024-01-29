//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class transpose1_graph : public graph {
public:
  typedef cint16 TT_DATA;

  port<input>  sig_i;
  port<output> sig_o;

  // Declare Mem Tile buffer:
  shared_buffer<TT_DATA> MT_buff;

  // Constructor:
  transpose1_graph( void )
  {
    // Write samples in linear transform order (3D cube)
    // --> incoming data from DFT-9 transforms in 2nd dimension
    tiling_parameters write_bd = {
      .buffer_dimension = {8,16,16},
      .tiling_dimension = {1,1,1},
      .offset = {0,0,0},
      .tile_traversal = {{.dimension=1, .stride=1, .wrap=9},
                         {.dimension=2, .stride=1, .wrap=16},
                         {.dimension=0, .stride=1, .wrap=7}} };

    // Read samples in permuted order (3D cube)
    // --> outgoing data for DFT-16 transforms in 3rd dimension
    tiling_parameters read_bd = {
      .buffer_dimension = {8,16,16},
      .tiling_dimension = {1,1,1},
      .offset = {0,0,0},
      .tile_traversal = {{.dimension=2, .stride=1, .wrap=16},
                         {.dimension=1, .stride=1, .wrap=9},
                         {.dimension=0, .stride=1, .wrap=7}} };

    // Create buffer:
    MT_buff = shared_buffer<TT_DATA>::create({8,16,16},1,1);
    write_access(MT_buff.in[0]) = tiling(write_bd);
    read_access(MT_buff.out[0]) = tiling(read_bd);
    num_buffers(MT_buff) = 2;
    repetition_count(MT_buff) = 8;

    connect<>( sig_i,          MT_buff.in[0] );
    connect<>( MT_buff.out[0], sig_o         );
  }
};

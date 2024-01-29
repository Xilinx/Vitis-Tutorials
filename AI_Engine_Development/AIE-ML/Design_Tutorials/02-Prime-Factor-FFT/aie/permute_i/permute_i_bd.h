//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>

using namespace adf;

namespace permute_i {
  typedef cint16 TT_DATA;
  static constexpr unsigned  NFFT7 = 7;
  static constexpr unsigned  NFFT9 = 9;
  static constexpr unsigned NFFT16 = 16;
  static constexpr unsigned NFFT_ALL = NFFT7 * NFFT9 * NFFT16;

  // Input permutation tiling parameters:

  // Write samples in linear transform order (3D cube)
  tiling_parameters write_bd = {
    .buffer_dimension = {8,16,16},
    .tiling_dimension = {1,1,1},
    .offset = {0,0,0} };

  // Read samples in permuted order (3D cube)
  tiling_parameters read_bd = {
    .buffer_dimension = {8,16,16},
    .tiling_dimension = {1,1,1},
    .offset = {0,0,0},
    .tile_traversal = {{.dimension=0, .stride=144, .wrap=7},
                       {.dimension=1, .stride=112, .wrap=9},
                       {.dimension=2, .stride=63,  .wrap=16}} };
};

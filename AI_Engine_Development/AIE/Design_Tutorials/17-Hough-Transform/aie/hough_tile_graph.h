//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include "hough_tile.h"
#include "hough_tile_sincos_lut.h"

using namespace adf;
using namespace hough;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

// R  --> # of row pixels in full image
// C  --> # of col pixels in full image
// NR --> # of AIE row tiles in rectangular array
// NC --> # of AIE col tiles in rectangular array
template<int R, int C, int NR, int NC, int RHO_MAX>
class hough_tile_graph : public graph {
private:
  static constexpr int RR = R;
  static constexpr int CC = C;
  static constexpr int NT = NR * NC;     // # of AIE tiles in design

  static constexpr int RHO_NUM   = 2*RHO_MAX+1;    // Number of bins in 'rho'
  static constexpr int RHO_DEPTH = 16*aie_ceil(static_cast<float>(RHO_NUM)/16); // Force multiple of 16
  static constexpr int COUNT_NUM = RHO_DEPTH*THETA_NUM;

  static_assert( CC % 8 == 0, "Image Column Dimension must be divisible by 8" ); // Based on for-loop construction

  kernel k_hough[NT];

public:
  std::array<port< input>,NT> sig_i;
  std::array<port<output>,NT> sig_o;

  using TT_DUT = hough_tile<RR,CC,RHO_MAX>;

  // Constructor:
  hough_tile_graph( std::vector<int>& LOC_X, std::vector<int>& LOC_Y )
  {
    for (int tt=0; tt < NT; tt++) {
      k_hough[tt] = kernel::create_object<TT_DUT>( std::vector<TT_THETA>(theta_init[tt]), std::vector<TT_COUNT>(COUNT_NUM),
                                                   std::vector<TT_COUNT>(THETA_NUM*CC) );
      source(k_hough[tt]) = "hough_tile.cpp";
      connect<stream,stream>( sig_i[tt],          k_hough[tt].in[0] );
      connect<stream,stream>( k_hough[tt].out[0], sig_o[tt] );
      runtime<ratio>(k_hough[tt]) = 0.9;
      location<kernel>(k_hough[tt]) = tile( LOC_X[tt], LOC_Y[tt] );
    } // tt
  }
};



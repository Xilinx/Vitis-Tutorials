//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "hough_tile.h"
#include "hough_tile_shared.h"

namespace hough {

  // ------------------------------------------------------------
  // Constructor
  // ------------------------------------------------------------

  template<int RR, int CC, int RHO_MAX>
  hough_tile<RR,CC,RHO_MAX>::hough_tile( TT_THETA (&SINCOS_i)[THETA_NUM], TT_COUNT (&COUNTS_i)[COUNT_NUM],
                                         TT_COUNT (&SCRATCH_i)[THETA_NUM*CC] )
    : SINCOS(SINCOS_i), COUNTS(COUNTS_i), SCRATCH(SCRATCH_i)
  {
    aie::set_rounding(aie::rounding_mode::symmetric_inf);
    aie::set_saturation(aie::saturation_mode::saturate);
  }

  // ------------------------------------------------------------
  // Run
  // ------------------------------------------------------------

  template<int RR, int CC, int RHO_MAX>
  void hough_tile<RR,CC,RHO_MAX>::run( input_stream<TT_PIXEL>* __restrict pixel_i,
                                       output_stream<TT_COUNT>* __restrict sig_o )
  {
    // Each kernel invocation performs one complete Hough Transform:
    clear_counts<RHO_DEPTH>( COUNTS );

    // Perform 'theta' computations:
    theta_compute<RR,CC,RHO_MAX,COUNT_NUM,THETA_NUM>( SINCOS, COUNTS, SCRATCH, pixel_i );

    // Stream out Hough transform:
    auto it = aie::begin_vector<8>( COUNTS );
    for (int ss=0; ss < COUNT_NUM/8; ss++) // Note: this is actual size not padded size
      chess_prepare_for_pipelining
      chess_loop_range(64,)
    {
      writeincr( sig_o, *it++ );
    } // ss
  }

}; // hough::

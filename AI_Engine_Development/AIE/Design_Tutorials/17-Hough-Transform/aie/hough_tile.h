//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>
#include "aie_helper.h"


using namespace adf;

namespace hough {

  typedef int16                    TT_PIXEL;
  typedef cint16                   TT_THETA;
  typedef int16                    TT_COUNT;
  typedef acc48                    TT_ACC;
  static constexpr int SHIFT = 15;             // Assumes 'theta' uses ap_fixed<16,1> representation
  static constexpr int THETA_NUM = 4;


  template<int RR, int CC, int RHO_MAX>
  class hough_tile {
  public:
    static constexpr int   RHO_NUM = 2*RHO_MAX+1;    // Number of bins in 'rho'
    static constexpr int RHO_DEPTH = 16*aie_ceil(static_cast<float>(RHO_NUM)/16); // Force multiple of 16
    static constexpr int COUNT_NUM = RHO_DEPTH*THETA_NUM;
  private:
    // Data Tables:
    alignas(32) TT_THETA (&SINCOS)[THETA_NUM];           // Stores sin() and cos() values for transform
    alignas(32) TT_COUNT (&COUNTS)[COUNT_NUM];           // Stores final counts
    alignas(32) TT_COUNT (&SCRATCH)[THETA_NUM*CC];       // Scratch memory
  public:

    // Constructor:
    hough_tile( TT_THETA (&SINCOS_i)[THETA_NUM], TT_COUNT (&COUNTS_i)[COUNT_NUM], TT_COUNT (&SCRATCH_i)[THETA_NUM*CC] );
    // Run:
    void run( input_stream<TT_PIXEL>* p_i, output_stream<TT_COUNT>* sig_o );

    // Register kernels:
    static void registerKernelClass( void )
    {
      REGISTER_FUNCTION( hough_tile::run );
      REGISTER_PARAMETER( SINCOS );
      REGISTER_PARAMETER( COUNTS );
      REGISTER_PARAMETER( SCRATCH );
    }
  };

}; // hough::


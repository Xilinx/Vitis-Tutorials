//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

template<unsigned REPEAT,unsigned NSAMP>
class interp1 {
public:
  static constexpr unsigned TP_COARSE_BITS = 11;
  static constexpr unsigned           NLUT = (1<<(1+TP_COARSE_BITS));
  static constexpr unsigned   TP_FINE_BITS = 12;

  alignas(16) float (&lut_copy)[NLUT];
  alignas(16) float  (&m_slopeBuff)[NSAMP];
  alignas(16) float  (&m_offsetBuff)[NSAMP];

  unsigned state;
  unsigned count;

  interp1( float (&lut_copy_i)[NLUT], float (&m_slopeBuff_i)[NSAMP], float (&m_offsetBuff_i)[NSAMP] );

  void copy_lut(input_async_buffer<float>& lut_i);

  void compute_workload(input_async_buffer<float>& lut_i, input_buffer<float>& domain_i,
                        output_buffer<float>& interp_o);

  void run( input_async_buffer<float>& lut_i, input_buffer<float>& domain_i, output_buffer<float>& interp_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( interp1::run );
    REGISTER_PARAMETER( lut_copy );
    REGISTER_PARAMETER( m_slopeBuff );
    REGISTER_PARAMETER( m_offsetBuff );
  }
};


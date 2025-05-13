//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class ifft2k_async {
public:
  static constexpr unsigned NSAMP = 2048;
  static constexpr float    SCALE = 4.882812500e-04; // 1/NSAMP scale factor

  alignas(16) cfloat (&tw1024)[1024];
  alignas(16) cfloat (&tw512)[512];

#include "ifft2k_async_twiddles_internal.h"

  ifft2k_async( cfloat (&tw1024_i)[1024], cfloat (&tw512_i)[512] );

  void run( input_buffer<cfloat>& sig_i, output_buffer<cfloat>& sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ifft2k_async::run );
    REGISTER_PARAMETER( tw1024 );
    REGISTER_PARAMETER( tw512 );
  }
};


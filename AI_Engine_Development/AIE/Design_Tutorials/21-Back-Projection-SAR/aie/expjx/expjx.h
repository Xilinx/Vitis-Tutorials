//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

template<unsigned NSAMP>
class expjx {
public:

  expjx(void);

  // Run:
  void run( input_buffer<float>& cos_i, input_buffer<float>& sin_i, output_buffer<cfloat>& exp_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( expjx<NSAMP>::run );
  }
};


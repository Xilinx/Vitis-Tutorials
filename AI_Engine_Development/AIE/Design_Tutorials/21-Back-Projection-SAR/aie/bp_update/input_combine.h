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
class input_combine {
public:

  input_combine(void);

  // Run:
  void merge( input_buffer <float>& real_i,
              input_buffer <float>& imag_i,
              output_buffer<cfloat>& complex_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( input_combine::merge );
  }
};


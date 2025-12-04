//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

template<unsigned NSAMP>
class selu {
public:

  const bfloat16   exp_S =  185.0f;
  const float      exp_B =  16256.0f;
  const bfloat16   Cpos  =  1.0507f;
  const bfloat16   Cneg  =  1.7581f;

  alignas(64) bfloat16 SCRATCH_POS[NSAMP];
  alignas(64) bfloat16 SCRATCH_NEG[NSAMP];

  // Constructor:
  selu( void );

  // Run:
  void run( input_buffer<bfloat16>& sig_i, output_buffer<bfloat16>& sig_o);

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( selu::run );
  }
};


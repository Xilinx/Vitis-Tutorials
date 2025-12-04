//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

template <unsigned NSAMP, unsigned NNODES, unsigned NSPLIT>
class max_pool1d_template {
public:
  typedef bfloat16  TT_DATA;
private:

public:
  // Constructor:
  max_pool1d_template( void );

  // Run:
  void run( input_buffer<TT_DATA>&  __restrict data_i,
            output_buffer<TT_DATA>& __restrict data_o);

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( max_pool1d_template::run );
  }
};


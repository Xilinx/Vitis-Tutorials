//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

template<unsigned NSAMP, unsigned NNODES, unsigned NWEIGHTS, unsigned NBIASES>
class dense_w16 {
public:
  typedef bfloat16  TT_DATA;
  
  // SeLu Parameters
  const TT_DATA   exp_S =  185.0f;
  const float     exp_B =  16256.0f;
  const TT_DATA   Cpos  =  1.0507f;
  const TT_DATA   Cneg  =  1.7581f;

private:

public:
 // SeLu
  aie::vector<TT_DATA,64> SeLu( aie::vector<TT_DATA,64> data_i);

  // Constructor:
  dense_w16( void );

  // Run:
  void run( input_buffer<TT_DATA>&  __restrict data_i,
            const TT_DATA              (&weights_0)[NWEIGHTS/2],
            const TT_DATA              (&weights_1)[NWEIGHTS/2],
            const TT_DATA              (&biases)[NBIASES],
            output_buffer<TT_DATA>& __restrict data_o);

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( dense_w16::run );
  }
};


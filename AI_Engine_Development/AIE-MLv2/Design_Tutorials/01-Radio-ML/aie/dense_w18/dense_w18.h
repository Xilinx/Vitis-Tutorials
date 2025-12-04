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
class dense_w18 {
public:
  typedef bfloat16  TT_DATA;
  
  // softmax Parameters
  const TT_DATA   exp_S =  185.0f;
  const float     exp_B =  16256.0f;

private:

public:

  // softmax
  void softmax( aie::vector<TT_DATA,32> vecA, output_buffer<TT_DATA>& data_o);

  // Constructor:
  dense_w18( void );

  // Run:
  void run( input_buffer<TT_DATA>&  __restrict data_i,
            const TT_DATA              (&weights)[NWEIGHTS],
            const TT_DATA              (&biases)[NNODES],
            output_buffer<TT_DATA>& __restrict data_o);

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( dense_w18::run );
  }
};


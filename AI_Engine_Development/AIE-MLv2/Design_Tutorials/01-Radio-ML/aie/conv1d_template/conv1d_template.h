//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

template<unsigned NSAMP, unsigned NNODES, unsigned KERNEL_SIZE, unsigned NWEIGHTS, unsigned NSPLIT>
class conv1d_template {
public:
  typedef bfloat16  TT_DATA;
private:

public:
  // Constructor:
  conv1d_template( void );

  // Run:
  void run( input_buffer<TT_DATA>&        __restrict data_i,
            const TT_DATA              (&weights)[NWEIGHTS],
            const TT_DATA              (&biases)[NNODES],
            output_buffer<TT_DATA>&       __restrict data_o);

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( conv1d_template::run );
  }
};

// ------------------------------------------------------------
// Compute Patterns
// ------------------------------------------------------------

#define COMPUTE_PATTERN(DATA0,DATA1,DATA2,DATA3,DATA4,OFFSET)                      \
  weights_reg = *p_weights++;                                                      \
  data_reg0  = aie::shuffle_down_fill(DATA0,DATA1,OFFSET);                         \
  acc_reg0 = mac_4x8_8x8(data_reg0,weights_reg,acc_reg0);                          \
  data_reg1  = aie::shuffle_down_fill(DATA1,DATA2,OFFSET);                         \
  acc_reg1 = mac_4x8_8x8(data_reg1,weights_reg,acc_reg1);                          \
  data_reg2  = aie::shuffle_down_fill(DATA2,DATA3,OFFSET);                         \
  acc_reg2 = mac_4x8_8x8(data_reg2,weights_reg,acc_reg2);                          \
  data_reg3  = aie::shuffle_down_fill(DATA3,DATA4,OFFSET);                         \
  acc_reg3 = mac_4x8_8x8(data_reg3,weights_reg,acc_reg3);                      

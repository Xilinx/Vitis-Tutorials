//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

class sortcpp_fp16 {
public:
  static constexpr unsigned SIZE = 16;

  // Constructor:
  sortcpp_fp16( void );

  // Run:
  void run( input_stream<float>* sig_i, output_stream<float>* sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( sortcpp_fp16::run );
  }
};


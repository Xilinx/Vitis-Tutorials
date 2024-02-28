//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

class sortcpp_fp1024 {
public:
  static constexpr unsigned SIZE = 1024;

  alignas(16) std::array<float,SIZE> data;

  // Constructor:
  sortcpp_fp1024( void );

  void load_data( input_stream<float>* sig_i);
  void store_data( output_stream<float>* sig_o );

  // Run:
  void run( input_stream<float>* sig_i, output_stream<float>* sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( sortcpp_fp1024::run );
    REGISTER_PARAMETER( data );
  }
};


//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

class bitonic_fp16 {
public:
  void stage0_api       ( aie::vector<float,16>& vec );
  void stage0_intrinsic ( aie::vector<float,16>& vec );
  void stage1 ( aie::vector<float,16>& vec );
  void stage1a( aie::vector<float,16>& vec );
  void stage2 ( aie::vector<float,16>& vec );
  void stage2a( aie::vector<float,16>& vec );
  void stage2b( aie::vector<float,16>& vec );
  void stage3 ( aie::vector<float,16>& vec );
  void stage3a( aie::vector<float,16>& vec );
  void stage3b( aie::vector<float,16>& vec );

  // Constructor:
  bitonic_fp16( void );

  // Run:
  void run( input_stream<float>* sig_i, output_stream<float>* sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( bitonic_fp16::run );
  }
};


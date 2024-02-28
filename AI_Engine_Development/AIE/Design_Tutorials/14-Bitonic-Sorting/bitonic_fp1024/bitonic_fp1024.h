//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

class bitonic_fp1024 {
public:
  static constexpr unsigned SIZE = 1024;
  static constexpr unsigned NSTAGES = 10;

  alignas(16) float (&data)[SIZE];

  // Constructor:
  bitonic_fp1024( float (&data_i)[SIZE] );

  void load_data( input_stream<float>* sig_i);
  void store_data( output_stream<float>* sig_o );

  template<unsigned GROUP,unsigned SPAN,unsigned ITER> void stageY(void);
  template<unsigned GROUP,unsigned SPAN,unsigned ITER> void stageB(void);

  void stage0_all(void);
  void stage1_all(void);
  void stage1a_all(void);
  void stage2_all(void);
  void stage2a_all(void);
  void stage2b_all(void);
  void stage3_all(void);
  void stage3a_all(void);
  void stage3b_all(void);
  void stage4_all(void);
  void stage5_all(void);
  void stage6_all(void);
  void stage7_all(void);
  void stage8_all(void);
  void stage9_all(void);

  // Run:
  void run( input_stream<float>* sig_i, output_stream<float>* sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( bitonic_fp1024::run );
    REGISTER_PARAMETER( data );
  }
};


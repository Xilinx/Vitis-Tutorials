//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class dummy_async {
public:
  static constexpr unsigned NSAMP = 4096;

  dummy_async(void);

  void run( input_buffer<float>& sig_i, output_async_buffer<float>& sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( dummy_async::run );
  }
};


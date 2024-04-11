//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "sincos_test.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class sincos_test_graph : public graph {
public:
  kernel k_kernel;

  port<output> phase_o;
  port<output> twid_o;

  // Constructor:
  sincos_test_graph( void )
  {
    k_kernel = kernel::create_object<sincos_test>();
    source(k_kernel) = "sincos_test.cpp";
    runtime<ratio>(k_kernel) = 0.9;
    connect<>( k_kernel.out[0], phase_o );
    connect<>( k_kernel.out[1], twid_o );
  }
};


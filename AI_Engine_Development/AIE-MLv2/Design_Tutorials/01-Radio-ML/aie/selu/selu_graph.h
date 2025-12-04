//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "selu.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<unsigned NSAMP>
class selu_graph : public graph {
public:
  // Declare Kernels
  kernel kk;

  // Declare IOs
  port<input>                 sig_i;
  port<output>                sig_o;

  selu_graph( void )
  {
    kk = kernel::create_object<selu<NSAMP>>();
    source(kk) = "selu.cpp";
    runtime<ratio>(kk) = 0.9;
    connect<>( sig_i,     kk.in[0] );   dimensions(kk.in[0])  = { NSAMP };
    connect<>( kk.out[0], sig_o    );   dimensions(kk.out[0]) = { NSAMP };
  }
};


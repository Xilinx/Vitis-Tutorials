//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "expjx.h"
#include "cos_lib_graph.h"
#include "sin_lib_graph.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
class expjx_graph : public graph {
public:
  kernel                      kk;
  port<input>                 sig_i;
  port<output>                sig_o;
  cos_lib_graph<REPEAT,NSAMP> dut_cos;
  sin_lib_graph<REPEAT,NSAMP> dut_sin;

  // Constructor:
  expjx_graph()
  {
    kk = kernel::create_object<expjx<NSAMP> >();
    source(kk) = "expjx.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = REPEAT;

    connect<>( sig_i, dut_cos.sig_i );
    connect<>( sig_i, dut_sin.sig_i );
    connect<>( dut_cos.sig_o, kk.in[0] );  dimensions(kk.in[0])  = { NSAMP };
    connect<>( dut_sin.sig_o, kk.in[1] );  dimensions(kk.in[1])  = { NSAMP };
    connect<>( kk.out[0], sig_o );         dimensions(kk.out[0]) = { NSAMP };
  }
};



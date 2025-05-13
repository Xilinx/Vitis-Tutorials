//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "interp1.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
class interp1_graph : public graph {
public:
  kernel       kk;
  port<input>  lut_i;
  port<input>  domain_i;
  port<output> interp_o;

  interp1_graph( void )
  {
    kk = kernel::create_object<interp1<REPEAT,NSAMP> >(std::vector<float>(interp1<REPEAT,NSAMP>::NLUT,0),
                                                 std::vector<float>(NSAMP,0),
                                                 std::vector<float>(NSAMP,0));
    source(kk) = "interp1.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = REPEAT;
    single_buffer(kk.in[0]);

    connect<>( lut_i,      kk.in[0] );  dimensions(kk.in[0])  = { interp1<REPEAT,NSAMP>::NLUT };
    connect<>( domain_i,   kk.in[1] );  dimensions(kk.in[1])  = { NSAMP };
    connect<>( kk.out[0],  interp_o );  dimensions(kk.out[0]) = { NSAMP };
  }
};



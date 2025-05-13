//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "dR_comp.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
class dR_comp_graph : public graph {
public:
  kernel                     kk;
  port<input>                sig_i;
  port<input>                R0_range_i;
  port<output>               domain_o;
  port<output>               theta_o;

  dR_comp_graph( void )
  {
    kk = kernel::create_object<dR_comp<REPEAT,NSAMP> >();
    source(kk) = "dR_comp.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = REPEAT;

    connect<parameter>(R0_range_i,async(kk.in[1]));
    connect<>( sig_i,      kk.in[0] );  dimensions(kk.in[0])  = { NSAMP };
    connect<>( kk.out[0],  domain_o );  dimensions(kk.out[0]) = { NSAMP };
    connect<>( kk.out[1],  theta_o  );  dimensions(kk.out[1]) = { NSAMP };
  }
};



//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "bp_update.h"
#include "input_combine.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
class bp_update_graph : public graph {
public:
  kernel       kk;
  kernel       cc;
  port<input>  image_i;
  port<input>  interp_real_i;
  port<input>  interp_imag_i;
  port<input>  phase_i;
  port<output> image_o;

  bp_update_graph(void)
  {
    kk = kernel::create_object<bp_update<NSAMP> >();
    source(kk) = "bp_update.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = REPEAT;
    dimensions(kk.in[0])  = { NSAMP };
    dimensions(kk.in[1])  = { NSAMP };
    dimensions(kk.in[2])  = { NSAMP };
    dimensions(kk.out[0]) = { NSAMP };

    cc = kernel::create_object<input_combine<NSAMP> >();
    source(cc) = "input_combine.cpp";
    runtime<ratio>(cc) = 0.9;
    repetition_count(cc) = REPEAT;
    dimensions(cc.in[0])  = { NSAMP };
    dimensions(cc.in[1])  = { NSAMP };
    dimensions(cc.out[0]) = { NSAMP };

    connect<>( interp_real_i, cc.in[0] );
    connect<>( interp_imag_i, cc.in[1] );

    connect<>( image_i,       kk.in[0] );
    connect<>( cc.out[0],     kk.in[1] );
    connect<>( phase_i,       kk.in[2] );
    connect<>( kk.out[0],     image_o  );
  }
};



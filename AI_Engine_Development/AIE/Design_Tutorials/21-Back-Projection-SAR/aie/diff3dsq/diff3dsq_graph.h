//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "diff3dsq.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
class diff3dsq_graph : public graph {
public:
  kernel       kk;
  port<input>  coord_i;
  port<input>  sig_i;
  port<output> sig_o;

  diff3dsq_graph(void)
  {
    kk = kernel::create_object<diff3dsq<REPEAT,NSAMP> >();
    source(kk) = "diff3dsq.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = REPEAT;

    connect<parameter>(coord_i,async(kk.in[1]));
    connect<>( sig_i,      kk.in[0] );  dimensions(kk.in[0])  = { 3*NSAMP };
    connect<>( kk.out[0],  sig_o    );  dimensions(kk.out[0]) = {   NSAMP };
  }
};



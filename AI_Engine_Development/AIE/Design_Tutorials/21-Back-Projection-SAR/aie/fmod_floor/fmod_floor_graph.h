//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "fmod_floor.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
class fmod_floor_graph : public graph {
public:
  kernel         kk;
  port<input>    sig_i;
  port<output>   sig_o;

  fmod_floor_graph( void )
  {
    kk = kernel::create_object<fmod_floor<NSAMP> >();
    source(kk) = "fmod_floor.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = REPEAT;

    connect<>( sig_i,      kk.in[0] );  dimensions(kk.in[0])  = { NSAMP };
    connect<>( kk.out[0],  sig_o    );  dimensions(kk.out[0]) = { NSAMP };
  }
};



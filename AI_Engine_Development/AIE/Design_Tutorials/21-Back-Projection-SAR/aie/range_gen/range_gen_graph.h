//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "range_gen.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP,unsigned ID>
class range_gen_graph : public graph {
public:
  kernel       kk;
  port<output> sig_o;

  range_gen_graph(void)
  {
    kk = kernel::create_object<range_gen<REPEAT,NSAMP,ID> >();
    source(kk) = "range_gen.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = 1; // One kernel invocation per radar pulse

    connect<>( kk.out[0], sig_o );
  }
};



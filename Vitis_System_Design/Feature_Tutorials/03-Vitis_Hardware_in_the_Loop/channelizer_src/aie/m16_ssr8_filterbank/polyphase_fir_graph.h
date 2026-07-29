//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "polyphase_fir.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<unsigned PATTERN,unsigned NSAMP>
class polyphase_fir_graph : public graph {
private:
  kernel k_fir;

public:
  std::array<port<input>,1>  sig_i;
  std::array<port<output>,1> sig_o;

  using TT_KERNEL = polyphase_fir<PATTERN,NSAMP>;

  // Constructor:
  polyphase_fir_graph( std::vector<typename TT_KERNEL::TT_COEFF> tapsA_i,
                       std::vector<typename TT_KERNEL::TT_COEFF> tapsB_i )
  {
    k_fir = kernel::create_object<TT_KERNEL>( tapsA_i, tapsB_i );

    source(k_fir) = "polyphase_fir.cpp";
    runtime<ratio>(k_fir) = 0.9;

    connect<stream,stream>( sig_i[0],     k_fir.in[0] );
    connect<stream,stream>( k_fir.out[0], sig_o[0]    );
  }
};


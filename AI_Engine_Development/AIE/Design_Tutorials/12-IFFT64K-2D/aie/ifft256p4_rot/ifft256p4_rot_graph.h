//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "ifft256p4_rot.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template <unsigned NSTREAM, unsigned PHASE>
class ifft256p4_rot_graph : public graph {
public:
  kernel k_kernel;

  using TTK = ifft256p4_rot<NSTREAM,PHASE>;

  port<input>  sig_i;
  port<output> sig_o;

  // Constructor:
  ifft256p4_rot_graph( void )
  {
    k_kernel =
      kernel::create_object<TTK>(std::vector<typename TTK::TT_DATA>(TTK::NFFT_1D));
    source(k_kernel) = "ifft256p4_rot.cpp";
    runtime<ratio>(k_kernel) = 0.9;

    connect<>( sig_i, k_kernel.in[0] );
    connect<>( k_kernel.out[0], sig_o );
  }
};


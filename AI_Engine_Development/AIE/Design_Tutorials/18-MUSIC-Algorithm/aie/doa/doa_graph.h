//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

#include "music_parameters.h"
#include "doa.h"

using namespace adf;

// ------------------------------------------------------------
// Block Level Graph
// ------------------------------------------------------------

class doa_graph : public graph {
public:
  template <unsigned START, unsigned END>
  void doa_kernel_create(void) {
    if constexpr (START <= END) {
        doa_kernel[START]  = adf::kernel::create_object<DOA<COL, 1, START * DOA_INTERVAL_LEN,  (START + 1) * DOA_INTERVAL_LEN>>();
        doa_kernel_create<START + 1, END>();
      }
    }
  adf::kernel doa_kernel[NUM_DOA_KERNELS];

  input_port  sig_i;
  output_port sig_o;

  // Constructor:
  doa_graph( void )
  {
    doa_kernel[0] = adf::kernel::create_object<DOA<COL, 0,  0 * DOA_INTERVAL_LEN,  1 * DOA_INTERVAL_LEN>>();
    doa_kernel_create<1, 63>();
    for (unsigned i = 0; i < NUM_DOA_KERNELS; ++i) {
      adf::runtime<ratio>(doa_kernel[i]) = 0.9;
      adf::source(doa_kernel[i]) = "doa.cpp";
    }
    // Connections:
    adf::connect(sig_i,doa_kernel[0].in[0]);
    for (unsigned i = 0; i < NUM_DOA_KERNELS - 1; ++i)
      adf::connect<>(doa_kernel[i].out[0], doa_kernel[i+1].in[0]);
    adf::connect<>(doa_kernel[NUM_DOA_KERNELS-1].out[0], sig_o);
    // Dimensions:
    adf::dimensions(doa_kernel[0].in[0])  = {N * N};
    for (unsigned i = 0; i < NUM_DOA_KERNELS - 1; ++i) {
      adf::dimensions(doa_kernel[i].out[0])  = {N * N + NUM_POINTS};  // noiseSS + pseudo spectrum
      adf::dimensions(doa_kernel[i+1].in[0]) = {N * N + NUM_POINTS};
    }
    adf::dimensions(doa_kernel[NUM_DOA_KERNELS-1].out[0]) = {N * N + NUM_POINTS};
  }
};



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
#include "finder.h"

using namespace adf;

// ------------------------------------------------------------
// Block Level Graph
// ------------------------------------------------------------

class finder_graph : public graph {
public:
    template <unsigned START, unsigned END>
    void finder_kernel_create(void) {
      if constexpr (START <= END) {
        finder_kernel[START]  = adf::kernel::create_object<Finder<2 * START,  2 * START + 1>>();
        finder_kernel_create<START + 1, END>();
      }
    }
  adf::kernel finder_kernel[NUM_FINDER_KERNELS];
  input_port  sig_i;
  output_port sig_o;

  // Constructor:
  finder_graph( void )
  {
    finder_kernel_create<0, 15>();
    for (unsigned i = 0; i < NUM_FINDER_KERNELS; ++i) {
      adf::runtime<ratio>(finder_kernel[i]) = 0.9;
      adf::source(finder_kernel[i]) = "finder.cpp";
    }
    // Connections:
    adf::connect<>(sig_i, finder_kernel[0].in[0]);
    for (unsigned i = 0; i < NUM_FINDER_KERNELS - 1; ++i)
      adf::connect<>(finder_kernel[i].out[0], finder_kernel[i+1].in[0]);
    adf::connect<>(finder_kernel[NUM_FINDER_KERNELS-1].out[0], sig_o);
    // Dimensions:
    for (unsigned i = 0; i < NUM_FINDER_KERNELS; ++i) {
      adf::dimensions(finder_kernel[i].in[0])  = {NUM_POINTS + NUM_REGIONS};
      adf::dimensions(finder_kernel[i].out[0]) = {NUM_POINTS + NUM_REGIONS};
    }
  }
};



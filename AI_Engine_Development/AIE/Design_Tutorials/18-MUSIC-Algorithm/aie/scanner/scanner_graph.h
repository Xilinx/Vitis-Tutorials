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
#include "scanner.h"

using namespace adf;

// ------------------------------------------------------------
// Block Level Graph
// ------------------------------------------------------------

class scanner_graph : public graph {
public:
  adf::kernel      scanner_kernel[NUM_SCANNER_KERNELS];
  input_port  sig_i;
  output_port sig_o;

  // Constructor:
  scanner_graph( void )
  {
    scanner_kernel[0] = adf::kernel::create_object<Scanner<0,               NUM_REGIONS / 2>>();
    scanner_kernel[1] = adf::kernel::create_object<Scanner<NUM_REGIONS / 2, NUM_REGIONS    >>();
    for (unsigned i = 0; i < NUM_SCANNER_KERNELS; ++i) {
      adf::runtime<ratio>(scanner_kernel[i]) = 0.9;
      adf::source(scanner_kernel[i]) = "scanner.cpp";
    }
    // Connections:
    adf::connect<>(sig_i, scanner_kernel[0].in[0]);
    adf::connect<>(scanner_kernel[0].out[0], scanner_kernel[1].in[0]);
    adf::connect<>(scanner_kernel[1].out[0], sig_o);

    // Dimensions:
    adf::dimensions(scanner_kernel[0].in[0])  = {N * N      + NUM_POINTS};
    adf::dimensions(scanner_kernel[0].out[0]) = {NUM_POINTS + NUM_REGIONS / 2};  // half of tags for the first scanner kernel
    adf::dimensions(scanner_kernel[1].in[0])  = {NUM_POINTS + NUM_REGIONS / 2};  // half of tags for the first scanner kernel
    adf::dimensions(scanner_kernel[1].out[0]) = {NUM_POINTS + NUM_REGIONS};
  }
};



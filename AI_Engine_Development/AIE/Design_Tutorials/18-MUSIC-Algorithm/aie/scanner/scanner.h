//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#pragma once

#include "adf.h"
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>
#include "music_parameters.h"

template<unsigned START, unsigned END>
class Scanner {
  public:
  Scanner(void) { };
    void run(adf::input_buffer< cfloat, adf::extents<adf::inherited_extent>> & __restrict in,
             adf::output_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict out);
    static void registerKernelClass(void) { REGISTER_FUNCTION(Scanner::run); };

  private:
    alignas(32) aie::vector<cfloat, N>  noiseSS;  // placeholder only and not used
    alignas(32) aie::vector<cfloat, N>  Pm_null_cfloat[NUM_REGIONS];
};

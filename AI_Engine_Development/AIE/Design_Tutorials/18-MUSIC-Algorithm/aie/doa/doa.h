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
#include "aie_api/utils.hpp"
#include "music_parameters.h"

template<unsigned N, unsigned INPUT_PM_NULL_FLAG, unsigned START, unsigned END>
class DOA {
  public:
    DOA(void);
    void run(adf::input_buffer< cfloat, adf::extents<adf::inherited_extent>> & __restrict in,
             adf::output_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict out);
    static void registerKernelClass(void) { REGISTER_FUNCTION(DOA::run); };

  private:
    alignas(32) aie::vector<cfloat, N>                 noiseSS[N];  // noise sub-space: N - nSources
    alignas(32) aie::vector<cfloat, N>                 steeringVector[DOA_INTERVAL_LEN];
    alignas(32) aie::vector<cfloat, DOA_INTERVAL_LEN>  Pm_null[NUM_DOA_INTERVALS];
};

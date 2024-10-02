//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>
#include "music_parameters.h"

template<unsigned COL_NORM, unsigned IN_TOTAL, unsigned OUT_START, unsigned OUT_END>
class QRD_Norm {
  public:
    QRD_Norm(void) { };
    void run(adf::input_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict in, adf::output_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict out);
    static void registerKernelClass(void) { REGISTER_FUNCTION(QRD_Norm::run); };

  private:
    alignas(32) aie::vector<cfloat,SEGMENT_SIZE> Q[TOTAL_NUM_SEGMENTS];
    alignas(32) aie::vector<cfloat,COL> R[COL];
};

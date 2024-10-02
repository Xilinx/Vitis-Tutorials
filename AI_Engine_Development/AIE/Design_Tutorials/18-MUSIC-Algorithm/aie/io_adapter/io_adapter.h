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

class IO_Adapter {
  public:
    IO_Adapter(void) { };
    void run(input_stream<cfloat> * __restrict sig_i_0,
             input_stream<cfloat> * __restrict sig_i_1,
             adf::output_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict out);
    static void registerKernelClass(void) { REGISTER_FUNCTION(IO_Adapter::run); };
};

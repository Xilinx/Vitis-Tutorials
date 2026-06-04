//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "kernels.h"

// MULTIPLY_FACTOR can be defined via preprocessor (e.g., -DMULTIPLY_FACTOR=3)
// If not defined, defaults to 2
#ifndef MULTIPLY_FACTOR
#define MULTIPLY_FACTOR 2
#endif

void
int32_in_out(input_stream<int32> *i1,
               output_stream<int32> *o1)
{
    writeincr(o1, readincr(i1) * MULTIPLY_FACTOR);
}

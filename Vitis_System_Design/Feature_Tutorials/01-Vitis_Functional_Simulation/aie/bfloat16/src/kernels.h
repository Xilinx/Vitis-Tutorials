//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once
#include "my_defs.h"
#include "adf.h"

using namespace adf;
void myFunc_1df(adf::input_buffer<bfloat16,adf::extents< NUM_SAMPLES>>  & __restrict i1,
                adf::output_buffer<bfloat16,adf::extents< NUM_SAMPLES>> & __restrict o1);


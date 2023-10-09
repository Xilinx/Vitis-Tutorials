/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>
using namespace adf;

void passth(input_buffer<int32> & __restrict wIn,
                output_buffer<int32> & __restrict wOut);
void passth_cint16(input_buffer<cint16> & __restrict wIn,
                output_buffer<cint16> & __restrict wOut);

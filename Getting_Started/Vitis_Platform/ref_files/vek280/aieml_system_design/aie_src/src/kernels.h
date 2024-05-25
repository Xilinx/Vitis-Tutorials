/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once
#include <adf.h>
#include "aie_api/aie.hpp"
#include "system_settings.h"

#ifndef OPTIMIZED_SOURCE
#define OPTIMIZED_SOURCE 0
#endif

template<typename ITYPE,typename OTYPE, int SHIFT_RESULT>
void ClassicMatMult(adf::input_buffer<ITYPE,adf::extents<adf::inherited_extent,adf::inherited_extent>> & __restrict inA, adf::input_buffer<ITYPE,adf::extents<adf::inherited_extent,adf::inherited_extent>> & __restrict inB, adf::output_buffer<OTYPE,adf::extents<adf::inherited_extent,adf::inherited_extent>> & __restrict outC);

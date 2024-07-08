//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#include "bilinear_kernel.h"
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>

void bilinear_kernel::interp(input_buffer<int32, extents<BUFFER_SIZE_IN>>& __restrict in_A, 
                             input_buffer<int32, extents<BUFFER_SIZE_IN>>& __restrict in_B, 
                             input_buffer<int32, extents<BUFFER_SIZE_IN>>& __restrict in_C, 
                             output_buffer<int32, extents<BUFFER_SIZE_OUT>>& __restrict out)
{
	// iterators for input & output buffers
	auto pInA = aie::begin_vector<8>(in_A);
	auto pInB = aie::begin_vector<8>(in_B);
	auto pInC = aie::begin_vector<8>(in_C);
	auto pOut = aie::begin_vector<8>(out);

    for (unsigned i = 0; i < PXLPERGRP/8; i++)
		chess_prepare_for_pipelining
		chess_loop_count(PXLPERGRP/8)
	{
		// get data for first x interpolation
		auto xfrac = (*pInA++).cast_to<float>();
		auto p11 = (*pInB++).cast_to<float>();
		auto p21 = (*pInC++).cast_to<float>();

		// compute first x interpolation
		auto tempy1 = fpmac(p11,xfrac,0,0x76543210,p21,0,0x76543210);
		auto pxy1 = fpmsc(tempy1,xfrac,0,0x76543210,p11,0,0x76543210);

		// get data for second x interpolation
		auto p12 = (*pInB++).cast_to<float>();
		auto p22 = (*pInC++).cast_to<float>();

		// compute second x interpolation
		auto tempy2 = fpmac(p12,xfrac,0,0x76543210,p22,0,0x76543210);
		auto pxy2 = fpmsc(tempy2,xfrac,0,0x76543210,p12,0,0x76543210);

		// get data for y interpolation
		auto yfrac = (*pInA++).cast_to<float>();

		// compute y interpolation
		auto tempxy = fpmac(pxy1,yfrac,0,0x76543210,pxy2,0,0x76543210);
		auto pxy = fpmsc(tempxy,yfrac,0,0x76543210,pxy1,0,0x76543210);

		// write interpolated pixels to output
		*pOut++ = as_v8int32(pxy);
	}
}

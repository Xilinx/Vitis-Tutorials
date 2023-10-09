//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>
#include "buffers.h"

using namespace adf;

class bilinear_kernel
{
private:
	alignas(aie::vector_ldst_align_v<float,8>) const float xacc_init[8] = {1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0};
	alignas(aie::vector_ldst_align_v<float,8>) const float xmul_init[8]  = {-1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0};
	alignas(aie::vector_ldst_align_v<float,8>) const float yacc_init[8] = {1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0};
	alignas(aie::vector_ldst_align_v<float,8>) const float ymul_init[8] = {-1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0};

	alignas(32) float (&xbuf)[4*PXLPERGRP];
	alignas(32) float (&ybuf)[4*PXLPERGRP];
	alignas(32) float (&zbuf)[4*PXLPERGRP];

public:
	bilinear_kernel(float (&xbuf_i)[4*PXLPERGRP], float (&ybuf_i)[4*PXLPERGRP], float (&zbuf_i)[4*PXLPERGRP]) 
	                : xbuf(xbuf_i), ybuf(ybuf_i), zbuf(zbuf_i) {}
	void interp(adf::input_buffer<int32, extents<BUFFER_SIZE_IN>>& in, adf::output_buffer<int32, extents<BUFFER_SIZE_OUT>>& out);
	static void registerKernelClass()
	{
		REGISTER_FUNCTION(bilinear_kernel::interp);
		REGISTER_PARAMETER(xbuf);
		REGISTER_PARAMETER(ybuf);
		REGISTER_PARAMETER(zbuf);
	}
};

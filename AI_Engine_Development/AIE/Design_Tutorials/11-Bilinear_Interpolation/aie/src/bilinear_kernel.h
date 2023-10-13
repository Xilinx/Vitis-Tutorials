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
	alignas(aie::vector_ldst_align_v<float,8>) const float acc_init[8] = {1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0};
	alignas(aie::vector_ldst_align_v<float,8>) const float mul_init[8]  = {-1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0};

	alignas(32) float (&lgbuf)[8*PXLPERGRP];
	alignas(32) float (&smbuf)[4*PXLPERGRP];

public:
	bilinear_kernel(float (&lgbuf_i)[8*PXLPERGRP], float (&smbuf_i)[4*PXLPERGRP]) 
	                : lgbuf(lgbuf_i), smbuf(smbuf_i) {}
	void interp(adf::input_buffer<int32, extents<BUFFER_SIZE_IN>>& in, adf::output_buffer<int32, extents<BUFFER_SIZE_OUT>>& out);
	static void registerKernelClass()
	{
		REGISTER_FUNCTION(bilinear_kernel::interp);
		REGISTER_PARAMETER(lgbuf);
		REGISTER_PARAMETER(smbuf);
	}
};

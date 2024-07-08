//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#pragma once

#include <aie_api/aie.hpp>
#include "buffers.h"

using namespace adf;

class bilinear_kernel
{
private:

public:
	bilinear_kernel() {}
	void interp(input_buffer<int32, extents<BUFFER_SIZE_IN>>& __restrict in_A, 
	            input_buffer<int32, extents<BUFFER_SIZE_IN>>& __restrict in_B, 
	            input_buffer<int32, extents<BUFFER_SIZE_IN>>& __restrict in_C, 
	            output_buffer<int32, extents<BUFFER_SIZE_OUT>>& __restrict out);
	static void registerKernelClass()
	{
		REGISTER_FUNCTION(bilinear_kernel::interp);
	}
};

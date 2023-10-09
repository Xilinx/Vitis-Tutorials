// 
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
// 

#pragma once
#include <adf.h>
#include "ddc_include.h"
#include "aie_api/aie_adf.hpp"

using namespace adf;

class demixer_5c_kernel0 {
public:

	demixer_5c_kernel0(void);
	void run(input_buffer<cint16> & __restrict cb_input, output_buffer<cint16> & __restrict cb_output);
	
	static void registerKernelClass( void )
	{
		REGISTER_FUNCTION( demixer_5c_kernel0::run );
	}	
};

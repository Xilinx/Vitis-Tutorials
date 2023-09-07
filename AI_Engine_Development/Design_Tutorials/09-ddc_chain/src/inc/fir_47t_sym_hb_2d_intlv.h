// 
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
// 

#pragma once
#include <adf.h>
#include "ddc_include.h"
#include "aie_api/aie_adf.hpp"

using namespace adf;

class fir_47t_sym_hb_2d {
public:
	static constexpr unsigned HB47_2D_COEFFICIENTS = 16;
	alignas(32) static constexpr int16_t coeffs_hb47_2d[HB47_2D_COEFFICIENTS] ={
			-15,   41,  -95,  188, -339,  572, -921, 1441, -2244, 3614, -6606, 20742,    0,    0,    0,    0
	};
	fir_47t_sym_hb_2d(void);
	void run(input_buffer<cint16> & __restrict cb_input, output_buffer<cint16> & __restrict cb_output);
	
	static void registerKernelClass( void )
	{
		REGISTER_FUNCTION( fir_47t_sym_hb_2d::run );
	}	
};

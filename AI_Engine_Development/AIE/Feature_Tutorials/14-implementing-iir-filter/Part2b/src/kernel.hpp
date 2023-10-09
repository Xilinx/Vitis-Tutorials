/*********************************************************************
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
**********************************************************************/

#ifndef __KERNEL_HPP__	// include guard to prevent multiple inclusion

	#define __KERNEL_HPP__

	#include <adf.h>			// Adaptive DataFlow header
	#include <aie_api/aie.hpp>	// header files for high-level intrinsics

	using Vector8f = aie::vector<float, 8>;		// vector of 8 floating-point elements
	using Vector16f = aie::vector<float, 16>;	// vector of 16 floating-point elements
	using VAcc8f = aie::accum<accfloat, 8>;		// accumulator with 8 floating-point elements

	//#define USE_API	// comment out to use low-level intrinsics

	const unsigned burst_cnt = 256;	// process burst_cnt * 8 samples per function invocation

	template<unsigned id>
	void SecondOrderSection(
		adf::input_buffer<float> & __restrict idata,	// 8 input samples per iteration
		adf::output_buffer<float> & __restrict odata,	// 8 output samples per iteration
		const float (&C_e)[48],		// run-time parameter: SIMD matrix of coefficients (even columns)
		const float (&C_o)[48]		// run-time parameter: SIMD matrix of coefficients (odd columns)
	);

#endif // __KERNEL_HPP__

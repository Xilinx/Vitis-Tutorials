/*********************************************************************
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
**********************************************************************/

#include <aie_api/aie_adf.hpp>

// !!! Need to separate C++20 portions for kernel from C++14-processed files (e.g. graph.hpp and tb.cpp)
#include "kernel.hpp"

//#define KERNEL_DEBUG	// comment-out to suppress debug messages

#ifdef KERNEL_DEBUG

	#include <aie_api/utils.hpp>	// print vector register contents

#endif // KERNEL_DEBUG

template<unsigned id>
void SecondOrderSection(
	adf::input_buffer<float> & __restrict idata,	// 8 input samples per iteration
	adf::output_buffer<float> & __restrict odata,	// 8 output samples per iteration
	const float (&C)[96]	// run-time parameter: SIMD matrix of coefficients
) {

	#ifdef KERNEL_DEBUG
		printf("Now in SecondOrderSection()...\n");
	#endif // KERNEL_DEBUG

	static Vector8f state_reg = aie::zeros<float, 8>();	// clear states

	// input/output iterators
	auto inIter = aie::begin_vector<8>(idata);
	auto outIter = aie::begin_vector<8>(odata);

	Vector8f xreg_hi = *inIter++;		// fetch input samples
	Vector16f xreg = aie::concat(state_reg, xreg_hi);	// xreg[4]: ym2; xreg[5]: ym1; xreg[6]: xm2; xreg[7]: xm1; xreg[8:15]: x0:x7
	Vector8f coeff = aie::load_v<8>(&C[0]);
	VAcc8f acc = aie::mul(coeff, xreg[4]);				// do 1st multiplication instead of zeroing

	for (auto i = 1; i < 12; i++) {
		coeff = aie::load_v<8>(&C[8 * i]);
		float xval = xreg[i + 4];
		acc = aie::mac(acc, coeff, xval);
		#ifdef KERNEL_DEBUG
			printf("  i = %d:\n", i);
			printf("    xreg[%d] = %f\n", (i + 4), xval);
			aie::print(coeff, true, "    coeff:");
			aie::print(acc.to_vector<float>(), true, "    acc:");
		#endif // KERNEL_DEBUG
	}

	Vector8f yout = acc;

	// update states
	state_reg[4] = yout[6];
	state_reg[5] = yout[7];
	state_reg[6] = xreg_hi[6];
	state_reg[7] = xreg_hi[7];

	*outIter++ = yout;

} // end SecondOrderSection()

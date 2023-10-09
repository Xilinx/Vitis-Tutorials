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

#ifdef USE_API

	template<unsigned id>
	void SecondOrderSection(
		adf::input_buffer<float> & __restrict idata,	// 8 input samples per iteration
		adf::output_buffer<float> & __restrict odata,	// 8 output samples per iteration
		const float (&C_e)[48],		// run-time parameter: SIMD matrix of coefficients (even columns)
		const float (&C_o)[48]		// run-time parameter: SIMD matrix of coefficients (odd columns)
	) {

		#ifdef KERNEL_DEBUG
			printf("Now in SecondOrderSection()...\n");
		#endif // KERNEL_DEBUG

		static Vector8f state_reg = aie::zeros<float, 8>();	// clear states

		// input/output iterators
		auto inIter = aie::begin_vector<8>(idata);
		auto outIter = aie::begin_vector<8>(odata);

		for (auto i = 0; i < burst_cnt; i++) {
			Vector8f xreg_hi = *inIter++;		// fetch input samples
			Vector16f xreg = aie::concat(state_reg, xreg_hi);

			auto ecoeff_iter = aie::begin_vector<8>(&C_e[0]);
			auto ocoeff_iter = aie::begin_vector<8>(&C_o[0]);

			VAcc8f acc_e = aie::zeros<accfloat, 8>();	// even accumulator
			VAcc8f acc_o = aie::zeros<accfloat, 8>();	// odd accumulator

			for (auto j = 0; j < 6; j++) {

				acc_e = aie::mac(acc_e, xreg.get(2 * j + 4), *ecoeff_iter++);	// even columns
				acc_o = aie::mac(acc_o, xreg.get(2 * j + 5), *ocoeff_iter++);	// odd columns

			} // end for (auto j = 0; j < 6; j ++)

			acc_o = aie::add(acc_o, acc_e.to_vector());	// acc_o += acc_e
			Vector8f yout = acc_o.to_vector();

			// update states
			state_reg = xreg_hi;
			state_reg[4] = yout[6];
			state_reg[5] = yout[7];

			*outIter++ = yout;

		} // end for (auto i = 0; i < burst_cnt; i++)

	} // end SecondOrderSection()

#else

	template<unsigned id>
	void SecondOrderSection(
		adf::input_buffer<float> & __restrict idata,	// 8 input samples per iteration
		adf::output_buffer<float> & __restrict odata,	// 8 output samples per iteration
		const float (&C_e)[48],		// run-time parameter: SIMD matrix of coefficients (even columns)
		const float (&C_o)[48]		// run-time parameter: SIMD matrix of coefficients (odd columns)
	) {

		#ifdef KERNEL_DEBUG
			printf("Now in SecondOrderSection()...\n");
		#endif // KERNEL_DEBUG

		static v8float state_reg = null_v8float();

		// input/output iterators
		auto inIter = aie::begin_vector<8>(idata);
		auto outIter = aie::begin_vector<8>(odata);

		for (auto i = 0; i < burst_cnt; i++) {

			v8float xreg_hi = *inIter++;
			v16float xreg = concat(state_reg, xreg_hi);

			v8float acc_e = null_v8float();
			v8float acc_o = null_v8float();

			v8float *ptr_coeff_e = (v8float *)(&C_e[0]);
			v8float *ptr_coeff_o = (v8float *)(&C_o[0]);

			for (auto j = 0; j < 6; j++)
			chess_flatten_loop
			{

				acc_e = fpmac(acc_e, xreg, (2 * j + 4), 0, *ptr_coeff_e++, 0, 0x76543210);	// even columns
				acc_o = fpmac(acc_o, xreg, (2 * j + 5), 0, *ptr_coeff_o++, 0, 0x76543210);	// odd columns

			} // end for (auto j = 0; j < 6; j++)

			acc_o = fpadd(acc_o, acc_e);
			*outIter++ = acc_o;

			// update states
			state_reg = xreg_hi;
			state_reg = upd_elem(state_reg, 4, ext_elem(acc_o, 6));
			state_reg = upd_elem(state_reg, 5, ext_elem(acc_o, 7));

		} // end for (auto i = 0; i < burst_cnt; i++)

	} // end SecondOrderSection()

#endif // USE_API

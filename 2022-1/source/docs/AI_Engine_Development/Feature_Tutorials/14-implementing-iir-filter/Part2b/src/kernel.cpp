/****************************************************************
# Support
GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

Copyright 2022 Xilinx, Inc.
****************************************************************/

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
		input_window<float> *idata,		// 8 input samples per iteration
		output_window<float> *odata, 	// 8 output samples per iteration
		const float (&C_e)[48],			// run-time parameter: SIMD matrix of coefficients (even columns)
		const float (&C_o)[48]			// run-time parameter: SIMD matrix of coefficients (odd columns)
	) {
		static Vector8f state_reg = aie::zeros<float, 8>();	// clear states

		for (auto i = 0; i < burst_cnt; i++)
		//chess_prepare_for_pipelining
		{
			Vector8f xreg_hi = window_readincr_v<8>(idata);		// fetch input samples
			Vector16f xreg = aie::concat(state_reg, xreg_hi);

			VAcc8f acc_e = aie::zeros<accfloat, 8>();	// even accumulator
			VAcc8f acc_o = aie::zeros<accfloat, 8>();	// odd accumulator

			for (auto j = 0; j < 6; j++) {

				Vector8f coeff_e = aie::load_v<8>(&C_e[8 * j]);		// even columns
				float xreg_e = xreg[2 * j + 4];
				acc_e = aie::mac(acc_e, xreg_e, coeff_e);

				Vector8f coeff_o = aie::load_v<8>(&C_o[8 * j]);		// odd columns
				float xreg_o = xreg[2 * j + 5];
				acc_o = aie::mac(acc_o, xreg_o, coeff_o);

			} // end for (auto j = 0; j < 6; j ++)

			acc_o = aie::add(acc_o, acc_e.to_vector());	// acc_o += acc_e
			Vector8f yout = acc_o.to_vector();

			// update states
			state_reg = xreg_hi;
			state_reg[4] = yout[6];
			state_reg[5] = yout[7];

			window_writeincr(odata, yout);

		} // end for (auto i = 0; i < burst_cnt; i++)

	} // end SecondOrderSection()

#else

	template<unsigned id>
	void SecondOrderSection(
		input_window_float *idata,		// 8 input samples per iteration
		output_window_float *odata, 	// 8 output samples per iteration
		const float (&C_e)[48],			// run-time parameter: SIMD matrix of coefficients (even columns)
		const float (&C_o)[48]			// run-time parameter: SIMD matrix of coefficients (odd columns)
	) {

		#ifdef KERNEL_DEBUG
			printf("Now in SecondOrderSection()...\n");
		#endif // KERNEL_DEBUG

		static v8float state_reg = null_v8float();

		for (auto i = 0; i < burst_cnt; i++) {

			v8float xreg_hi = window_readincr_v8(idata);
			v16float xreg = concat(state_reg, xreg_hi);

			v8float acc_e = null_v8float();
			v8float acc_o = null_v8float();

			v8float *ptr_coeff_e = (v8float *)(&C_e[0]);
			v8float *ptr_coeff_o = (v8float *)(&C_o[0]);

			for (auto j = 0; j < 6; j++)
			chess_flatten_loop
			{

				v8float coeff_e = *ptr_coeff_e++;
				acc_e = fpmac(acc_e, xreg, (2 * j + 4), 0, coeff_e, 0, 0x76543210);

				v8float coeff_o = *ptr_coeff_o++;
				acc_o = fpmac(acc_o, xreg, (2 * j + 5), 0, coeff_o, 0, 0x76543210);

			} // end for (auto j = 0; j < 6; j++)

			acc_o = fpadd(acc_o, acc_e);
			window_writeincr(odata, acc_o);

			// update states
			state_reg = xreg_hi;
			state_reg = upd_elem(state_reg, 4, ext_elem(acc_o, 6));
			state_reg = upd_elem(state_reg, 5, ext_elem(acc_o, 7));

		} // end for (auto i = 0; i < burst_cnt; i++)

	} // end SecondOrderSection()

#endif // USE_API

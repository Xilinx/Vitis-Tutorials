/****************************************************************
# Support
GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"><sup>XD0xx | &copy; Copyright 2021 Xilinx, Inc.</sup></p>
****************************************************************/

#ifndef __KERNEL_HPP__	// include guard to prevent multiple inclusion

	#define __KERNEL_HPP__

	#include <adf.h>			// Adaptive DataFlow header
	#include <aie_api/aie.hpp>	// header files for high-level intrinsics

	typedef aie::vector<float, 8> Vector8f;		// vector of 8 floating-point elements
	typedef aie::vector<float, 16> Vector16f;	// vector of 16 floating-point elements
	typedef aie::accum<accfloat, 8> VAcc8f;		// accumulator with 8 floating-point elements

	template<unsigned id>
	void SecondOrderSection(
	    input_window<float> *idata,		// 8 input samples per iteration
		output_window<float> *odata, 	// 8 output samples per iteration
		const float (&C)[96]			// run-time parameter: SIMD matrix of coefficients
	);

#endif // __KERNEL_HPP__

/****************************************************************
# Support
GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

Copyright 2022 Xilinx, Inc.
****************************************************************/

#ifndef __GRAPH_H__			// include guard to prevent multiple inclusion

	#define __GRAPH_H__

	#include <adf.h>		// Adaptive DataFlow header

	#include "kernel.hpp"

	using namespace adf;

	// dataflow graph declaration
	class the_graph : public graph {	// inherit all properties of the adaptive     dataflow graph

		public:
			input_plio pl_in;
			output_plio pl_out;

			kernel section1;
			input_port cmtx_e;	// input port for SIMD matrix coefficients (even columns)
			input_port cmtx_o;	// input port for SIMD matrix coefficients (odd columns)

			// constructor
			the_graph() {

				// associate the kernel with the function to be executed
				section1 = kernel::create(SecondOrderSection<1>);

				pl_in = input_plio::create("Input", plio_32_bits, "data/input.dat");
				pl_out = output_plio::create("Output", plio_32_bits, "output.dat");

				const unsigned num_bytes = 8 * sizeof(float) * burst_cnt;

				// establish connections

				connect<parameter>(cmtx_e, adf::async(section1.in[1]));
				connect<parameter>(cmtx_o, adf::async(section1.in[2]));

				connect<window<num_bytes>> (pl_in.out[0], section1.in[0]);				// window size in bytes
				connect<window<num_bytes>> (section1.out[0], pl_out.in[0]);

				// specify which source code file contains the kernel function
				source(section1) = "kernel.cpp";

				// !!! temporary value: assumes this kernel dominates the AI engine tile !!!
				runtime<ratio>(section1) = 1.0;

			} // end the_graph()

	}; // end class the_graph

#endif // __GRAPH_H__

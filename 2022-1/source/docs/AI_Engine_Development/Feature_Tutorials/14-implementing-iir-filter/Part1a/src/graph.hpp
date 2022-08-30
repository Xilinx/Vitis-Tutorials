/****************************************************************
# Support
GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"><sup>XD0xx | &copy; Copyright 2021 Xilinx, Inc.</sup></p>
****************************************************************/

#ifndef __GRAPH_H__			// include guard to prevent multiple inclusion

	#define __GRAPH_H__

	#include <adf.h>		// Adaptive DataFlow header

	#include "kernel.hpp"

	using namespace adf;

	// dataflow graph declaration
	class the_graph : public graph {	// inherit all properties of the adaptive     dataflow graph

		private:
			kernel section1;

		public:
			input_port in;		// input port for data to enter the kernel
			input_port cmtx1;	// input port for SIMD matrix coefficients
			output_port out;	// output port for data to leave the kernel

			// constructor
			the_graph() {

				// associate the kernel with the function to be executed
				section1 = kernel::create(SecondOrderSection<1>);

				const unsigned num_bytes = 8 * sizeof(float);

				// establish connections
				connect<window<num_bytes>> net0 (in, section1.in[0]);				// window size in bytes
				connect<parameter>(cmtx1, adf::async(section1.in[1]));
				connect<window<num_bytes>> net1 (section1.out[0], out);

				// specify which source code file contains the kernel function
				source(section1) = "kernel.cpp";

				// !!! temporary value: assumes this kernel dominates the AI engine tile !!!
				runtime<ratio>(section1) = 1.0;

			} // end the_graph()

	}; // end class the_graph

#endif // __GRAPH_H__

/*********************************************************************
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
**********************************************************************/

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

				const unsigned num_samples = 8 * burst_cnt;

				// declare buffer sizes
				dimensions(section1.in[0]) = {num_samples};
				dimensions(section1.out[0]) = {num_samples};

				// establish connections

				connect<parameter>(cmtx_e, adf::async(section1.in[1]));
				connect<parameter>(cmtx_o, adf::async(section1.in[2]));

				connect(pl_in.out[0], section1.in[0]);
				connect(section1.out[0], pl_out.in[0]);

				// specify which source code file contains the kernel function
				source(section1) = "kernel.cpp";

				// !!! temporary value: assumes this kernel dominates the AI engine tile !!!
				runtime<ratio>(section1) = 1.0;

			} // end the_graph()

	}; // end class the_graph

#endif // __GRAPH_H__

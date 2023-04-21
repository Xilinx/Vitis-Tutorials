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

		private:
			kernel section1;

		public:
			input_plio in;		// input port for data to enter the kernel
			input_port cmtx1;	// input port for SIMD matrix coefficients
			output_plio out;	// output port for data to leave the kernel

			// constructor
			the_graph() {

				// associate the kernel with the function to be executed
				section1 = kernel::create(SecondOrderSection<1>);

				// declare data widths and files for simulation
				in = input_plio::create(plio_32_bits, "data/input.dat");
				out = output_plio::create(plio_32_bits, "output.dat");

				const unsigned num_samples = 8;

				// establish connections
				connect(in.out[0], section1.in[0]);
				dimensions(section1.in[0]) = {num_samples};
				connect<parameter>(cmtx1, adf::async(section1.in[1]));
				connect(section1.out[0], out.in[0]);
				dimensions(section1.out[0]) = {num_samples};

				// specify which source code file contains the kernel function
				source(section1) = "kernel.cpp";

				// !!! temporary value: assumes this kernel dominates the AI engine tile !!!
				runtime<ratio>(section1) = 1.0;

			} // end the_graph()

	}; // end class the_graph

#endif // __GRAPH_H__

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
			kernel section2;
			kernel section3;

		public:
			input_plio in;		// input port for data to enter the kernel
			input_port cmtx1;	// input port for SIMD matrix coefficients
			input_port cmtx2;
			input_port cmtx3;
			output_plio out;	// output port for data to leave the kernel

			// constructor
			the_graph() {

				// associate the kernel with the function to be executed
				section1 = kernel::create(SecondOrderSection<1>);
				section2 = kernel::create(SecondOrderSection<2>);
				section3 = kernel::create(SecondOrderSection<3>);

				// declare data widths and files for simulation
				#ifndef RTP_SWITCH
					in = input_plio::create(plio_32_bits, "data/input.dat");
				#else
					in = input_plio::create(plio_32_bits, "data/two_freqs.dat");
				#endif // RTP_SWITCH
				out = output_plio::create(plio_32_bits, "output.dat");

				const unsigned num_samples = 8;

				// declare buffer sizes
				dimensions(section1.in[0]) = {num_samples};
				dimensions(section1.out[0]) = {num_samples};

				dimensions(section2.in[0]) = {num_samples};
				dimensions(section2.out[0]) = {num_samples};

				dimensions(section3.in[0]) = {num_samples};
				dimensions(section3.out[0]) = {num_samples};

				// establish connections
				connect(in.out[0], section1.in[0]);
				connect<parameter>(cmtx1, adf::async(section1.in[1]));

				connect(section1.out[0], section2.in[0]);
				connect<parameter>(cmtx2, adf::async(section2.in[1]));

				connect(section2.out[0], section3.in[0]);
				connect<parameter>(cmtx3, adf::async(section3.in[1]));

				connect(section3.out[0], out.in[0]);

				// specify which source code file contains the kernel function
				source(section1) = "kernel.cpp";
				source(section2) = "kernel.cpp";
				source(section3) = "kernel.cpp";

				// !!! temporary value: assumes this kernel dominates the AI engine tile !!!
				runtime<ratio>(section1) = 1.0;
				runtime<ratio>(section2) = 1.0;
				runtime<ratio>(section3) = 1.0;

			} // end the_graph()

	}; // end class the_graph

#endif // __GRAPH_H__

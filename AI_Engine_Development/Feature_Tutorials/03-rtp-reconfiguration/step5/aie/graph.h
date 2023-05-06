/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <adf.h>

#include "fir24_sym_param.h"

using namespace adf;

class adaptive_graph : public graph
{
public:
    input_plio in;
    port<direction::in> coefficients;
    port<direction::inout> coefficients_readback;

    // This is the hierachical port used to send output samples to the platform
    output_plio dataout;

    // Declare the filter kernel
    kernel fir24;

    adaptive_graph()
    {
      // Initialize the kernels
      fir24 = kernel::create(fir24_sym);

	  //Create input & output
	  in=input_plio::create("Datain0", plio_32_bits,  "data/input.txt");
	  dataout=output_plio::create("Dataout0", plio_32_bits,  "data/output.txt");

      // Set a runtime ratio for the filter
      runtime<ratio>(fir24) = 0.5;

      // Declare the sources for the two kernels
      source(fir24) = "kernels/hb24.cc";

      // connect filter coefficients, readback
      connect< parameter >(coefficients, async(fir24.in[1]));
      connect< parameter >(async(fir24.inout[0]),coefficients_readback);
      connect< >(in.out[0], fir24.in[0]);
      connect< >(fir24.out[0], dataout.in[0]);
    }
};




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
    port<direction::in> coefficients;
    input_plio in;

    // This is the hierachical port used to send output samples to the platform
    output_plio dataout;

    // Declare the filter kernel
    kernel fir24;
    // Declare the noise generator
    kernel noisegen;

    adaptive_graph()
    {
      // Initialize the kernels
      fir24 = kernel::create(fir24_sym);

      in=input_plio::create("Datain0", plio_32_bits,  "data/input.txt");
	  dataout=output_plio::create("Dataout0", plio_32_bits,  "data/output.txt");

      // Set a runtime ratio for the filter
      runtime<ratio>(fir24) = 0.5;

      // Declare the sources for the kernel
      source(fir24) = "kernels/hb24.cc";

      // connect filter coefficients 
      connect< parameter >(coefficients, async(fir24.in[1]));
      connect< >(in.out[0], fir24.in[0]);
      connect< >(fir24.out[0], dataout.in[0]);
    }
};




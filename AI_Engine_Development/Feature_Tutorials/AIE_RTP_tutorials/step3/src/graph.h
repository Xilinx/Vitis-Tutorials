/**********
Copyright (c) 2020, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/
#include <adf.h>

#include "fir24_sym_param.h"
#include "noise.h"

using namespace adf;

class adaptive_graph : public graph
{
public:
    port<direction::in> coefficients;

    // This is the hierachical port used to send output samples to the platform
    port<direction::out> dataout;

    // Declare the filter kernel
    kernel fir24;
    // Declare the noise generator
    kernel noisegen;

    adaptive_graph()
    {
      // Initialize the kernels
      fir24 = kernel::create(fir24_sym);
      noisegen = kernel::create(random_noise);

      // Place the noise generator in the fabric
      fabric<pl>(noisegen);

      // Set a runtime ratio for the filter
      runtime<ratio>(fir24) = 0.5;

      // Declare the sources for the two kernels
      source(fir24) = "kernels/hb24.cc";
      source(noisegen) = "fpga/random_noise.cpp";

      // connect filter coefficients
      connect< parameter >(coefficients, async(fir24.in[1]));
      connect< stream, window<256, 96> >(noisegen.out[0], fir24.in[0]);
      connect< window<256> >(fir24.out[0], dataout);
    }
};




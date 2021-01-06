/**********
© Copyright 2020 Xilinx, Inc.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/
#include <adf.h>

#include "fir24_sym_param.h"
#include "noise.h"

using namespace adf;

class adaptive_graph : public graph
{
public:
    port<direction::in> coefficients;
    port<direction::in> size;

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
      source(noisegen) = "hls/random_noise.cpp";

      // connect filter coefficients and size
      connect< parameter >(coefficients, async(fir24.in[1]));
      connect< parameter >(size, async(noisegen.in[0]));
      connect< stream, window<256, 96> >(noisegen.out[0], fir24.in[0]);
      connect< window<256> >(fir24.out[0], dataout);
    }
};




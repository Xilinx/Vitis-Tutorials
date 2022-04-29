/**********
© Copyright 2020-2022 Xilinx, Inc.
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
      connect< window<256, 96> >(in.out[0], fir24.in[0]);
      connect< window<256> >(fir24.out[0], dataout.in[0]);
    }
};




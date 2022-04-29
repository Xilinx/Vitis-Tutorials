/**********
© Copyright 2021-2022 Xilinx, Inc.
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

#include "kernel.h"

using namespace adf;

class adaptive_graph : public graph
{
public:
    input_plio in;

    output_plio dataout;

    // Declare the filter kernel
    kernel k[2];

    adaptive_graph()
    {
		k[0] = kernel::create(aie_dest1);
		source(k[0]) = "aie_dest1.cc";
		k[1] = kernel::create(aie_dest2);
		source(k[1]) = "aie_dest2.cc";
	
		in=input_plio::create("Datain0", plio_32_bits,  "data/input.txt");
		dataout=output_plio::create("Dataout0", plio_32_bits,  "data/output.txt");
	
		runtime<ratio>(k[0]) = 0.8;
		runtime<ratio>(k[1]) = 0.8;

		connect< window<128> >net0(in.out[0], k[0].in[0]);
		connect< stream >net1(k[0].out[0], k[1].in[0]);
		connect< window<128> >net2(k[0].out[1], k[1].in[1]);
		connect< stream >net3(k[1].out[0], dataout.in[0]);
		fifo_depth(net1)=32;
    }
};




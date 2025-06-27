/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
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

		in=input_plio::create("Datain0", plio_128_bits,  "data/input.txt");
		dataout=output_plio::create("Dataout0", plio_128_bits,  "data/output.txt");

		runtime<ratio>(k[0]) = 0.8;
		runtime<ratio>(k[1]) = 0.8;

		connect< >net0(in.out[0], k[0].in[0]);
		connect< >net1(k[0].out[0], k[1].in[0]);
		connect< >net2(k[0].out[1], k[1].in[1]);
		connect< >net3(k[1].out[0], dataout.in[0]);
		fifo_depth(net1)=32;
    }
};




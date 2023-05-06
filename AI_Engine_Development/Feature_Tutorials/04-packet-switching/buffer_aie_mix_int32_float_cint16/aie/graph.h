/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <adf.h>
#include "kernels.h"

using namespace adf;
class mygraph: public adf::graph {
private:
	adf:: kernel core[4];

	adf:: pktsplit<4> sp;
	adf:: pktmerge<4> mg;
public:
	adf::input_plio  in;
	adf::output_plio  out;
	mygraph() {
		core[0] = adf::kernel::create(aie_core1);
		core[1] = adf::kernel::create(aie_core2);
		core[2] = adf::kernel::create(aie_core3);
    	core[3] = adf::kernel::create(aie_core4);
		adf::source(core[0]) = "aie_core1.cpp";
		adf::source(core[1]) = "aie_core2.cpp";
		adf::source(core[2]) = "aie_core3.cpp";
		adf::source(core[3]) = "aie_core4.cpp";
		
		in=adf::input_plio::create("Datain0", plio_32_bits,  "data/input.txt");
		out=adf::output_plio::create("Dataout0", plio_32_bits,  "data/output.txt");

		sp = adf::pktsplit<4>::create();	
		mg = adf::pktmerge<4>::create();
		for(int i=0;i<4;i++){
			adf::runtime<ratio>(core[i]) = 0.9;
 			adf::connect<> (sp.out[i], core[i].in[0]);
			adf::connect<> (core[i].out[0], mg.in[i]);
		}
		adf::connect<adf::pktstream> (in.out[0], sp.in[0]);	
		adf::connect<adf::pktstream> (mg.out[0], out.in[0]);
	}
};

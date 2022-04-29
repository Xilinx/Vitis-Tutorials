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
 			adf::connect<adf::pktstream, adf::window<32> > (sp.out[i], core[i].in[0]);
			adf::connect<adf::window<32>, adf::pktstream > (core[i].out[0], mg.in[i]);
		}
		adf::connect<adf::pktstream> (in.out[0], sp.in[0]);	
		adf::connect<adf::pktstream> (mg.out[0], out.in[0]);
	}
};

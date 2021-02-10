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

#include "graph.h"

PLIO *in0 = new PLIO("DataIn1", adf::plio_32_bits,"data/input.txt");

// RTL Kernel PLIO
PLIO *ai_to_pl = new PLIO("clip_in",adf::plio_32_bits, "data/output.txt"); 
PLIO *pl_to_ai = new PLIO("clip_out", adf::plio_32_bits,"data/input2.txt"); 

PLIO *out0 = new PLIO("DataOut1",adf::plio_32_bits, "data/output2.txt");

// RTL Kernel Addition to the platform
simulation::platform<2,2> platform(in0, pl_to_ai, out0, ai_to_pl);
//simulation::platform<1,1> platform(in0, out0);

clipped clipgraph;

connect<> net0(platform.src[0], clipgraph.in);
//connect<> net1(clipgraph.out, platform.sink[0]);

// Additional nets to the RTL Kernel
connect<> net1(clipgraph.clip_in,platform.sink[1]);
connect<> net2(platform.src[1],clipgraph.clip_out);
connect<> net3(clipgraph.out, platform.sink[0]);

#ifdef __AIESIM__
int main(int argc, char ** argv) {
	clipgraph.init();
    clipgraph.run(4); 
    clipgraph.end();
    return 0;
}
#endif

// /**********
// © Copyright 2020 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// **********/

#include "graphs/mygraph.h"


PLIO *in0   = new PLIO("DataIn0",   adf::plio_64_bits, "data/input.txt");
PLIO *in1   = new PLIO("DataIn1",   adf::plio_64_bits, "data/input.txt");
PLIO *in2   = new PLIO("DataIn2",   adf::plio_64_bits, "data/input.txt");
PLIO *in3   = new PLIO("DataIn3",   adf::plio_64_bits, "data/input.txt");
PLIO *out0  = new PLIO("DataOut0",  adf::plio_64_bits, "data/output0.txt");
PLIO *out1  = new PLIO("DataOut1",  adf::plio_64_bits, "data/output1.txt");
PLIO *out2  = new PLIO("DataOut2",  adf::plio_64_bits, "data/output2.txt");
PLIO *out3  = new PLIO("DataOut3",  adf::plio_64_bits, "data/output3.txt");
//PLIO *in = new PLIO("DataIn", cardano::plio_32_bits, "data/input0.txt");
//PLIO *out = new PLIO("DataOut", cardano::plio_32_bits, "data/output2.txt");

// connect dataflow graph to simulation platform
// Only for simulation // simulation::platform<1,1> platform(
// Only for simulation //     "data/input.txt",
// Only for simulation //     "data/output.txt"
// Only for simulation //     );
simulation::platform<4,4> platform(
    in0,
    in1,
    in2,
    in3,
    out0,
    out1,
    out2,
    out3
    );

mygraph<512, 0, 4>  mygraph_top;

connect<> datain_0(platform.src[0], mygraph_top.in[0]);
connect<> datain_1(platform.src[1], mygraph_top.in[1]);
connect<> datain_2(platform.src[2], mygraph_top.in[2]);
connect<> datain_3(platform.src[3], mygraph_top.in[3]);
connect<> dataout_0(mygraph_top.out[0], platform.sink[0]);
connect<> dataout_1(mygraph_top.out[1], platform.sink[1]);
connect<> dataout_2(mygraph_top.out[2], platform.sink[2]);
connect<> dataout_3(mygraph_top.out[3], platform.sink[3]);

#ifdef __AIESIM__
// initialize and run the dataflow graph
int main(void) 
{
  mygraph_top.init();
  mygraph_top.run();
  mygraph_top.end();
  return 0;
}
#endif


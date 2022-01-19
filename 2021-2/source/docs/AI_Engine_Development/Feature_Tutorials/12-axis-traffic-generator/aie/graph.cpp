/**********
© Copyright 2021 Xilinx, Inc.

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

#include "graphs/mygraph.h"


PLIO *in = new PLIO("DataIn",   adf::plio_64_bits, "data/input.txt");
PLIO *out = new PLIO("DataOut", adf::plio_64_bits, "data/output.txt");


simulation::platform<1,1> platform(
    in,
    out
    );

mygraph<WIN_SAMPS>  mygraph_top;
//mygraph_float<WIN_SAMPS> mygraph_top;

connect<> datain_0(platform.src[0], mygraph_top.in);
connect<> dataout_0(mygraph_top.out, platform.sink[0]);

// initialize and run the dataflow graph
#ifdef __AIESIM__
int main(void) 
{
  mygraph_top.init();
  mygraph_top.run(1);
  mygraph_top.end();
  return 0;
}
#endif


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
#include "graph.h"
using namespace adf;
PLIO *din = new PLIO("Datain0", plio_32_bits,  "data/input.txt");
PLIO *dout = new PLIO("Dataout0", plio_32_bits,  "data/output.txt");
simulation::platform<1,1> plat(din,dout);

adaptive_graph gr;

connect<> netin(plat.src[0],gr.in);
connect<> netout(gr.dataout,plat.sink[0]);


#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char **argv)
{
  gr.init();
  gr.run(4);
  gr.wait(10000);
};
#endif



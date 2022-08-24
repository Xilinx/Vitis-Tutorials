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

using namespace adf;

PLIO *din = new PLIO("Datain0", plio_32_bits,  "data/input.txt");
PLIO *dout = new PLIO("Dataout0", plio_32_bits,  "data/output.txt");
adf::simulation::platform<1,1> platform(din,dout);

mygraph gr;

adf::connect<> net0(platform.src[0], gr.in);
adf::connect<> net1(gr.out, platform.sink[0]);

#if defined(__AIESIM__) || defined(__ADF_FRONTEND__)
int main(int argc, char ** argv) {
  gr.init();
  gr.run(2);
  gr.end();
  return 0;
}
#endif

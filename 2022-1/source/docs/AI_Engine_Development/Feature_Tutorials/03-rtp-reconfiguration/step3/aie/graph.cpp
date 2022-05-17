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
#include "graph.h"

adaptive_graph gr;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char **argv)
{
  int narrow_filter[12] = {180, 89, -80, -391, -720, -834, -478, 505, 2063, 3896, 5535, 6504};
  int wide_filter[12] = {-21, -249, 319, -78, -511, 977, -610, -844, 2574, -2754, -1066, 18539};

  gr.init();
  //run for 16 iterations, update narrow filter coefficients, wait, update wide filter coefficients, run for 16 iterations
  gr.update(gr.coefficients, narrow_filter, 12);
  gr.run(16);
  gr.wait();
  gr.update(gr.coefficients, wide_filter, 12);
  gr.run(16);
  gr.end();
  return 0;
};
#endif




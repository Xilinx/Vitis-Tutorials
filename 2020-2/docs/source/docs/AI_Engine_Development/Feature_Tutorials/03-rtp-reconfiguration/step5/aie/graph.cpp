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
#include <iostream>

using namespace adf;
PLIO *din = new PLIO("Datain0", plio_32_bits,  "data/input.txt");
PLIO *dout = new PLIO("Dataout0", plio_32_bits,  "data/output.txt");
simulation::platform<1,1> plat(din,dout);

adaptive_graph gr;

// send complex output samples to the output file
connect<> netin(plat.src[0],gr.in);
connect<> netout(gr.dataout, plat.sink[0]);

#ifdef __AIESIM__
int main(int argc, char **argv)
{
  int narrow_filter[12] = {180, 89, -80, -391, -720, -834, -478, 505, 2063, 3896, 5535, 6504};
  int wide_filter[12] = {-21, -249, 319, -78, -511, 977, -610, -844, 2574, -2754, -1066, 18539};
  int coeffs_readback[12]={0,0,0,0,0,0,0,0,0,0,0,0};

  gr.init();
  //run for 16 iterations, update narrow filter coefficients, wait, update wide filter coefficients, run for 16 iterations
  gr.update(gr.coefficients, narrow_filter, 12);
  gr.run(16); // start PL kernel & AIE kernel
  gr.read(gr.coefficients_readback,coeffs_readback,12);
  std::cout<<"Coefficients read back are:";
  for(int i=0;i<12;i++){
	std::cout<<coeffs_readback[i]<<",\t";
  }
  std::cout<<std::endl;
  gr.wait(); // wait PL kernel & AIE kernel to complete
  gr.read(gr.coefficients_readback,coeffs_readback,12);
  std::cout<<"Coefficients read back are:";
  for(int i=0;i<12;i++){
	std::cout<<coeffs_readback[i]<<",\t";
  }
  std::cout<<std::endl;
  gr.update(gr.coefficients, wide_filter, 12);
  gr.run(16); // start PL kernel & AIE kernel
  gr.read(gr.coefficients_readback,coeffs_readback,12);
  std::cout<<"Coefficients read back are:";
  for(int i=0;i<12;i++){
	std::cout<<coeffs_readback[i]<<",\t";
  }
  std::cout<<std::endl;

  gr.end();
};
#endif



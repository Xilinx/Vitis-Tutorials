/**********
Copyright (c) 2020, Xilinx, Inc.
All rights reserved.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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



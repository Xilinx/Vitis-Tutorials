/**********
© Copyright 2020-2021 Xilinx, Inc.
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
#include <adf.h>
#include "kernels.h"

class mygraph: public adf::graph {
private:
  adf:: kernel core[4];

  adf:: pktsplit<4> sp;
  adf:: pktmerge<4> mg;
public:
  adf::port<input>  in;
  adf::port<output>  out;
  mygraph() {
    core[0] = adf::kernel::create(aie_core1);
    core[1] = adf::kernel::create(aie_core2);
    core[2] = adf::kernel::create(aie_core3);
    core[3] = adf::kernel::create(aie_core4);
    adf::source(core[0]) = "aie_core1.cpp";
    adf::source(core[1]) = "aie_core2.cpp";
    adf::source(core[2]) = "aie_core3.cpp";
    adf::source(core[3]) = "aie_core4.cpp";

    sp = adf::pktsplit<4>::create();
    mg = adf::pktmerge<4>::create();
    for(int i=0;i<4;i++){
    	adf::runtime<ratio>(core[i]) = 0.9;
    	adf::connect<adf::pktstream, adf::window<32> > (sp.out[i], core[i].in[0]);
        adf::connect<adf::window<32>, adf::pktstream > (core[i].out[0], mg.in[i]);
    }
    adf::connect<adf::pktstream> (in, sp.in[0]);
    adf::connect<adf::pktstream> (mg.out[0], out);
  }
};

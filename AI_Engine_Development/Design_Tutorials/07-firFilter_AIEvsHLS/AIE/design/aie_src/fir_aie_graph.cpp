//------------------------------------------------------------------------------
// © Copyright 2021 Xilinx, Inc.
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
//------------------------------------------------------------------------------

#include "fir_aie_graph.h"

using namespace adf;

//PLIO *pldata_in  = new PLIO("DataIn",  plio_128_bits,"data/fir_input_128b.txt");
//PLIO *pldata_in  = new PLIO("DataIn",  plio_32_bits,"../../design/aie_src/aiesim_data/input_128b.txt");
//PLIO *pldata_in  = new PLIO("DataIn",  plio_32_bits,"../../design/aie_src/aiesim_data/input_new.txt");
PLIO *pldata_in  = new PLIO("DataIn",  plio_128_bits,"../../../design/aie_src/aiesim_data/input_const.txt");
PLIO *pldata_out = new PLIO("DataOut", plio_128_bits,"data/fir_output_128b.txt");

simulation::platform<1,1>  plat(pldata_in, pldata_out);
FirGraph FilterChain;

connect<> net0(plat.src[0], FilterChain.in);
connect<> net1(FilterChain.out, plat.sink[0]);

#if defined(__AIESIM__) || defined(__NEW_X86Sim__)

int main(void) {
	FilterChain.init() ;
	FilterChain.run(8);  //N_ITERATIONS);
	FilterChain.end() ;
    return 0 ;
}

#endif


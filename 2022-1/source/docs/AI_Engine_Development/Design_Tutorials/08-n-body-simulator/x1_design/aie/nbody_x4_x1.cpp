/* (c) Copyright 2021 Xilinx, Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 * 		http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

#include "nbody_x4_x1.h"

using namespace adf;
#define COL_OFFSET 0

PLIO *in_i  = new PLIO("in_i",  adf::plio_32_bits, "../../data/input_i0.txt", 400);
PLIO *in_j  = new PLIO("in_j",  adf::plio_32_bits, "../../data/input_j.txt", 400);
PLIO *out_i = new PLIO("out_i", adf::plio_32_bits, "./data/output_i0.txt", 400);

nbodySystem<COL_OFFSET> myGraph;

simulation::platform<2,1> platform(in_i, in_j, out_i);


connect<> neti0(platform.src[0], myGraph.in[0]);
connect<> neti1(platform.src[1], myGraph.in[1]);
connect<> neto0(myGraph.out[0],  platform.sink[0]);


#ifdef __AIESIM__

int main(void) {
  myGraph.init();
  myGraph.run(1);
  myGraph.end();
  return 0;
}

#endif

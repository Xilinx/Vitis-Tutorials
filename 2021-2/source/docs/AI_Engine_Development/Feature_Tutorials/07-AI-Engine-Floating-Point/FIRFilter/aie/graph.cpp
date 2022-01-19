/*
 * (c) Copyright 2020–2021 Xilinx, Inc.. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "graph.h"

PLIO *in0 = new PLIO("DataIn0", plio_64_bits, "../data/inputf.txt");
PLIO *in1 = new PLIO("DataIn1", plio_64_bits, "../data/inputcf.txt");
PLIO *out0 = new PLIO("DataOut0", plio_64_bits, "foutput.txt");
PLIO *out1 = new PLIO("DataOut1", plio_64_bits, "cfoutput.txt");

simulation::platform<2,2> P(in0,in1,out0,out1);

TopGraph G;

connect<> net10(P.src[0], G.in[0]);
connect<> net11(P.src[1], G.in[1]);
connect<> net20(G.out[0], P.sink[0]);
connect<> net21(G.out[1], P.sink[1]);

#if defined(__AIESIM__) || defined(__X86SIM__)
   int main(int argc, char ** argv)
   {
      G.init();
      G.run(NITERATIONS);
      G.end();

      return 0;
   }
#endif

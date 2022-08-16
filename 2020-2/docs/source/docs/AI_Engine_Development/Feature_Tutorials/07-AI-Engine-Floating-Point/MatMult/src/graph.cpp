/*
 * (c) Copyright 2020 Xilinx, Inc. All rights reserved.
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

PLIO *in0 = new PLIO("DataIn0", plio_64_bits, "../data/inputa_float.txt");
PLIO *in1 = new PLIO("DataIn1", plio_64_bits, "../data/inputb_float.txt");
PLIO *in2 = new PLIO("DataIn2", plio_64_bits, "../data/inputa_cfloat.txt");
PLIO *in3 = new PLIO("DataIn3", plio_64_bits, "../data/inputb_cfloat.txt");

PLIO *out0 = new PLIO("DataOut0", plio_64_bits, "float_output.txt");
PLIO *out1 = new PLIO("DataOut1", plio_64_bits, "cfloat_output.txt");
PLIO *out2 = new PLIO("DataOut2", plio_64_bits, "cfloat_conf_output.txt");
PLIO *out3 = new PLIO("DataOut3", plio_64_bits, "cfloat_conj_output.txt");

simulation::platform<4,4> P(in0,in1,in2,in3,out0,out1,out2,out3);

TopGraph G;

connect<> net10(P.src[0], G.in[0]);
connect<> net11(P.src[1], G.in[1]);
connect<> net12(P.src[2], G.in[2]);
connect<> net13(P.src[3], G.in[3]);

connect<> net20(G.out[0], P.sink[0]);
connect<> net21(G.out[1], P.sink[1]);
connect<> net22(G.out[2], P.sink[2]);
connect<> net23(G.out[3], P.sink[3]);

#ifdef __AIESIM__
   int main(int argc, char ** argv)
   {
      G.init();
      G.run(2);
      G.end();

      return 0;
   }
#endif

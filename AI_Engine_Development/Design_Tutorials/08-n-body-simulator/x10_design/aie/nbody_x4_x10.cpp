/*Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "nbody_x4_x10.h"

using namespace adf;
#define COL_OFFSET 0


//input j PLIO
PLIO *in_j  = new PLIO("in_j",  adf::plio_32_bits, "../../data/input_j.txt", 400);
        
//input i PLIOs
PLIO *in_i0  = new PLIO("in_i0",  adf::plio_32_bits, "../../data/input_i0.txt", 400);
PLIO *in_i1  = new PLIO("in_i1",  adf::plio_32_bits, "../../data/input_i1.txt", 400);
PLIO *in_i2  = new PLIO("in_i2",  adf::plio_32_bits, "../../data/input_i2.txt", 400);
PLIO *in_i3  = new PLIO("in_i3",  adf::plio_32_bits, "../../data/input_i3.txt", 400);
PLIO *in_i4  = new PLIO("in_i4",  adf::plio_32_bits, "../../data/input_i4.txt", 400);
PLIO *in_i5  = new PLIO("in_i5",  adf::plio_32_bits, "../../data/input_i5.txt", 400);
PLIO *in_i6  = new PLIO("in_i6",  adf::plio_32_bits, "../../data/input_i6.txt", 400);
PLIO *in_i7  = new PLIO("in_i7",  adf::plio_32_bits, "../../data/input_i7.txt", 400);
PLIO *in_i8  = new PLIO("in_i8",  adf::plio_32_bits, "../../data/input_i8.txt", 400);
PLIO *in_i9  = new PLIO("in_i9",  adf::plio_32_bits, "../../data/input_i9.txt", 400);

//output PLIOs
PLIO *out_i0 = new PLIO("out_i0", adf::plio_32_bits, "./data/output_i0.txt", 400);
PLIO *out_i1 = new PLIO("out_i1", adf::plio_32_bits, "./data/output_i1.txt", 400);
PLIO *out_i2 = new PLIO("out_i2", adf::plio_32_bits, "./data/output_i2.txt", 400);
PLIO *out_i3 = new PLIO("out_i3", adf::plio_32_bits, "./data/output_i3.txt", 400);
PLIO *out_i4 = new PLIO("out_i4", adf::plio_32_bits, "./data/output_i4.txt", 400);
PLIO *out_i5 = new PLIO("out_i5", adf::plio_32_bits, "./data/output_i5.txt", 400);
PLIO *out_i6 = new PLIO("out_i6", adf::plio_32_bits, "./data/output_i6.txt", 400);
PLIO *out_i7 = new PLIO("out_i7", adf::plio_32_bits, "./data/output_i7.txt", 400);
PLIO *out_i8 = new PLIO("out_i8", adf::plio_32_bits, "./data/output_i8.txt", 400);
PLIO *out_i9 = new PLIO("out_i9", adf::plio_32_bits, "./data/output_i9.txt", 400);

nbodySystem<COL_OFFSET> myGraph;

simulation::platform<11,10> platform(in_j,
                                   in_i0,
                                   in_i1,
                                   in_i2,
                                   in_i3,
                                   in_i4,
                                   in_i5,
                                   in_i6,
                                   in_i7,
                                   in_i8,
                                   in_i9,
                                   out_i0,
                                   out_i1,
                                   out_i2,
                                   out_i3,
                                   out_i4,
                                   out_i5,
                                   out_i6,
                                   out_i7,
                                   out_i8,
                                   out_i9);

//Connect platform to graph's j input
connect<> netij(platform.src[0], myGraph.in_j);

//Connect platform to graph's i inputs
connect<> netii0(platform.src[1], myGraph.in_i[0]);
connect<> netii1(platform.src[2], myGraph.in_i[1]);
connect<> netii2(platform.src[3], myGraph.in_i[2]);
connect<> netii3(platform.src[4], myGraph.in_i[3]);
connect<> netii4(platform.src[5], myGraph.in_i[4]);
connect<> netii5(platform.src[6], myGraph.in_i[5]);
connect<> netii6(platform.src[7], myGraph.in_i[6]);
connect<> netii7(platform.src[8], myGraph.in_i[7]);
connect<> netii8(platform.src[9], myGraph.in_i[8]);
connect<> netii9(platform.src[10], myGraph.in_i[9]);

//Connect platform to graph's outputs 
connect<> neto0(myGraph.out[0],  platform.sink[0]);
connect<> neto1(myGraph.out[1],  platform.sink[1]);
connect<> neto2(myGraph.out[2],  platform.sink[2]);
connect<> neto3(myGraph.out[3],  platform.sink[3]);
connect<> neto4(myGraph.out[4],  platform.sink[4]);
connect<> neto5(myGraph.out[5],  platform.sink[5]);
connect<> neto6(myGraph.out[6],  platform.sink[6]);
connect<> neto7(myGraph.out[7],  platform.sink[7]);
connect<> neto8(myGraph.out[8],  platform.sink[8]);
connect<> neto9(myGraph.out[9],  platform.sink[9]);
#ifdef __AIESIM__

int main(void) {
  myGraph.init();
  myGraph.run(1);
  myGraph.end();
  return 0;
}

#endif

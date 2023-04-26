/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
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

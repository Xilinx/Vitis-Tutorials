//
// Copyright (C) 2020-2022, Xilinx, Inc. All rights reserved.
// Copyright (C) 2022-2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//
// Author Derek Hagen

#include "graphs/mygraph2.h"



mygraph<256, 16, 4>  mygraph_top;

#if defined (__AIESIM__) || (__X86SIM__)
// initialize and run the dataflow graph
int main(void) 
{
  //int16 coe0[16] {
  //  0, 0, 0, 0, 16384, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  //};
  //int16 coe1[16] {
  //  0, 0, 0, 0, 16384, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  //};

  int16 coe0[16] {
    -41,
    -173,
    -399,
    -348,
    666,
    2956,
    5852,
    7902,
    7902,
    5852,
    2956,
    666,
    -348,
    -399,
    -173,
    -41
  };
  int16 coe1[16] {
    82,
    -213,
    368,
    -0,
    -1570,
    4301,
    -7035,
    8192,
    -7035,
    4301,
    -1570,
    0,
    368,
    -213,
    82,
    0
  };

  mygraph_top.init();
  mygraph_top.update(mygraph_top.coeff[0], coe0, 16);
  mygraph_top.update(mygraph_top.coeff[1], coe1, 16);
  mygraph_top.run();
  mygraph_top.end();
  return 0;
}
#endif


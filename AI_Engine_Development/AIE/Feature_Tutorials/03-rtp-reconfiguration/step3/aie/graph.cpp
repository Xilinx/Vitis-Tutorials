/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "graph.h"

adaptive_graph gr;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char **argv)
{
  int narrow_filter[12] = {180, 89, -80, -391, -720, -834, -478, 505, 2063, 3896, 5535, 6504};
  int wide_filter[12] = {-21, -249, 319, -78, -511, 977, -610, -844, 2574, -2754, -1066, 18539};

  gr.init();
  //run for 16 iterations, update narrow filter coefficients, wait, update wide filter coefficients, run for 16 iterations
  gr.update(gr.coefficients, narrow_filter, 12);
  gr.run(16);
  gr.wait();
  gr.update(gr.coefficients, wide_filter, 12);
  gr.run(16);
  gr.end();
  return 0;
};
#endif




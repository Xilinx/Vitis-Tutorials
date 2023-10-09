/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
#include "graphs/mygraph.h"
using namespace adf;

mygraph<WIN_SAMPS>  mygraph_top;

// initialize and run the dataflow graph
#if defined(__AIESIM__) || defined(__X86SIM__)
int main(void) 
{
  mygraph_top.init();
  mygraph_top.run(1);
  mygraph_top.end();
  return 0;
}
#endif


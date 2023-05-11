//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

#include "graphs/mygraph.h"



mygraph<512, 0, 4>  mygraph_top;

#ifdef __AIESIM__
// initialize and run the dataflow graph
int main(void) 
{
  mygraph_top.init();
  mygraph_top.run();
  mygraph_top.end();
  return 0;
}
#endif


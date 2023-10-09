/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "graph.h"

converter mygraph;
#if defined(__AIESIM__) || defined(__X86SIM__)
// run the dataflow graph
int main(void) {
   mygraph.init();
   mygraph.run(7); //run for 7 iterations
   mygraph.end();  //Wait for graph to complete 7 iterations
   return 0;
}
#endif

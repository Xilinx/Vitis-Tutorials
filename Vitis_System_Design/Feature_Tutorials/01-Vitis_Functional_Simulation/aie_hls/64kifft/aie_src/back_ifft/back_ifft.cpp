//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "back_ifft.h"

// instantiate ADF graph
Back_ifft mygraph;

// initialize and run the dataflow graph
#if defined(__AIESIM__) || defined(__X86SIM__)
int main(void) {
   mygraph.init();
   mygraph.run();
   mygraph.end();
   return 0;
}
#endif

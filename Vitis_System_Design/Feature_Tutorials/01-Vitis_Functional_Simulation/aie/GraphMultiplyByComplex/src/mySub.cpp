//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "mySub.h"

// instantiate ADF graph
MySub mygraph;

// To generate input and reference output data files,
// select 'Create testbench' option on the Model Composer Hub block.
// initialize and run the dataflow graph
#if defined(__AIESIM__) || defined(__X86SIM__)
int main(void) {
   mygraph.init();
   mygraph.run();
   mygraph.end();
   return 0;
}
#endif

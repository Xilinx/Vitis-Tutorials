//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "AIE_subsystem.h"
#include "XmcRuntimeParamHelper.h"

// instantiate ADF graph
AIE_subsystem mygraph;

// initialize and run the dataflow graph
#if defined(__AIESIM__) || defined(__X86SIM__)
int main(void) {
   int32 In2[12] { };
   if (!XMCRuntimeParamHelper<int32>::getInput("./data/input/In2.txt", In2, 12)) {
      std::cerr << "main(): Error reading input data file " << "./data/input/In2.txt" << std::endl;
   }
   mygraph.init();
   mygraph.run();
   mygraph.update(mygraph.In2, In2, 12);
   mygraph.end();
   return 0;
}
#endif

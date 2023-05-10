/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "graph.h"

TopGraph G;

#if defined(__AIESIM__) || defined(__X86SIM__)
   int main(int argc, char ** argv)
   {
      G.init();
      G.run(NITERATIONS);
      G.end();

      return 0;
   }
#endif

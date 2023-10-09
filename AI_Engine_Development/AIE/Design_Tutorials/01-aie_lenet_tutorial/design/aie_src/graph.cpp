/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/


#include "graph.h"

myGraph g;


#ifdef __AIESIM__
   int main(int argc, char ** argv)
   { 
      g.init();
      g.run(100);
      g.end();
     
      return 0;
   }
#endif


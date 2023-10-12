// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT


#include "graph.h"

// Less or equal to 8
#define NK 4
BroadcastGraph<NK> G;

int main(int argc, char ** argv) {

  G.init();
  G.run(NIterations);
  G.end();
  return 0;
}

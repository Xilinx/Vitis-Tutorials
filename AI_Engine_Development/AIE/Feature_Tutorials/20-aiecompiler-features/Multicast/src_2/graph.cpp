// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT


#include "graph.h"

testcase::BroadcastGraph graph1;


int main(int argc, char ** argv) {

  graph1.init();
  graph1.run(NIterations);
  graph1.end();
  return 0;
}

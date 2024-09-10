/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
#include <adf.h>
#include "k_1_buff_i_1_buff_o.h"
#include "graph.h"

using namespace adf;

my_graph mygraph;

#if defined(__AIESIM__) || defined(__X86SIM__)

int main(void) {
  mygraph.init();
  mygraph.run(4);
  mygraph.end();
  return 0;
}

#endif

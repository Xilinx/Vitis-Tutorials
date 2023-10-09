/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "graph.h"

using namespace adf;

mygraph gr;

#if defined(__AIESIM__) || defined(__ADF_FRONTEND__)
int main(int argc, char ** argv) {
  gr.init();
  gr.run(2);
  gr.end();
  return 0;
}
#endif

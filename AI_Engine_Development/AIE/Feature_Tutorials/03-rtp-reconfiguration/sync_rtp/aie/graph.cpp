/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <adf.h>
#include "graph.h"
#include <unistd.h>

using namespace adf;

rtpgraph gr;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char ** argv) { 

  gr.init();
  gr.run(4);

  //synchronous RTP value calls
  gr.update(gr.value,10);
  gr.update(gr.value,100);
  gr.update(gr.value,1000);
  gr.update(gr.value,10000);

  gr.end();
  return 0;
};
#endif

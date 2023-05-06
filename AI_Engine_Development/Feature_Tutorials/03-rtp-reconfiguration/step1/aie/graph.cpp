/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <adf.h>
#include "graph.h"
#include <unistd.h>

using namespace adf;

ddsgraph gr;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char ** argv) { 

  gr.init();
  gr.run(4);

  //synchronous RTP trigger calls
  gr.update(gr.trigger,10);
  gr.update(gr.trigger,10);
  gr.update(gr.trigger,100);
  gr.update(gr.trigger,100);

  gr.end();
  return 0;
};
#endif

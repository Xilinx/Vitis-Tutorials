//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <string>
#include <iostream>
#include <fstream>

#include "sar_top_8engine_cfg.h"
#include "sar_top_1engine_graph.h"

// Instantiate AIE graph:
sar_top_1engine_graph<REPEAT,NSAMP,0> engine0;
sar_top_1engine_graph<REPEAT,NSAMP,1> engine1;
sar_top_1engine_graph<REPEAT,NSAMP,2> engine2;
sar_top_1engine_graph<REPEAT,NSAMP,3> engine3;
sar_top_1engine_graph<REPEAT,NSAMP,4> engine4;
sar_top_1engine_graph<REPEAT,NSAMP,5> engine5;
sar_top_1engine_graph<REPEAT,NSAMP,6> engine6;
sar_top_1engine_graph<REPEAT,NSAMP,7> engine7;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

// This is dummy code. We don't use AIE simulation for 8-engine design, use HW Emulation instead
int main(void)
{
  engine0.init();
  engine1.init();
  engine2.init();
  engine3.init();
  engine4.init();
  engine5.init();
  engine6.init();
  engine7.init();

  engine0.end();
  engine1.end();
  engine2.end();
  engine3.end();
  engine4.end();
  engine5.end();
  engine6.end();
  engine7.end();

  bool flag = 0;
  if ( flag == 0 )
    std::cout << "--- PASSED ---" << std::endl;
  else
    std::cout << "*** FAILED ***" << std::endl;

  return (flag);
}

/*
#Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier:MIT
#*/
#include <stdio.h>
#include <adf.h>

void mysquare(input_window_int32 * win, 
              output_window_int32 *out) { 

  
  for (unsigned i = 0 ; i < 32 ; i++) {
    int j = window_readincr(win);
    window_writeincr(out,j*j);
  }

}

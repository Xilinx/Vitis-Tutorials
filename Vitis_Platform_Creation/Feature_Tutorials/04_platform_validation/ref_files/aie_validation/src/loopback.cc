/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <adf.h>
#include "kernels.h"

static int32 myGlobal[32] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};

void loopback(input_window_int32 * in_buf, output_window_int32 * buf, int32 myRtp ) {
  int32 i;
  for (i=0;i<32;i++) {
    int k = window_readincr(in_buf);
	
	if constexpr (is_rtp) {
		int val = k + myGlobal[i];
		window_writeincr(buf, (val >> myRtp));
	} else {
		window_writeincr(buf, (k >> myRtp));		
	}
  }
};


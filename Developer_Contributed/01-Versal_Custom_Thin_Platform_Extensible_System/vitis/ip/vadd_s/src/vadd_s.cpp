//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

#define IO_TYPE uint16

const int MAX_SAMPLES = 1024;

void vadd_s(
  uint32  samples,
  IO_TYPE *in0,
  IO_TYPE *in1,
  IO_TYPE *out
) {

#pragma HLS INTERFACE mode=axis register_mode=both depth=MAX_SAMPLES port=in0     register
#pragma HLS INTERFACE mode=axis register_mode=both depth=MAX_SAMPLES port=in1     register
#pragma HLS INTERFACE mode=axis register_mode=both depth=MAX_SAMPLES port=out     register
#pragma HLS INTERFACE s_axilite                                      port=samples bundle=CONTROL
#pragma HLS INTERFACE s_axilite                                      port=return  bundle=CONTROL

  for(int i = 0 ; i < samples ; i += 1) {
    out[i] = in0[i] + in1[i];
  }
}

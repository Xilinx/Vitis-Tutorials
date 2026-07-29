//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//
#include "axis1to2.h"
#include <math.h>

/**
 * @brief HLS
 * @param in0 direction=input protocol=Default length=8
 * @param in1 direction=input protocol=Default length=8
 * @param out0 direction=output protocol=Default length=8
 */

void axis1to2(
  hls::stream< std::complex<data_t> >& in0,
  hls::stream< std::complex<data_t> >& out0,
  hls::stream< std::complex<data_t> >& out1
) {
  #pragma HLS INTERFACE axis port=in0
  #pragma HLS INTERFACE axis port=out0
  #pragma HLS INTERFACE axis port=out1
  #pragma HLS INTERFACE ap_ctrl_none port=return
  #pragma HLS dataflow

  for(int i = 0 ; i < 8 ; i ++) {
    std::complex<data_t> tmp = in0.read();
    out0.write(tmp);
    out1.write(tmp);
  }
}

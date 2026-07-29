//
// Copyright (C) 2022-2022, Xilinx, Inc. All rights reserved.
// Copyright (C) 2022-2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//
#include "vadd_s.h"
#include <math.h>

/**
 * @brief HLS
 * @param in0 direction=input protocol=Default length=8
 * @param in1 direction=input protocol=Default length=8
 * @param out0 direction=output protocol=Default length=8
 */

void vadd_s(
  hls::stream< std::complex<data_t> >& in0,
  hls::stream< std::complex<data_t> >& in1,
  hls::stream< std::complex<data_t> >& out0
) {
  #pragma HLS INTERFACE axis port=in0
  #pragma HLS INTERFACE axis port=in1
  #pragma HLS INTERFACE axis port=out0
  #pragma HLS INTERFACE ap_ctrl_none port=return
  #pragma HLS dataflow

  for(int i = 0 ; i < 8 ; i ++) {
    out0.write(in0.read() + in1.read());
  }
}

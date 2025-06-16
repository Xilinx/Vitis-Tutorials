//
// Copyright (C) 2020-2022, Xilinx, Inc. All rights reserved.
// Copyright (C) 2022-2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//
#ifndef HLS_H_
#define HLS_H_

#include <cstdint>
#include <complex>
#include <hls_stream.h>

typedef short data_t;

void vadd_s(
  hls::stream< std::complex<data_t> >& in0,
  hls::stream< std::complex<data_t> >& in1,
  hls::stream< std::complex<data_t> >& out0
);

#endif


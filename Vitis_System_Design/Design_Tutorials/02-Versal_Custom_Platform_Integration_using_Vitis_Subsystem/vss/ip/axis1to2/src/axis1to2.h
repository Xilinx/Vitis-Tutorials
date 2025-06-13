//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//
#ifndef HLS_H_
#define HLS_H_

#include <cstdint>
#include <complex>
#include <hls_stream.h>

typedef short data_t;

void axis1to2(
  hls::stream< std::complex<data_t> >& in0,
  hls::stream< std::complex<data_t> >& out0,
  hls::stream< std::complex<data_t> >& out1
);

#endif


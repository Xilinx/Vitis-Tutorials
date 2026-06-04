//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __HLS_KERNELS_H__
#define __HLS_KERNELS_H__

#include <hls_stream.h>

void pl_func(hls::stream<float>  &in1,
             hls::stream<double> &in2, 
             hls::stream<float>  &out1,
             hls::stream<double> &out2);

#endif

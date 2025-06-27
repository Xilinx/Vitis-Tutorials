//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __HLS_KERNELS_H__
#define __HLS_KERNELS_H__

#include <hls_stream.h>
#include <ap_fixed.h>

void pl_func(int *in1, 
             int (&in2)[5],
             int *out1,
             int (&out2)[5]);

#endif

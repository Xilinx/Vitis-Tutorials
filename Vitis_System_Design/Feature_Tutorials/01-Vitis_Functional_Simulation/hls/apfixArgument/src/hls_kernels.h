//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __HLS_KERNELS_H__
#define __HLS_KERNELS_H__

#include <ap_fixed.h>

void pl_func(const ap_ufixed<32,15> *in1, 
             const ap_fixed<64,24> *in2,
             ap_ufixed<32,15> *out1, 
             ap_fixed<64,24> *out2);

#endif

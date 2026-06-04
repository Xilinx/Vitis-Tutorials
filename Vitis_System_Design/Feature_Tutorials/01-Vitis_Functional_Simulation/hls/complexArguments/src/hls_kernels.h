//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __HLS_KERNELS_H__
#define __HLS_KERNELS_H__

#include <complex>

void pl_func(const std::complex<uint32_t> *in1, 
             const std::complex<float> *in2,
             std::complex<uint32_t> *out1, 
             std::complex<float> *out2);

#endif

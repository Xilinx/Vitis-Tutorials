//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <complex>

void pl_func(const std::complex<uint32_t> *in1, 
             const std::complex<float> *in2,
             std::complex<uint32_t> *out1, 
             std::complex<float> *out2)
{
#pragma HLS INTERFACE ap_fifo depth=5 port=in1,in2,out1,out2
    for (int i=0; i<5; i++)
    {   
        out1[i] = in1[i];
        out2[i] = in2[i];
    }
}

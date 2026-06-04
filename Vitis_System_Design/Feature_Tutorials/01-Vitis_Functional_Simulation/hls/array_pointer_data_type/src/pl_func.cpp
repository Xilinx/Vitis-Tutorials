//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <ap_fixed.h>
#include <hls_stream.h>

void pl_func(int *in1, 
             int (&in2)[5],
             int *out1,
             int (&out2)[5])
{
    #pragma HLS interface mode=ap_memory port=in1 depth=5
    #pragma HLS interface mode=ap_memory port=out1 depth=5
    for (int i=0; i<5; i++)
    {
        //std::cerr << "DEBUG SSCHURMA i=" << i << " in1[i]=" << in1[i] << " in2[i]=" << in2[i] << std::endl;
        out1[i] = in1[i] * 2;
        out2[i] = in2[i] * 2;
    }
}

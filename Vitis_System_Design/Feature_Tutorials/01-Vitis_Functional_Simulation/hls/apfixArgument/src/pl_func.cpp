//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <ap_fixed.h>
#include <hls_stream.h>

void pl_func(const ap_ufixed<32,15> *in1, 
             const ap_fixed<64,24> *in2,
             ap_ufixed<32,15> *out1, 
             ap_fixed<64,24> *out2)
{
#pragma HLS interface mode=ap_memory depth=5 port=in1,in2
#pragma HLS interface mode=ap_memory depth=5 port=out1,out2
    for (int i=0; i<5; i++)
    {   
        out1[i] = in1[i];
        out2[i] = in2[i];
    }
}

//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <ap_fixed.h>
#include <hls_stream.h>

void pl_func(hls::stream<float>  &in1,
             hls::stream<double> &in2, 
             hls::stream<float>  &out1,
             hls::stream<double> &out2)
{
    for (int i=0; i<5; i++)
    {   
        out1.write(in1.read() * 2.0);
        out2.write(in2.read() * 2.0);
    }
}

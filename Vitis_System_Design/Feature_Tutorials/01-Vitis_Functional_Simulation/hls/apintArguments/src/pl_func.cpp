//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "hls_kernels.h"

#define NUM_SAMPLES 5
void pl_func(hls::stream<ap_int<BITWIDTH> > &in1 ,
             hls::stream<ap_uint<BITWIDTH> > &in2 ,
             hls::stream<ap_int<BITWIDTH> > &out1,
             hls::stream<ap_uint<BITWIDTH> > &out2)
{
    ap_int<BITWIDTH> value1;
    ap_uint<BITWIDTH> value2;
    for (int i=0; i<NUM_SAMPLES; i++)
    {
        #pragma HLS PIPELINE II=1 
        in1.read(value1);
        value1 *= 2;
        out1.write(value1);
        
        in2.read(value2);
        value2 += 10;
        out2.write(value2);
    }    
}

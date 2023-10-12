// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "adf.h"

void f0(input_stream<int32_t> *in0, output_stream<int32_t> *out0)
{
    for(int i=0 ; i<32 ; i++) {
	int val = readincr(in0);
    writeincr(out0, val);
}
}
void f1(input_stream<int32_t> *in0, output_stream<int32_t> *out0,
        int32_t in1, int32_t &out1)
{
	int oval1;

    for(int i=0 ; i<32 ; i++) {
    int val = readincr(in0);
    int oval0 = in1 + val;
     oval1 = in1;
    writeincr(out0, oval0);
	}
    out1 = oval1;
}

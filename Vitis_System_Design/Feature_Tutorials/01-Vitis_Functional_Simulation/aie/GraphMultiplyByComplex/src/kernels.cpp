//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "kernels.h"

void
complex_in_out(input_stream<cint16> *i1,
               output_stream<cint16> *o1)
{
    int16 r;
    int16 i;
    cint16 in, out;
    in = readincr(i1);
    out.real = in.real*2;
    out.imag = in.imag*3;
    writeincr(o1, out);
}
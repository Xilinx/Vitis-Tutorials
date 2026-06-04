//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "kernels.h"

__attribute__((noinline))
    void myFunc_1df(adf::input_buffer<bfloat16,adf::extents< NUM_SAMPLES>>  & __restrict i1,
                    adf::output_buffer<bfloat16,adf::extents< NUM_SAMPLES>> & __restrict o1)
{
    bfloat16 *__restrict pi1 = i1.data();
    bfloat16 *__restrict po1 = o1.data();
   // v2cint16* intptr = (v2cint16*)pi1; 
   // v2cint16* outptr = (v2cint16*)po1; 
    for(int i = 0 ; i < 64; i++)
    {
        po1[i] = pi1[i]; 

    }
}

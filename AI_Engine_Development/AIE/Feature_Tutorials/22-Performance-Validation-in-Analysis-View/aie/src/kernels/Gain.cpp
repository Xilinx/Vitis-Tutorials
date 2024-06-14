/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "../kernels.h"

void gain(adf::input_buffer<cint16,adf::extents<BUFFER_SIZE>> & in,
adf::output_buffer<cint16,adf::extents<BUFFER_SIZE>> & out,
cint16 Gain)
{
    auto pin = aie::begin_vector<8>(in);
    auto pout = aie::begin_vector<8>(out);

    for(int i=0;i<BUFFER_SIZE/8;i++)
    {
        auto prod = mul(*pin++,Gain);
        *pout++ = prod.to_vector<cint16>(0);
    }
}

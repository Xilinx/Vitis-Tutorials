// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "kernels.h"


template<int N>
void upsample_buffer(adf::input_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict in, adf::output_buffer<cint16, adf::extents<N*BUFFER_SIZE>> &__restrict out)
{
    auto pi1 = aie::begin_vector<8>(in);
    auto po1 = aie::begin_vector<8>(out);

    // Copy input to output
    for (int i = 0; i < BUFFER_SIZE/8 ; i++)
    chess_prepare_for_pipelining
    {
        auto reg = *pi1++;
        for(int j=0;j<N;j++)
        chess_flatten_loop
        {
            *po1++ = *pi1++;
        }
    }

}

// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "kernels.h"


template<int FrameLength,int VectorLength>
void passthrough_buffer(adf::input_buffer<int32, adf::extents<FrameLength>> &__restrict in, adf::output_buffer<int32, adf::extents<FrameLength>> &__restrict out)
{
    int32 i;
    int32 FrameLengthOut = out.size();

    auto pi1 = aie::begin_vector<VectorLength>(in);
    auto po1 = aie::begin_vector<VectorLength>(out);

    // Copy input to output
    for (i = 0; i < FrameLengthOut / VectorLength; i++)
        chess_prepare_for_pipelining
        {
            *po1++ = *pi1++;
        }
}


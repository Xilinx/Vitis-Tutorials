// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "kernels.h"

int dinit[] = {0, 1, 2, 3, 4, 5, 6, 7};

void UpConv_7_5_iobuf(adf::input_buffer<int32, adf::extents<adf::inherited_extent>> &__restrict in, adf::output_buffer<int32, adf::extents<adf::inherited_extent>> &__restrict out)
{
    int32 i;
    int32 FrameLengthOut = out.size();
    int32 FrameLengthIn = in.size();

    auto pi1 = aie::begin_vector<VECTOR_LENGTH>(in);
    auto po1 = aie::begin_vector<VECTOR_LENGTH>(out);
    aie::vector<int,VECTOR_LENGTH> d = aie::load_v<8>(dinit);

    // Copy input
    for (i = 0; i < FrameLengthIn/VECTOR_LENGTH ; i++)
    chess_prepare_for_pipelining
    {
        *po1++ = *pi1++;
    }

    // Extrapolate
    for (i = FrameLengthIn/VECTOR_LENGTH; i < FrameLengthOut/VECTOR_LENGTH ; i++)
    chess_prepare_for_pipelining
    {
        *po1++ = aie::add(d,i*VECTOR_LENGTH);
    }
}

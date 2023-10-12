// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "kernels.h"

int dinit[] = {0, 1, 2, 3, 4, 5, 6, 7};

template<int FrameLengthIn>
void UpConv_7_5_stream(input_stream<int32> *__restrict in, output_stream<int32> *__restrict out)
{
    int32 i;
    const int32 FrameLengthOut = FrameLengthIn*7/5;

    aie::vector<int,VECTOR_LENGTH> d;

    for (i = 0; i < FrameLengthOut/VECTOR_LENGTH ; i++)
	chess_prepare_for_pipelining
    {
        if(i<FrameLengthIn/VECTOR_LENGTH)
            d = readincr_v<VECTOR_LENGTH>(in);
        else
            d = aie::add(d,VECTOR_LENGTH);

        writeincr(out,d);
    }

}

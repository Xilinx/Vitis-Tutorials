// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "kernels.h"



template<int FrameLengthIn>
void DownConv_5_7_stream(input_stream<int32> *__restrict in, output_stream<int32> *__restrict out)
{
    int32 i;
    const int32 FrameLengthOut = FrameLengthIn*5/7;

    for (i = 0; i < FrameLengthIn/VECTOR_LENGTH ; i++)
	chess_prepare_for_pipelining
    {
        auto d = readincr_v<VECTOR_LENGTH>(in);
        if(i<FrameLengthOut/VECTOR_LENGTH)  writeincr(out,d);
    }

}

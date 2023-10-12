// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "kernels.h"

template<int FrameLength>
void Subtract_stream(input_stream<int32> *__restrict in1, input_stream<int32> *__restrict in2, output_stream<int32> *__restrict out)
{
    int32 i;

    aie::vector<int,VECTOR_LENGTH> d1 ;
    aie::vector<int,VECTOR_LENGTH> d2 ;

    for (i = 0; i < FrameLength/VECTOR_LENGTH ; i++)
	chess_prepare_for_pipelining
    {
         d1 = readincr_v<VECTOR_LENGTH>(in1);
         d2 = readincr_v<VECTOR_LENGTH>(in2);
        writeincr(out,aie::sub(d1,d2));
    }

}

// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "kernels.h"


void passthrough_buffer(adf::input_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict in, adf::output_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict out)
{
    auto pi1 = aie::begin_vector<8>(in);
    auto po1 = aie::begin_vector<8>(out);

    // Copy input to output
    for (int i = 0; i < BUFFER_SIZE/8 ; i++)
    chess_prepare_for_pipelining
    {
        *po1++ = *pi1++;
    }

}

#if ARCH_AIECMP == 10

void passthrough_stream(input_stream<cint16> *__restrict in, output_stream<cint16> *__restrict out)
{
    for (int i = 0; i < BUFFER_SIZE/4 ; i++)
	chess_prepare_for_pipelining
    {
        writeincr(out,readincr_v<4>(in));
    }
}

void passthrough_bufstr(adf::input_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict in, output_stream<cint16> *__restrict out)
{
    auto pi1 = aie::begin_vector<4>(in);

    // Copy input to output
    for (int i = 0; i < BUFFER_SIZE/4 ; i++)
    chess_prepare_for_pipelining
    {
        auto inval = *pi1++;
        writeincr(out,inval);
    }

}

void passthrough_strbuf(input_stream<cint16> *__restrict in, adf::output_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict out)
{
    auto po1 = aie::begin_vector<4>(out);

    // Copy input to output
    for (int i = 0; i < BUFFER_SIZE/4 ; i++)
    chess_prepare_for_pipelining
    {
        auto inval = readincr_v<4>(in);
        *po1++ = inval;
    }

}

#elif ARCH_AIECMP == 20

void passthrough_stream(input_stream<cint16> *__restrict in, output_stream<cint16> *__restrict out)
{
    for (int i = 0; i < BUFFER_SIZE ; i++)
	chess_prepare_for_pipelining
    {
        writeincr(out,readincr(in));
    }
}

void passthrough_bufstr(adf::input_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict in, output_stream<cint16> *__restrict out)
{
    auto pi1 = aie::begin(in);

    // Copy input to output
    for (int i = 0; i < BUFFER_SIZE ; i++)
    chess_prepare_for_pipelining
    {
        auto inval = *pi1++;
        writeincr(out,inval);
    }

}

void passthrough_strbuf(input_stream<cint16> *__restrict in, adf::output_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict out)
{
    auto po1 = aie::begin(out);

    // Copy input to output
    for (int i = 0; i < BUFFER_SIZE ; i++)
    chess_prepare_for_pipelining
    {
        auto inval = readincr(in);
        *po1++ = inval;
    }

}
#endif

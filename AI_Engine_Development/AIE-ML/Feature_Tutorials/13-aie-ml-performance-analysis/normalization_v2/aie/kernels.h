/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef COPY_MODULE_H
#define COPY_MODULE_H

#include <adf.h>
using namespace adf;
template<int COL, int ROW, int REPEAT>
__attribute__((noinline)) void mean(input_buffer<bfloat16> & __restrict data,output_async_buffer<bfloat16> & __restrict out);

template<int COL, int ROW, int REPEAT>
__attribute__((noinline)) void deviation(input_buffer<bfloat16> & __restrict data, input_async_buffer<bfloat16> & __restrict mean, output_async_buffer<bfloat16> & __restrict out);

template<int COL, int ROW, int REPEAT>
__attribute__((noinline)) void norm(input_buffer<bfloat16> & __restrict data,input_async_buffer<bfloat16> & __restrict mean_deviation, output_buffer<bfloat16> & __restrict out);
#endif

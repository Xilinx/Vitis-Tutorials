/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef COPY_MODULE_H
#define COPY_MODULE_H

#include <adf.h>
using namespace adf;
template<int COL, int ROW>
__attribute__((noinline)) void mean_dev_norm_first(input_buffer<bfloat16> & __restrict data, input_stream<bfloat16> * __restrict mean_dev, output_buffer<bfloat16> & __restrict out, output_cascade<accfloat> * __restrict partial_out);

template<int COL, int ROW>
__attribute__((noinline)) void mean_dev_norm_middle(input_buffer<bfloat16> & __restrict data, input_stream<bfloat16> * __restrict mean_dev, input_cascade<accfloat> * __restrict partial_in, output_buffer<bfloat16> & __restrict out, output_cascade<accfloat> * __restrict partial_out);

template<int COL, int ROW, int NUM>
__attribute__((noinline)) void mean_dev_norm_last(input_buffer<bfloat16> & __restrict data, input_cascade<accfloat> * __restrict partial_in, output_buffer<bfloat16> & __restrict out, output_stream<bfloat16> * __restrict mean_dev_out);

#endif

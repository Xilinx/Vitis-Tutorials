/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <adf.h>
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"

#define PIE_VALUE float(3.1415)
#define NUM_SAMPLES 8
#define BUFFER_SIZE 128
#define THRESHOLD 50

using namespace adf;

void peak_detect(
    input_buffer<int32> & __restrict inx, 
    output_buffer<int32> & __restrict to_next,
    //output_stream_int32 * __restrict outmax,
    output_buffer<float> & __restrict outmin_v
);

void upscale(
    input_buffer<float> & __restrict in_pie,
    //input_stream_int32 * __restrict outmax,
    output_buffer<float> & __restrict out_upscale
);

void data_shuffle(
    input_buffer<int32> & __restrict from_prev,
    //input_stream_int32 * __restrict outmax,
    output_buffer<int32> & __restrict out_shift
);

#endif

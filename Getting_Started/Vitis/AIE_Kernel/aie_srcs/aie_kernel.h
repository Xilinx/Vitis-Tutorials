/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#ifndef __AIE_KERNELS_H__
#define __AIE_KERNELS_H__

#include <adf.h>

#define NUM_SAMPLES 8

void MatMul( adf::input_buffer<int32> & __restrict in1, adf::input_buffer<int32> & __restrict in2,
             adf::output_buffer<int32> & __restrict out_put)  ;

#endif


/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __KERNEL_H__
#define __KERNEL_H__
#include <adf.h>
using namespace adf;
__attribute__ ((noinline)) void aie_dest1(input_buffer<int32,extents<32>> &in, 
        output_stream<int32> *out, output_buffer<int32,extents<32>> &outm);
__attribute__ ((noinline)) void aie_dest2(input_stream<int32> *in, input_buffer<int32,extents<32>> &inm, 
        output_stream<int32> *outm);
#endif

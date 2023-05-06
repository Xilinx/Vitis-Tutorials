/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __KERNEL_H__
#define __KERNEL_H__
#include <adf.h>
using namespace adf;
__attribute__ ((noinline)) void aie_dest1(input_buffer<int32,extents<1024>> & __restrict in, 
        output_stream<int32> * __restrict out, output_buffer<int32,extents<1024>> & __restrict outm);
__attribute__ ((noinline)) void aie_dest2(input_stream<int32> * __restrict in, input_buffer<int32,extents<1024>> & __restrict inm, 
        output_stream<int32> * __restrict outm);
#endif

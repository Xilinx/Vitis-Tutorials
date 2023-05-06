/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __KERNELS_H__
#define __KERNELS_H__
using namespace adf;
void aie_core1(input_buffer<int32,extents<8>> &in,output_buffer<int32,extents<8>> &out);
void aie_core2(input_buffer<int32,extents<8>> &in,output_buffer<int32,extents<8>> &out);
void aie_core3(input_buffer<float,extents<8>> &in,output_buffer<float,extents<8>> &out);
void aie_core4(input_buffer<cint16,extents<8>> &in,output_buffer<cint16,extents<8>> &out);
#endif

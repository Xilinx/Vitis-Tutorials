/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __KERNELS_H__
#define __KERNELS_H__

#include <adf.h>
using namespace adf;
template<int32 NUM>
void vect_add(input_buffer<int32,extents<NUM>>& __restrict in,output_buffer<int32,extents<NUM>>& __restrict out,int32 value);

#endif /* __DDS_H__ */



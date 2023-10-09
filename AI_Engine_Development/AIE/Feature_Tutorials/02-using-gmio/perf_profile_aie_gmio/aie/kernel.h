/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __KERNEL_H__
#define __KERNEL_H__
#include <adf.h>
using namespace adf;
void vec_incr(input_buffer<int32,extents<256>>& data,output_buffer<int32,extents<258>>& out);
#endif

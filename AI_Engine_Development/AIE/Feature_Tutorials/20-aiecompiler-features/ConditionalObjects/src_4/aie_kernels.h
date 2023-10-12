// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#ifndef __AIE_KERNELS_H__
#define __AIE_KERNELS_H__

#include <adf.h>

void f0(input_stream<int32_t> *, output_stream<int32_t> *);
void f1(input_stream<int32_t> *, output_stream<int32_t> *, int32_t, int32_t &);

#endif

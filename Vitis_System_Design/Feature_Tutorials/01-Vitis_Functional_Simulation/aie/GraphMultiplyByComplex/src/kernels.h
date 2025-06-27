//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef _AIE_KERNELS_CFLOAT_KERNELS_KERNELS_H_
#define _AIE_KERNELS_CFLOAT_KERNELS_KERNELS_H_

#include <adf.h>

void
complex_in_out(input_stream<cint16> *i1,
               output_stream<cint16> *o1);

#endif // ifndef _AIE_KERNELS_CFLOAT_KERNELS_KERNELS_H_

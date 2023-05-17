/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/


#include "ap_fixed.h"
#include <stdint.h>
#include "hls_math.h"

#define dataType double
#define MAXN 512
#define NCU 4

extern "C" void cholesky_kernel(int diagSize, dataType* matrixA);

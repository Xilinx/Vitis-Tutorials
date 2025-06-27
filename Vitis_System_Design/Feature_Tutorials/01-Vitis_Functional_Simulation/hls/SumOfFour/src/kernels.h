//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef _HLS_KERNELS_H_
#define _HLS_KERNELS_H_

#include "hls_stream.h"
#include <stdint.h>

void sum_of_four(hls::stream<int32_t> &in,
            hls::stream<int32_t> &out);

#endif // ifndef _HLS_KERNELS_H_

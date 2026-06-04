//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef _HLS_KERNELS_H_
#define _HLS_KERNELS_H_

#include "hls_stream.h"
#include <stdint.h>

void hls_array_of_stream(hls::stream<int32_t> in[4],
            hls::stream<int32_t> out[4]);

#endif // ifndef _HLS_KERNELS_H_

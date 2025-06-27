//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "kernels.h"

void sum_of_four(hls::stream<int32_t> &in,
            hls::stream<int32_t> &out)
{
    int32_t sum = 0;
    for (unsigned int i=0; i<4; i++) {
        sum += in.read();
    }
    out.write(sum);
}
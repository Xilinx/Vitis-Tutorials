//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "kernels.h"

void hls_array_of_stream(hls::stream<int32_t> in[4],
            hls::stream<int32_t> out[4])
{
    int32_t temp[4];
    for (unsigned int i=0; i<4; i++) {
        temp[i] = in[i].read()*(i+1);
        out[i].write(temp[i]);
    }
    
}
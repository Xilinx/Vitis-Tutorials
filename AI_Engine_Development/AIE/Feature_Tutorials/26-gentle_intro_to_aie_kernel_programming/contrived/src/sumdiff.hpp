/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#pragma once

#include "adf.h"

// vector addition or subtraction depending on input RTP
template<typename Tacc, typename Ti, typename To, unsigned nelems, unsigned burst_count>
void sumdiff(
    const int8 mode,                                                // runtime parameter: 0: add; otherwise subtract
    input_cascade<Tacc> *icstrm,                                    // input vector via accumulator cascade
    input_stream<Ti> *istrm,                                        // input vector via plio stream
    adf::output_buffer<To, adf::extents<burst_count * nelems>> &out // output sum or difference
);
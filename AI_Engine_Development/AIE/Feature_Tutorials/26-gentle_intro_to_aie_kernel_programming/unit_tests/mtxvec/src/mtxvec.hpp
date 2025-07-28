/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#pragma once

#include "adf.h"

// concatenates 4 sub-matrices into a larger matrix
// then multiplies the resulting matrix with an input vector
template<typename Ti, typename Tacc, unsigned mrows, unsigned mcols, unsigned burst_count>
void mtxvec(
    adf::input_buffer<Ti, adf::extents<burst_count * mrows * mcols>> &UL_in,  // input upper left matrix
    adf::input_buffer<Ti, adf::extents<burst_count * mrows * mcols>> &LL_in,  // input lower left matrix
    adf::input_buffer<Ti, adf::extents<burst_count * mrows * mcols>> &UR_in,  // input upper right matrix
    adf::input_buffer<Ti, adf::extents<burst_count * mrows * mcols>> &LR_in,  // input lower right matrix
    input_stream<Ti> *istrm,        // input vector
    output_cascade<Tacc> *ocstrm    // output accumulator cascade
);

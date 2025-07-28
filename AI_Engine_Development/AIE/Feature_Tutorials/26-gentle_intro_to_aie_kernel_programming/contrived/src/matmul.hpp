/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#pragma once

#include "adf.h"

// Amtx: Arows x Acols
// Bmtx: Acols x Bcols
// Pmtx: Arows x Bcols
// multiply 2 matrix *tiles*
template <typename Ta, typename Tb, typename Tp, unsigned Arows, unsigned Acols, unsigned Bcols, unsigned burst_count>
void matmul(
    adf::input_buffer< Ta, adf::extents<burst_count * Arows * Acols>> &Amtx,  // input "A" matrix
    adf::input_buffer< Tb, adf::extents<burst_count * Acols * Bcols>> &Bmtx,  // input "B" matrix
    adf::output_buffer<Tp, adf::extents<burst_count * Arows * Bcols>> &Pmtx   // output "P" product matrix
);

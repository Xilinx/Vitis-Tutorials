/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#pragma once

#include "adf.h"

// cascade stream source
template<typename Ti, typename Tacc, unsigned nelems, unsigned burst_count>
void cascsrc(
    input_stream<Ti> *istrm,        // input stream
    output_cascade<Tacc> *ocstrm    // output cascade stream
);
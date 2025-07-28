/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#pragma once

#include "adf.h"

// cascade stream sink
template<typename Tacc, typename To, unsigned nelems, unsigned burst_count>
void cascsink(
    input_cascade<Tacc> *icstrm,    // input cascade stream
    output_stream<To> *ostrm        // output stream
);

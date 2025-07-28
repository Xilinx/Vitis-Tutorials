/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#pragma once

#include "adf.h"

// calculate the squared magnitude of the input vector elements
// processes "burst_count" vectors during each invocation
template  <typename Ti, typename To, unsigned vlen, unsigned burst_count>
void sqmag(input_stream<Ti> *istrm, // input stream
           output_stream<To> *ostrm // output stream
);


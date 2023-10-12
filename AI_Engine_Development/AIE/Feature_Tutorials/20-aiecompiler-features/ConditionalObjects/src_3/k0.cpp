// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "adf.h"
template<int ID>
void f0(input_stream<int32> *in , output_stream<int32> * out)
{
    writeincr(out, (readincr(in) +ID));
}

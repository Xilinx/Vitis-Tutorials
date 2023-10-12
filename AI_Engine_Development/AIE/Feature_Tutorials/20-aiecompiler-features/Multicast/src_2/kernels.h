// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

#include "system_settings.h"

void passthrough_buffer(adf::input_buffer<int32, adf::extents<adf::inherited_extent>> &__restrict in, adf::output_buffer<int32, adf::extents<adf::inherited_extent>> &__restrict out);

template<int FrameLength>
void passthrough_stream(input_stream<int32> *__restrict in1, output_stream<int32> *__restrict out);

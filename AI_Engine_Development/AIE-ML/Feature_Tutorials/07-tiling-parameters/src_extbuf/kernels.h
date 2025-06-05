// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include <adf.h>
#include <aie_api/aie.hpp>

#include "system_settings.h"

template <int FrameLength, int VectorLength>
void passthrough_buffer(adf::input_buffer<int32, adf::extents<FrameLength>> &__restrict in, adf::output_buffer<int32, adf::extents<FrameLength>> &__restrict out);

template <int FrameLength, int VectorLength>
void passthrough_buffer2(adf::input_buffer<int32, adf::extents<FrameLength>> &__restrict in, adf::output_buffer<int32, adf::extents<FrameLength>> &__restrict out1, adf::output_buffer<int32, adf::extents<FrameLength>> &__restrict out2);

template <int FrameLength, int VectorLength>
void passthrough_buffer_block(adf::input_buffer<int32, adf::extents<FrameLength>> &__restrict in, adf::output_buffer<int32, adf::extents<FrameLength>> &__restrict out1);

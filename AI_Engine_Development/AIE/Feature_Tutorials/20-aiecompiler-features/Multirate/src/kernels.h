// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include <adf.h>
#include <aie_api/aie.hpp>

#include "system_settings.h"


void DownConv_5_7_iobuf(adf::input_buffer<int32, adf::extents<adf::inherited_extent>> &__restrict in, adf::output_buffer<int32, adf::extents<adf::inherited_extent>> &__restrict out);
void UpConv_7_5_iobuf(adf::input_buffer<int32, adf::extents<adf::inherited_extent>> &__restrict in, adf::output_buffer<int32, adf::extents<adf::inherited_extent>> &__restrict out);
void Subtract_iobuf(adf::input_buffer<int32, adf::extents<adf::inherited_extent>> &__restrict in1, adf::input_buffer<int32, adf::extents<adf::inherited_extent>> &__restrict in2, adf::output_buffer<int32, adf::extents<adf::inherited_extent>> &__restrict out);

template<int FrameLengthIn>
void DownConv_5_7_stream(input_stream<int32> *__restrict in, output_stream<int32> *__restrict out);
template<int FrameLengthIn>
void UpConv_7_5_stream(input_stream<int32> *__restrict in, output_stream<int32> *__restrict out);
template<int FrameLength>
void Subtract_stream(input_stream<int32> *__restrict in1, input_stream<int32> *__restrict in2, output_stream<int32> *__restrict out);

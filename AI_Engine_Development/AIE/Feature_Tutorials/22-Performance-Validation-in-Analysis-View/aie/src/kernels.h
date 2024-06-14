// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

#include "system_settings.h"

void gain(adf::input_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict in, 
            adf::output_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict out,cint16 Gain);

void passthrough_buffer(adf::input_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict in, 
            adf::output_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict out);


void passthrough_stream(input_stream<cint16> *__restrict in, output_stream<cint16> *__restrict out);

void passthrough_bufstr(adf::input_buffer<cint16,adf::extents<BUFFER_SIZE>> &__restrict in,
            output_stream<cint16> *__restrict out);

void passthrough_strbuf(input_stream<cint16> *__restrict in, 
            adf::output_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict out);

void filter_str(input_stream<cint16> *__restrict in, output_stream<cint16> *__restrict out,int32 FiltSel);

void filter_buf(adf::input_buffer<cint16, adf::extents<BUFFER_SIZE>,adf::margin<MARGIN_SIZE>> &__restrict in, 
	adf::output_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict out,int32 FiltSel);

template<int N>
void upsample_buffer(adf::input_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict in, adf::output_buffer<cint16, adf::extents<N*BUFFER_SIZE>> &__restrict out);

/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
#pragma once
#include <adf.h>

using namespace adf;
typedef input_buffer<cint16> in_type;
typedef output_buffer<cint16> out_type;


void k_1_buff_i_1_buff_o(in_type & in,out_type & out);

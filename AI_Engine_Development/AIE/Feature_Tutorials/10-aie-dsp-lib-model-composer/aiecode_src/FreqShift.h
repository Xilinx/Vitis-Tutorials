/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
 
 #include <adf.h>

template <int FRAME_LENGTH>
void FreqShift(adf::input_buffer<cint16>& __restrict in, 
               adf::output_buffer<cint16>& __restrict out);

template <int FRAME_LENGTH>
void FreqShift8(adf::input_buffer<cint16>& __restrict in1, 
                adf::input_buffer<cint16>& __restrict in2, 
                adf::input_buffer<cint16>& __restrict in3, 
                adf::input_buffer<cint16>& __restrict in4, 
                adf::input_buffer<cint16>& __restrict in5, 
                adf::input_buffer<cint16>& __restrict in6, 
                adf::input_buffer<cint16>& __restrict in7, 
                adf::input_buffer<cint16>& __restrict in8,
                adf::output_buffer<cint16>& __restrict out);
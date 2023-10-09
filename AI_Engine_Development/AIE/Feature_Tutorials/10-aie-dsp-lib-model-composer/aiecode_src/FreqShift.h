/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
 
 #include <adf.h>

template <int FRAME_LENGTH>
void FreqShift(input_window_cint16 * __restrict in, output_window_cint16 * __restrict out);

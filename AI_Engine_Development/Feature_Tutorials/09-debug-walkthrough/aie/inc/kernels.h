/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#ifndef __KERNELS_H__
#define __KERNELS_H__

//AIE API versions of kernels
void bf8x8_fst_api(input_window<cint16> * restrict c_input,
                   input_window<cint16> * restrict x_input,
                   output_stream_cacc48 * restrict cascade_out);

void bf8x8_mid_api(input_window<cint16> * restrict c_input,
                   input_window<cint16> * restrict x_input,
                   input_stream_cacc48 * cascade_in,
                   output_stream_cacc48 * cascade_out);

void bf8x8_lst_api(input_window<cint16> * restrict c_input,
                   input_window<cint16> * restrict x_input,
                   input_stream_cacc48 * cascade_in,
                   output_window<cint16> * restrict c_output);

#endif // __KERNELS_H__


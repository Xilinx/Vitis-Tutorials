//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

void datamover(input_window_cint16 * restrict cb_input, output_window_cint16 * restrict cb_output);
void datamover_scalar(input_window_cint16 * restrict cb_input, output_window_cint16 * restrict cb_output);
void datamover_vector_reg(input_window_cint16 * restrict cb_input, output_window_cint16 * restrict cb_output);
void datamover_mul_one(input_window_cint16 * restrict cb_input, output_window_cint16 * restrict cb_output);
void stream_datamover(input_stream_cint16 * restrict stream_in, output_stream_cint16 * restrict stream_out);
void datascale(input_window_cint16 * restrict cb_input, output_window_cint16 * restrict cb_output);


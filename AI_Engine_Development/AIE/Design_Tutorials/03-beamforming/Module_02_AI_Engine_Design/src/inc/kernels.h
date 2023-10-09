
/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#ifndef __KERNELS_H__
#define __KERNELS_H__


extern "C" { 
 
    // beamforming kernels on AIE
	void bf8x8_fst(input_window_cint16 * restrict c_input, input_window_cint16 * restrict x_input, output_stream_cacc48 * cascadeout);
		 
	void bf8x8_mid(input_window_cint16 * restrict c_input, input_window_cint16 * restrict x_input, input_stream_cacc48 * data_in, output_stream_cacc48 * cascadeout);
	 
	void bf8x8_lst(input_window_cint16 * restrict c_input, input_window_cint16 * restrict x_input, input_stream_cacc48 * data_in, output_window_cint16 * restrict data_out);
  
};

#endif // __KERNELS_H__



/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#ifndef __KERNELS_H__
#define __KERNELS_H__


extern "C" { 
 
    // beamforming kernels on AIE
	
   void bf8x8_fst(input_buffer<cint16> & __restrict c_input, input_buffer<cint16> & __restrict x_input, output_cascade<cacc48> * cascadeout);
   
   void bf8x8_mid(input_buffer<cint16> & __restrict c_input, input_buffer<cint16> & __restrict x_input, input_cascade<cacc48> * data_in, output_cascade<cacc48> * cascadeout);
	
   void bf8x8_lst(input_buffer<cint16> & __restrict c_input, input_buffer<cint16> & __restrict x_input, input_cascade<cacc48> * data_in, output_buffer<cint16> & __restrict data_out);
  
};

#endif // __KERNELS_H__


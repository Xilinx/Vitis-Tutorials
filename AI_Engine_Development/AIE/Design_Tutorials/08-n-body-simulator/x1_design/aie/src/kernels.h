/*
Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef FUNCTION_NBODY_H
#define FUNCTION_NBODY_H

#include <adf.h>
using namespace adf;
// AIE Kernels

void nbody(
	input_buffer<float> & __restrict w_input_i,  		// x, y, z, vx, vy, vz, m = NUM_I*7 samples input_buffer<int>  & __restrict i
	input_async_buffer<float> & __restrict w_input_j,
	output_buffer<float> & __restrict w_output_i  		// new x, y, z, vx, vy, vz, m = NUM_I*7 samples
);

void transmit_new_i(       
	input_buffer<float> & __restrict w_input_i,         	// x, y, z, vx, vy, vz, m
	output_buffer<float> & __restrict w_output_i 			// x, y, z, vx, vy, vz, m
);


#endif	//FUNCTION_NBODY_H

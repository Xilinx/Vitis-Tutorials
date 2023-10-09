/*Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#ifndef FUNCTION_NBODY_H
#define FUNCTION_NBODY_H

#include <adf.h>

// AIE Kernels

void nbody(
	input_window_float * w_input_i,  		// x, y, z, vx, vy, vz, m = NUM_I*7 samples
	input_window_float * w_input_j,
	output_window_float * w_output_i  		// new x, y, z, vx, vy, vz, m = NUM_I*7 samples
);

void transmit_new_i(       
	input_window_float * w_input_i,         	// x, y, z, vx, vy, vz, m
	output_window_float * w_output_i 			// x, y, z, vx, vy, vz, m
);


#endif	//FUNCTION_NBODY_H

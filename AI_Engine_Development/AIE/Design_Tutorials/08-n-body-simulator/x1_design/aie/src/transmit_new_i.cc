/*
Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
	
#include "include.h"
#include <vector>
using namespace aie;
using namespace adf;

void transmit_new_i(       
	input_buffer<float> & __restrict w_input_i,          // x, y, z, m, vx, vy, vz
	output_buffer<float> & __restrict w_output_i  		// x, y, z, m, vx, vy, vz
)  {	
	auto ptr = aie::begin_vector<8>(w_input_i);
	auto ptr_1= aie::begin_vector<8>(w_output_i);
	for (int n=0; n<7; n++) {
		for (int i=0; i<NUM_I_VECTORS; i++) {
			*ptr_1  = *ptr;
			ptr_1++;ptr++;
		}
		
	}
}

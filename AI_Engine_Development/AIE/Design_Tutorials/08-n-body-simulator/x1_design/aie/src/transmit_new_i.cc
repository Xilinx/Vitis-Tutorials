/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "include.h"

void transmit_new_i(       
	input_window_float * w_input_i,         // x, y, z, m, vx, vy, vz
	output_window_float * w_output_i 		// x, y, z, m, vx, vy, vz
)  {

	for (int n=0; n<7; n++) {
		for (int i=0; i<NUM_I_VECTORS; i++) {
			v8float newi  = window_readincr_v8(w_input_i);
			for (int k=0; k<8; k++) {
				float e = ::ext_elem(newi, k);	
				window_writeincr(w_output_i, e);
			}
		}	
	}
	
}




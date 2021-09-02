/* (c) Copyright 2021 Xilinx, Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 * 		http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
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

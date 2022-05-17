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




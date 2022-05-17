/* (c) Copyright 2020 Xilinx, Inc.
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

#ifndef __KERNELS_H__
#define __KERNELS_H__


extern "C" { 
 
    // beamforming kernels on AIE
	void bf8x8_fst(input_window_cint16 * restrict c_input, input_window_cint16 * restrict x_input, output_stream_cacc48 * cascadeout);
		 
	void bf8x8_mid(input_window_cint16 * restrict c_input, input_window_cint16 * restrict x_input, input_stream_cacc48 * data_in, output_stream_cacc48 * cascadeout);
	 
	void bf8x8_lst(input_window_cint16 * restrict c_input, input_window_cint16 * restrict x_input, input_stream_cacc48 * data_in, output_window_cint16 * restrict data_out);
  
};

#endif // __KERNELS_H__


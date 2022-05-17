/*
 * (c) Copyright 2021-2022 Xilinx, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
 
/*
 * Frequency Shift
 *
 */

#include <adf.h>

// Definition of the 2 constants 1 and 1/sqrt(2)
#define One 16384	
#define RS2 11585
#define ACC_SHIFT 14

// Sine-Cosine values for a frequency shifting of fs/8
alignas (32) int16_t  cos_sin[16] = 
{ One, 0, RS2, RS2, 0, One, -RS2, RS2, -One, 0, -RS2, -RS2, 0, -One, RS2, -RS2 };


// FRAME_LENGTH should be a power of 2 greater than or equal to 32
template <int FRAME_LENGTH>
void FreqShift(input_window_cint16 * __restrict in, output_window_cint16 * __restrict out)
{
	v8cint16 coeffs = *(v8cint16*) cos_sin;

	v8cint16 data = undef_v8cint16();
	v8cacc48 acc;

	for(unsigned i=0; i<FRAME_LENGTH/8; i++)
		chess_prepare_for_pipelining
		chess_loop_range(4,)
	{
		data = window_readincr_v8(in);
		acc = mul(data,coeffs);
		window_writeincr(out,srs(acc,ACC_SHIFT));
	}

}

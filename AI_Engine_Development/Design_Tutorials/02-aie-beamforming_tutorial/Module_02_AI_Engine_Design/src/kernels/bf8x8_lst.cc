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
 
#include <adf.h>
#include "include.h"


void bf8x8_init()
{
	set_rnd(rnd_sym_inf);
	set_sat();
};


void bf8x8_lst(input_window_cint16  * restrict c_input, input_window_cint16  * restrict x_input,
         input_stream_cacc48 * data_in, output_window_cint16 * restrict c_output)
{

	// buffer for coefficients
	v16cint16 bufa = undef_v16cint16();
	v16cint16 bufb = undef_v16cint16();
	
	// Loop Through the PRBs
	for (unsigned prbcnt=0; prbcnt<NUM_PRBS; ++prbcnt)
	chess_flatten_loop
	{
		
		// initialize the coefficient memory
		bufa = upd_w(bufa, 0, window_readincr_v8(c_input));
		bufa = upd_w(bufa, 1, window_readincr_v8(c_input));
		bufb = upd_w(bufb, 0, window_readincr_v8(c_input));
		bufb = upd_w(bufb, 1, window_readincr_v8(c_input));
		
		//---------------------------------------
		// Every Loop Deals with Two Subcarriers
		//---------------------------------------
		for (unsigned i=0; i<6; ++i)
		chess_prepare_for_pipelining
		{
			v8cint16 dat;
			v8cacc48 acc  = undef_v8cacc48(); 

			// read in 8 input data
			dat = window_readincr_v8(x_input);
			
			acc = upd_lo(acc, mac4(READSCD, bufa, 0, 0x3210, 8, dat, 0,  0x0000, 1)); 
			acc = upd_hi(acc, mac4(READSCD, bufa, 4, 0x3210, 8, dat, 0,  0x0000, 1)); 
					
			acc = upd_lo(acc, mac4(ext_lo(acc),    bufb, 0, 0x3210, 8, dat, 2,  0x0000, 1)); bufa = upd_w(bufa, 0, window_readincr_v8(c_input));
			acc = upd_hi(acc, mac4(ext_hi(acc),    bufb, 4, 0x3210, 8, dat, 2,  0x0000, 1)); bufa = upd_w(bufa, 1, window_readincr_v8(c_input));
								
			acc = upd_lo(acc, mac4(ext_lo(acc),    bufa, 0, 0x3210, 8, dat, 4,  0x0000, 1)); bufb = upd_w(bufb, 0, window_readincr_v8(c_input));
			acc = upd_hi(acc, mac4(ext_hi(acc),    bufa, 4, 0x3210, 8, dat, 4,  0x0000, 1)); bufb = upd_w(bufb, 1, window_read_v8(c_input)); window_decr_v8(c_input, 4);
						
			acc = upd_lo(acc, mac4(ext_lo(acc),    bufb, 0, 0x3210, 8, dat, 6,  0x0000, 1)); 
			acc = upd_hi(acc, mac4(ext_hi(acc),    bufb, 4, 0x3210, 8, dat, 6,  0x0000, 1)); window_writeincr(c_output, srs(acc, PDSCH_SHFT));

			// read in 8 input data
			dat = window_readincr_v8(x_input);
		
			acc = upd_lo(acc,mac4(READSCD, bufb, 0, 0x3210, 8, dat, 6,  0x0000, 1)); 
			acc = upd_hi(acc,mac4(READSCD, bufb, 4, 0x3210, 8, dat, 6,  0x0000, 1)); 
			
			acc = upd_lo(acc,mac4(ext_lo(acc),   bufa,  0, 0x3210, 8, dat, 4,  0x0000, 1)); bufb = upd_w(bufb, 1, window_readdecr_v8(c_input));
			acc = upd_hi(acc,mac4(ext_hi(acc),   bufa,  4, 0x3210, 8, dat, 4,  0x0000, 1)); bufb = upd_w(bufb, 0, window_readdecr_v8(c_input));
								
			acc = upd_lo(acc,mac4(ext_lo(acc),   bufb,  0, 0x3210, 8, dat, 2,  0x0000, 1)); bufa = upd_w(bufa, 1, window_readdecr_v8(c_input));
			acc = upd_hi(acc,mac4(ext_hi(acc),   bufb,  4, 0x3210, 8, dat, 2,  0x0000, 1)); bufa = upd_w(bufa, 0, window_read_v8(c_input)); window_incr_v8(c_input, 4);
						
			acc = upd_lo(acc,mac4(ext_lo(acc),   bufa,  0, 0x3210, 8, dat, 0,  0x0000, 1)); 
			acc = upd_hi(acc,mac4(ext_hi(acc),   bufa,  4, 0x3210, 8, dat, 0,  0x0000, 1)); window_writeincr(c_output, srs(acc, PDSCH_SHFT));
			
		}// end of loop for PRB
			
		// move the coefficient pointer to next PRB.
		window_incr_v8(c_input, 4);
		
	}// end of loop through PRBs
  
}// end of function

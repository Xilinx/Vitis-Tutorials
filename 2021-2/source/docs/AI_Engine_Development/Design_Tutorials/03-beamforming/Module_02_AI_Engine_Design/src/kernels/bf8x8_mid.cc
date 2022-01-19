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

void bf8x8_mid(input_window_cint16  * restrict c_input, input_window_cint16  * restrict x_input,
         input_stream_cacc48 * cascadein, output_stream_cacc48 * cascadeout)
{

	// Buffer for coefficients
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
			v4cacc48 acca; 
			v4cacc48 accb;
			
			// read in 8 input data
			dat = window_readincr_v8(x_input);
			
			acca = mac4(READSCD, bufa, 0, 0x3210, 8, dat, 0,  0x0000, 1); 
			accb = mac4(READSCD, bufa, 4, 0x3210, 8, dat, 0,  0x0000, 1); 
					
			acca = mac4(acca,    bufb, 0, 0x3210, 8, dat, 2,  0x0000, 1); bufa = upd_w(bufa, 0, window_readincr_v8(c_input));
			accb = mac4(accb,    bufb, 4, 0x3210, 8, dat, 2,  0x0000, 1); bufa = upd_w(bufa, 1, window_readincr_v8(c_input));
								
			acca = mac4(acca,    bufa, 0, 0x3210, 8, dat, 4,  0x0000, 1); bufb = upd_w(bufb, 0, window_readincr_v8(c_input));
			accb = mac4(accb,    bufa, 4, 0x3210, 8, dat, 4,  0x0000, 1); bufb = upd_w(bufb, 1, window_read_v8(c_input)); window_decr_v8(c_input, 4);
						
			acca = mac4(acca,    bufb, 0, 0x3210, 8, dat, 6,  0x0000, 1); put_mcd(acca);
			accb = mac4(accb,    bufb, 4, 0x3210, 8, dat, 6,  0x0000, 1); put_mcd(accb);
			
			// read in 8 input data
			dat = window_readincr_v8(x_input);
		
			acca = mac4(READSCD, bufb,  0, 0x3210, 8, dat, 6,  0x0000, 1); 
			accb = mac4(READSCD, bufb,  4, 0x3210, 8, dat, 6,  0x0000, 1); 
								 
			acca = mac4(acca,    bufa,  0, 0x3210, 8, dat, 4,  0x0000, 1); bufb = upd_w(bufb, 1, window_readdecr_v8(c_input));
			accb = mac4(accb,    bufa,  4, 0x3210, 8, dat, 4,  0x0000, 1); bufb = upd_w(bufb, 0, window_readdecr_v8(c_input));
								 
			acca = mac4(acca,    bufb,  0, 0x3210, 8, dat, 2,  0x0000, 1); bufa = upd_w(bufa, 1, window_readdecr_v8(c_input));
			accb = mac4(accb,    bufb,  4, 0x3210, 8, dat, 2,  0x0000, 1); bufa = upd_w(bufa, 0, window_read_v8(c_input)); window_incr_v8(c_input, 4);
								 
			acca = mac4(acca,    bufa,  0, 0x3210, 8, dat, 0,  0x0000, 1); put_mcd(acca);
			accb = mac4(accb,    bufa,  4, 0x3210, 8, dat, 0,  0x0000, 1); put_mcd(accb);
			
		}// end of loop for PRB

		// move the coefficient pointer to next PRB.
		window_incr_v8(c_input, 4);
		
	}// end of loop for PRBs
  
}// end of function


 
  

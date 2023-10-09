
/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include <adf.h>
#include "include.h"

void bf8x8_fst(input_window_cint16 * restrict c_input, input_window_cint16 * restrict x_input,
         output_stream_cacc48 * cascadeout)
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
			
			acca = mul4(         bufa, 0, 0x3210, 8, dat, 0,  0x0000, 1); 
			accb = mul4(         bufa, 4, 0x3210, 8, dat, 0,  0x0000, 1); 
					
			acca = mac4(acca,    bufb, 0, 0x3210, 8, dat, 2,  0x0000, 1); bufa = upd_w(bufa, 0, window_readincr_v8(c_input));
			accb = mac4(accb,    bufb, 4, 0x3210, 8, dat, 2,  0x0000, 1); bufa = upd_w(bufa, 1, window_readincr_v8(c_input));
								
			acca = mac4(acca,    bufa, 0, 0x3210, 8, dat, 4,  0x0000, 1); bufb = upd_w(bufb, 0, window_readincr_v8(c_input));
			accb = mac4(accb,    bufa, 4, 0x3210, 8, dat, 4,  0x0000, 1); bufb = upd_w(bufb, 1, window_read_v8(c_input)); window_decr_v8(c_input, 4);
						
			acca = mac4(acca,    bufb, 0, 0x3210, 8, dat, 6,  0x0000, 1); put_mcd(acca);
			accb = mac4(accb,    bufb, 4, 0x3210, 8, dat, 6,  0x0000, 1); put_mcd(accb);
			
			// read in 8 input data
			dat = window_readincr_v8(x_input);
		
			acca = mul4(        bufb,  0, 0x3210, 8, dat, 6,  0x0000, 1); 
			accb = mul4(        bufb,  4, 0x3210, 8, dat, 6,  0x0000, 1); 
			
			acca = mac4(acca,   bufa,  0, 0x3210, 8, dat, 4,  0x0000, 1); bufb = upd_w(bufb, 1, window_readdecr_v8(c_input));
			accb = mac4(accb,   bufa,  4, 0x3210, 8, dat, 4,  0x0000, 1); bufb = upd_w(bufb, 0, window_readdecr_v8(c_input));
								
			acca = mac4(acca,   bufb,  0, 0x3210, 8, dat, 2,  0x0000, 1); bufa = upd_w(bufa, 1, window_readdecr_v8(c_input));
			accb = mac4(accb,   bufb,  4, 0x3210, 8, dat, 2,  0x0000, 1); bufa = upd_w(bufa, 0, window_read_v8(c_input)); window_incr_v8(c_input, 4);
						
			acca = mac4(acca,   bufa,  0, 0x3210, 8, dat, 0,  0x0000, 1); put_mcd(acca);
			accb = mac4(accb,   bufa,  4, 0x3210, 8, dat, 0,  0x0000, 1); put_mcd(accb);
			
		}// end of loop for PRB
			
		// move the coefficient pointer to next PRB.
		window_incr_v8(c_input, 4);
		
	}// end of loop for PRBs
  
}// end of function


 
  

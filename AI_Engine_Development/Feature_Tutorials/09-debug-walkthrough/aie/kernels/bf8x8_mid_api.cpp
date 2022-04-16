/*
 * Copyright 2021 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef INLINE

#include "include.h"
#include "aie_api/aie.hpp"
#include "aie_api/utils.hpp"
#include "aie_api/aie_adf.hpp"

// O0=x0 *C0+x8 *C1 + x16*C2+x24*C3 + x32*C4+x40*C5 + x48*C6+x56*C7;
// O1=x1 *C0+x9 *C1 + x17*C2+x25*C3 + x33*C4+x41*C5 + x49*C6+x57*C7;
// O2=x2 *C0+x10*C1 + x18*C2+x26*C3 + ...;
// O3=x3 *C0+x11*C1 + x19*C2+x27*C3 + ...;
// O4=x4 *C0+x12*C1 + x20*C2+x28*C3 + ...;
// O5=x5 *C0+x13*C1 + x21*C2+x29*C3 + ...;
// O6=x6 *C0+x14*C1 + x22*C2+x30*C3 + ...;
// O7=x7 *C0+x15*C1 + x23*C2+x31*C3 + x39*C4+x47*C5 + x55*C6+x63*C7;

void bf8x8_mid_api(input_window<cint16> * restrict c_input, input_window<cint16> * restrict x_input, input_stream_cacc48 * cascade_in, output_stream_cacc48 * cascade_out)
{
    // Loop Through the PRBs
    for (unsigned prbcnt=0; prbcnt<NUM_PRBS; ++prbcnt)
        chess_prepare_for_pipelining
    {
        //---------------------------------------
        // Every Loop Deals with Two Subcarriers
        //---------------------------------------
        for (unsigned i=0; i<12; ++i)  
        chess_prepare_for_pipelining
        chess_unroll_loop(2)
        {
            // initialize the coefficient memory
            aie::vector<cint16, 8> bufa0 = window_readincr_v8(c_input); 
            aie::vector<cint16, 8> bufa1 = window_readincr_v8(c_input); 
            aie::vector<cint16, 8> bufb0;
            aie::vector<cint16, 8> bufb1;

            // read in 8 input data
            aie::vector<cint16, 8 > dat0 = window_readincr_v8(x_input);
            auto acc1 = readincr_v<8>(cascade_in);
            acc1      = aie::mac(acc1, dat0[0], bufa0); bufb0 = window_readincr_v8(c_input); 
            acc1      = aie::mac(acc1, dat0[1], bufa1); bufb1 = window_readincr_v8(c_input); 

            acc1      = aie::mac(acc1, dat0[2], bufb0); bufa0 = window_readincr_v8(c_input); 
            acc1      = aie::mac(acc1, dat0[3], bufb1); bufa1 = window_readincr_v8(c_input); 
 
            acc1      = aie::mac(acc1, dat0[4], bufa0); bufb0 = window_readincr_v8(c_input);
            acc1      = aie::mac(acc1, dat0[5], bufa1); bufb1 =     window_read_v8(c_input); 

            acc1      = aie::mac(acc1, dat0[6], bufb0); 
            acc1      = aie::mac(acc1, dat0[7], bufb1); 
            writeincr(cascade_out, acc1);

            window_decr(c_input,7*8); //Move back to refill start with same contents as outer loop
        }// end of loop for PRB
      
        // move the coefficient pointer to next PRB.
        window_incr(c_input, 8*8);
    }
}

#endif //INLINE

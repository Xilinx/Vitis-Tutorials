/**********
© Copyright 2020 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/

#include <adf.h>
#include "../../include.h"

/*
// 27-tap FIR and 2x up-sampling
Interpolation rate:     2x
Coefficients:           c0  0 c2  0 c4  0 c6  0 c8  0 c10 0 c12 c13 c12 0 c10 0 c8  0 c6  0 c4  0 c2  0  c0  
Data is interpolated:   d0  0 d1  0 d2  0 d3  0 d4  0 d5  0 d6  0   d7  0 d8  0 d9  0 d10 0 d11 0 d12 0  d13 ...
Outputs:                o0 = c0*(d0+d13) + c2*(d1+d12) + c4*(d2+d11) + c6*(d3+d10) + c8*(d4+d9) + c10*(d5+d8)
                             + c12*(d6+d7)
                        o1 = c13*d7
                        o2 = c0*(d1+d14) + c2*(d2+d13) + c4*(d3+d12) + c6*(d4+d11) + c8*(d5+d10) + c10*(d6+d9)
                             + c12*(d7+d8)
                        o3 = c13*d8
                        ...
offset: 3

*/

//static int16_t chess_storage(%chess_alignof(v16int16)) coeffs_27_i [INTERPOLATOR27_COEFFICIENTS] = {33, -158, 0, 0, 491, -1214, 2674, 0, 0, -5942, 20503, 32767, 0, 0, 0, 0};

static int16_t chess_storage(%chess_alignof(v16int16)) coeffs_27_i [INTERPOLATOR27_COEFFICIENTS] = {33, -158,491, -1214, 2674, -5942, 20503, 0, 32767};

void fir_27t_sym_hb_2i
(       
	input_window_cint16 * cb_input,
	output_window_cint16 * cb_output)
{
   const int shift = 0 ;	
   const unsigned output_samples = INTERPOLATOR27_OUTPUT_SAMPLES ;
  
  v32cint16 sbuff = undef_v32cint16();
  
  const unsigned LSIZE = (output_samples / 8 /2 );
  v16int16 coe = *(v16int16*)coeffs_27_i;

  //sbuff = upd_w(sbuff, 0, cb_readincr_v8(cb_input)); // 0:7|X|X|X
  sbuff = upd_w(sbuff, 0, window_readincr_v8(cb_input));

  v8cacc48 acc0 = undef_v8cacc48();
  v8cacc48 acc1 = undef_v8cacc48();

  const int sft = shift+15;

  for ( unsigned l=0; l<LSIZE; ++l )
    chess_prepare_for_pipelining
    chess_loop_range(8,)
    {

    //sbuff = upd_w(sbuff, 1, cb_readincr_v8(cb_input)); // 0:7|8:15|X|X
    sbuff = upd_w(sbuff, 1, window_readincr_v8(cb_input));

    acc0 = upd_hi(acc0, mul4(    sbuff, 10, 0x3210, 1, coe, 8,0x0000,1) ); //d10..d13

    //sbuff = upd_w(sbuff, 2, cb_readincr_v8(cb_input)); // 0:7|8:15|16:23|X
    sbuff = upd_w(sbuff, 2, window_readincr_v8(cb_input));

    acc0 = upd_lo(acc0,  mul4_sym(    sbuff, 7,0x3210,1, 12, coe, 4,0x0000,1) ); //d7..d15
    acc0 = upd_lo(acc0,  mac4_sym(ext_lo(acc0), sbuff, 3,0x3210,1, 16, coe, 0,0x0000,1) ); //d3..d19 //sym could be 12
    //cb_decr_v8(cb_input,2);
    window_decr_v8(cb_input,2);

    acc1 = upd_hi(acc1,  mul4(    sbuff, 14, 0x3210, 1, coe, 8,0x0000,1) ); //d14..d17

    acc1 = upd_lo(acc1, mul4_sym( sbuff, 7,0x3210,1, 20, coe, 0,0x0000,1) ); //d7..d23
    
    //sbuff = upd_w(sbuff, 0, cb_readincr_v8(cb_input)); // 8:15|8:15|16:23|X for next iteration
    sbuff = upd_w(sbuff, 0, window_readincr_v8(cb_input));

    acc1 = upd_lo(acc1, mac4_sym(ext_lo(acc1),    sbuff, 11,0x3210,1, 16, coe, 4,0x0000,1) ); //d11..d19
    
    //cb_writeincr(cb_output, srsI(acc0, sft));     
    //cb_writeincr(cb_output, srsI(acc1, sft)); 

    window_writeincr(cb_output, srsI(acc0, sft)); // printf(srsI(acc0,sft)) -- Enable profiling    
    window_writeincr(cb_output, srsI(acc1, sft)); // printf(srsI(acc1,sft))
    }
  //cb_incr_v8(cb_input,1); 
  window_incr_v8(cb_input,1);
}


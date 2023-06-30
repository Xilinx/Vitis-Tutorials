/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <adf.h>
#include "include.h"
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"
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

alignas(aie::vector_decl_align) static int16_t coeffs_27_i [INTERPOLATOR27_COEFFICIENTS] = {33, -158,491, -1214, 2674, -5942, 20503, 0, 32767};

void fir_27t_sym_hb_2i
(
        input_window_cint16 *__restrict cb_input,
        output_window_cint16 *__restrict cb_output)
{
     const int shift = 0 ;
     const unsigned output_samples = INTERPOLATOR27_OUTPUT_SAMPLES ;
     aie::vector<cint16, 32> sbuff;
     const unsigned LSIZE = (output_samples / 8 /2 );

     const aie::vector<int16, 16> coe = aie::load_v<16>(coeffs_27_i);

     sbuff.insert(0, window_readincr_v<8>(cb_input)); // 0:7|X|X|X

     constexpr unsigned Lanes         = 4;
     constexpr unsigned CoeffStep     = 1;
     constexpr unsigned DataStep      = 1;
     constexpr unsigned Hi_Points     = 4;

     constexpr unsigned Lo_Points     = 14;

     using mul_ops_hi = aie::sliding_mul_xy_ops <Lanes, Hi_Points, CoeffStep, DataStep, int16, cint16, cacc48>;
     using mul_ops_lo = aie::sliding_mul_sym_ops<Lanes, Lo_Points, CoeffStep, DataStep, DataStep, int16, cint16, cacc48>;

     constexpr unsigned Lo_Points_A   = 8;
     constexpr unsigned Lo_Points_B   = 6;

     using mul_ops_lo2_a = aie::sliding_mul_sym_ops<Lanes, Lo_Points_A, CoeffStep, DataStep, DataStep, int16, cint16, cacc48>;
     using mul_ops_lo2_b = aie::sliding_mul_sym_ops<Lanes, Lo_Points_B, CoeffStep, DataStep, DataStep, int16, cint16, cacc48>;

     const int sft = shift+15;

     for (unsigned int l = 0; l < LSIZE; ++l) chess_prepare_for_pipelining
                                              chess_loop_range(6, )
     {
         aie::accum<cacc48, 4> acc1a, acc1b;
         aie::accum<cacc48, 4> acc2a, acc2b;

         sbuff.insert(1, window_readincr_v<8>(cb_input)); // 0:7|8:15|X|X

         acc1a = mul_ops_hi::mul(    coe, 8, sbuff, 10); //d10..d13
         // printf("%u",acc1a);
         // printf("%u",sbuff);
         sbuff.insert(2, window_readincr_v<8>(cb_input)); // 0:7|8:15|16:23|X

         acc1b = mul_ops_lo::mul_sym(coe, 0, sbuff,  3); //d7..d15

         window_decr(cb_input, 16);

         acc2a = mul_ops_hi::mul(              coe, 8, sbuff, 14);     //d14..d17

         acc2b = mul_ops_lo2_a::mul_sym(       coe, 0, sbuff,  7, 20); //d7..d23

         sbuff.insert(0, window_readincr_v<8>(cb_input)); // 8:15|8:15|16:23|X for next iteration

         acc2b = mul_ops_lo2_b::mac_sym(acc2b, coe, 4, sbuff, 11);    //d11..d19

         window_writeincr(cb_output, aie::concat(acc1b, acc1a).to_vector_zip<cint16>(sft));
         window_writeincr(cb_output, aie::concat(acc2b, acc2a).to_vector_zip<cint16>(sft));
     }

     window_incr(cb_input, 8);
 }

/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include <adf.h>
#include "../../include.h"

/**
 * -------------------------------------------------------------------------------
 * 27-tap half-band symmetric FIR with 2x decimation
 * -------------------------------------------------------------------------------
 *
 * Coefficients:           c0  -  2  -  4  -  ...  -  12 13 12  -  ...  -  4  -  2  -  0
 * Data:                   d0  1  2  3  4  5  ... 11  12 13 14 15  ... 21 22 23 24 25 26 27 28 ...
 *
 * Intermediate:           o0  = c00*( 0+26) + c02*( 2+24) + c04*( 4+22) + ... + c12*(12+14) + c13*(13)
                       o0' = c00*( 1+27) + c02*( 3+25) + c04*( 5+23) + ... + c12*(13+15) + c13*(14)  // dropped
                       o1  = c00*( 2+28) + c02*( 4+26) + c04*( 6+24) + ... + c12*(14+16) + c13*(15)
                       o1' = c00*( 3+29) + c02*( 5+27) + c04*( 7+25) + ... + c12*(15+17) + c13*(16)  // dropped
                       o2  = c00*( 4+30) + c02*( 6+28) + c04*( 8+26) + ... + c12*(16+18) + c13*(17)
                       o2' = c00*( 5+31) + c02*( 7+29) + c04*( 9+27) + ... + c12*(17+19) + c13*(18)  // dropped
                       o3  = c00*( 6+32) + c02*( 8+30) + c04*(10+28) + ... + c12*(18+20) + c13*(19)
                       o3' = c00*( 7+33) + c02*( 9+31) + c04*(11+29) + ... + c12*(19+21) + c13*(20)  // dropped
 *
 * Decimation rate:        2x
 * Outputs:                o0  = c00*( 0+26) + c02*( 2+24) + c04*( 4+22) + ... + c12*(12+14) + c13*(13)
                       o1  = c00*( 2+28) + c02*( 4+26) + c04*( 6+24) + ... + c12*(14+16) + c13*(15)
                       o2  = c00*( 4+30) + c02*( 6+28) + c04*( 8+26) + ... + c12*(16+18) + c13*(17)
                       o3  = c00*( 6+32) + c02*( 8+30) + c04*(10+28) + ... + c12*(18+20) + c13*(19)
                       //    ^---------------- 14x pre-added multiplication columns -----------^   ^-mtap-^
                       //    ==>  1x sym_mul, 2x sym_mac, 1x symc_mac
                       ...
 *
                       1st iteration
                       -----------------
                       o0  = c0*( 0+26) +  c2*( 2+24) +  0          + 0
                       o1  = c0*( 2+28) +  c2*( 4+26) +  0          + 0
                       o2  = c0*( 4+30) +  c2*( 6+28) +  0          + 0
                       o3  = c0*( 6+32) +  c2*( 8+30) +  0          + 0
 *
                       o0  = c4*( 4+22) +  c6*( 6+20) +  c8*( 8+18) + 0
                       o1  = c4*( 6+24) +  c6*( 8+22) +  c8*(10+20) + 0
                       o2  = c4*( 8+26) +  c6*(10+24) +  c8*(12+22) + 0
                       o3  = c4*(10+28) +  c6*(12+26) +  c8*(14+24) + 0
 *
                       o0  =  0         + c10*(10+16) + c12*(12+14) + c13*(13)
                       o1  =  0         + c10*(12+18) + c12*(14+16) + c13*(15)
                       o2  =  0         + c10*(14+20) + c12*(16+18) + c13*(17)
                       o3  =  0         + c10*(16+22) + c12*(18+20) + c13*(19)
 *
                       <o00..o03> = F(<c0..cB>, <00..08>, <24..32>)
                       <o00..o03> = F(<c0..cB>, <04..14>, <18..28>)
                       <o00..o03> = F(<c0..cB>, <10..18>, <13..22>)
 *
                       <o04..o07> = F(<c0..cB>, <08..16>, <32..40>)
                       <o04..o07> = F(<c0..cB>, <12..22>, <26..36>)
                       <o04..o07> = F(<c0..cB>, <18..26>, <21..30>)
 *
 *
 * This design implements circular buffers for inputs and outputs.
 * Input buffer offset is 6 (to support back-to-back filter chains). Round coefficient
 * to nearest byte boundary; 32-27+1=6.
 *
 * -------------------------
 * Design Configurations
 * -------------------------
 * FIR_27TAP_SYMM_HB_DEC2                - enable this design
 INBUF_OFFSET_0                      - enable version with input buffer offset amount is 0 (default offset is 6)
 * TESTING                                                              - enable testbench setup (shared function name: fir_reference, fir_filter)
 * REFERENCE                                                            - enable reference code
 *
**********/

/*
  static int16_t coeffs_27[DECIMATOR27_COEFFICIENTS] =
  {     -1000,    -2000,    0,     0,    3000,    -4000,    5000,     0,    0,   -6000,   7000,   4064,  0 , 0 ,0 ,0
  };
*/

int16_t coeffs_27_d[DECIMATOR27_COEFFICIENTS]  = {33, -158, 0, 0, 491, -1214, 2674, 0, 0, -5942, 20503, 32767, 0, 0, 0, 0};

// Default implementation (shift=6)
/**
 * Modified marray arguments to fit the offset inputs.
 *
 * o0  = 0*( 0+38) + 0 + 0 + c0*( 6+32)
 * o1  = 0         + 0 + 0 + c0*( 8+34)
 * o2  = 0         + 0 + 0 + c0*(10+36)
 * o3  = 0         + 0 + 0 + c0*(12+38)
 *
 * o0  = c2*( 8+30) + c4*(10+28) + c6*(12+26) + c8*(14+24)
 * o1  = c2*(10+32) + c4*(12+30) + c6*(14+28) + c8*(16+26)
 * o2  = c2*(12+34) + c4*(14+32) + c6*(16+30) + c8*(18+28)
 * o3  = c2*(14+36) + c4*(16+34) + c6*(18+32) + c8*(20+30)
 *
 * o0  = c10*(16+22) + c12*(18+20) + 0 + c13*(19)
 * o1  = c10*(18+24) + c12*(20+22) + 0 + c13*(21)
 * o2  = c10*(20+26) + c12*(22+24) + 0 + c13*(23)
 * o3  = c10*(22+28) + c12*(24+26) + 0 + c13*(25)
 *
 * <o0..3> = F(<c0..cB>, <d06..12>, <d32..38>)
 * <o0..3> = F(<c0..cB>, <d08..20>, <d24..36>)
 * <o0..3> = F(<c0..cB>, <d16..25>, <d20..28>)
 *
 * <o4..7> = F(<c0..cB>, <d14..20>, <d40..46>)
 * <o4..7> = F(<c0..cB>, <d16..28>, <d32..44>)
 * <o4..7> = F(<c0..cB>, <d24..33>, <d28..working>)
**********/

//36 version with circular_buffer
void fir_27taps_symm_hb_dec2
(
    input_window_cint16 * inputw,
    output_window_cint16 * outputw
) {
    const int shift = 1 ;
    const unsigned output_samples =  DECIMATOR27_OUTPUT_SAMPLES ;
    static int loopct = 0;
    const v16int16 coe =  *((const v16int16 *) coeffs_27_d) ;

    //create a copy of the input window to handle symmetry
    input_window_cint16 temp_w;
    input_window_cint16 * restrict symw = &temp_w;
    *symw = *inputw;

    v8cint16 vdata;

    window_incr_v8(symw,3);

    v16cint16 lbuff = undef_v16cint16();
    v16cint16 rbuff = undef_v16cint16();

    window_readincr(inputw, vdata);
    lbuff = upd_w0(lbuff, vdata);   //|0:7|X|---|X|X| ->8

    window_readincr(symw, vdata);
    rbuff = upd_w0(rbuff, vdata);   //|0:7|X|---|24:31|X| ->32

    const unsigned lc = (output_samples / 4/ 2);
    for ( unsigned l=0; l<lc; ++l )
        chess_loop_range(4,)  // for 128 samples, we only require 4 iterations
        chess_prepare_for_pipelining
    {
        v4cacc48 acc0;

        window_readincr(inputw, vdata);
        lbuff = upd_w1(lbuff, vdata); //|0:7|8:15|---|24:31|X| l->16
        // lbuff = upd_w1(lbuff, window_readincr_v8(inputw));
        window_read(symw, vdata);
        rbuff = upd_w1(rbuff, vdata);   window_decr_v8(symw,2); //|0:7|8:15|---|24:31|32:39| r->16

        acc0 = mul4_sym(        lbuff, 6,0x6420,2, rbuff,8,     coe,0,0x0000,1);   //d6..14 d30-38 c0..1

        window_readincr(inputw, vdata);
        lbuff = upd_w0(lbuff, vdata); //|16:23|8:15|---|24:31|32:39| l->24

        acc0 = mac4_sym(   acc0,lbuff,10,0x6420,2, rbuff,4,     coe,4,0x0000,1);

        window_readincr(inputw, vdata);
        lbuff = upd_w1(lbuff, vdata); //|16:23|24:31|---|24:31|32:39| l->32
        window_read(symw, vdata);
        rbuff = upd_w1(rbuff, vdata); window_incr_v8(symw,2); //|16:23|24:31|---|24:31|16:23| r->32

        acc0 = mac4_sym_ct(acc0,lbuff,14,0x6420,2, rbuff,0, 5 , coe,8,0x0000,1);
        window_writeincr(outputw,fsrs(acc0,shift));

        window_read(inputw, vdata);
        lbuff = upd_w0(lbuff, vdata); window_decr_v8(inputw,2);
        window_readincr(symw, vdata);
        rbuff = upd_w1(rbuff, vdata);

        acc0 = mul4_sym_ct(     lbuff, 6,0x6420,2, rbuff,8, 5, coe,8,0x0000,1);

        // window_readdecr(inputw, vdata);
        window_read(inputw, vdata);
        lbuff = upd_w0(lbuff, vdata); window_decr_v8(inputw, 1);
        window_readincr(symw, vdata);
        rbuff = upd_w0(rbuff, vdata);

        acc0 = mac4_sym(   acc0,lbuff,18,0x6420,2, rbuff,12,    coe,4,0x0000,1);

        window_read(inputw, vdata);
        lbuff = upd_w1(lbuff, vdata); window_incr_v8(inputw,2);

        acc0 = mac4_sym(   acc0,lbuff,14,0x6420,2, rbuff,16,    coe,0,0x0000,1);
        window_writeincr(outputw,fsrs(acc0,shift));
    }
    window_incr_v8(inputw, 3);
}

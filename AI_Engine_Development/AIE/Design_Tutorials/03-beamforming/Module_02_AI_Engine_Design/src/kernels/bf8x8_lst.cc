/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef INLINE

#include "include.h"
#include "aie_api/aie.hpp"
#include "aie_api/utils.hpp"
#include "aie_api/aie_adf.hpp"


void bf8x8_init()
{
    set_rnd(rnd_sym_inf);
    set_sat();
};

void bf8x8_lst(input_buffer<cint16> & __restrict c_input, input_buffer<cint16> & __restrict x_input, input_cascade<cacc48> * cascadein, output_buffer<cint16> & __restrict dat_out)
{
    
    auto x_in = aie::cbegin_vector<8>(x_input);
    auto d_out= aie::begin_vector<8>(dat_out);
    auto c_in = aie::cbegin_vector<8>(c_input);
    for (unsigned prbcnt=0; prbcnt<NUM_PRBS; ++prbcnt)
        chess_prepare_for_pipelining
    {
        for (unsigned i=0; i<12; ++i)  
        chess_prepare_for_pipelining
        chess_unroll_loop(2)
        {
            aie::vector<cint16, 8> bufa0 = *c_in++;
            aie::vector<cint16, 8> bufa1 = *c_in++;
            aie::vector<cint16, 8> bufb0;
            aie::vector<cint16, 8> bufb1;

            aie::vector<cint16, 8 > dat0 = *x_in++;
            aie::accum<cacc48 , 8> acc1;
            cascadein >> acc1;
            acc1      = aie::mac(acc1, dat0[0], bufa0); bufb0 = *c_in++; 
            acc1      = aie::mac(acc1, dat0[1], bufa1); bufb1 = *c_in++; 

            acc1      = aie::mac(acc1, dat0[2], bufb0); bufa0 = *c_in++; 
            acc1      = aie::mac(acc1, dat0[3], bufb1); bufa1 = *c_in++; 
 
            acc1      = aie::mac(acc1, dat0[4], bufa0); bufb0 = *c_in++; 
            acc1      = aie::mac(acc1, dat0[5], bufa1); bufb1 = *c_in;   

            acc1      = aie::mac(acc1, dat0[6], bufb0); 
            acc1      = aie::mac(acc1, dat0[7], bufb1); 
            *d_out++ =  acc1.to_vector<cint16>(SHFT);
            c_in = c_in - 7;
        }
        c_in = c_in + 8;
    }
}

#endif //INLINE

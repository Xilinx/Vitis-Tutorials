/*  (c) Copyright 2014 - 2021 Xilinx, Inc. All rights reserved.
   
    This file contains confidential and proprietary information
    of Xilinx, Inc. and is protected under U.S. and
    international copyright and other intellectual property
    laws.
   
    DISCLAIMER
    This disclaimer is not a license and does not grant any
    rights to the materials distributed herewith. Except as
    otherwise provided in a valid license issued to you by
    Xilinx, and to the maximum extent permitted by applicable
    law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
    WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
    AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
    BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
    INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
    (2) Xilinx shall not be liable (whether in contract or tort,
    including negligence, or under any other theory of
    liability) for any loss or damage of any kind or nature
    related to, arising under or in connection with these
    materials, including for any direct, or any indirect,
    special, incidental, or consequential loss or damage
    (including loss of data, profits, goodwill, or any type of
    loss or damage suffered as a result of any action brought
    by a third party) even if such damage or loss was
    reasonably foreseeable or Xilinx had been advised of the
    possibility of the same.
   
    CRITICAL APPLICATIONS
    Xilinx products are not designed or intended to be fail-
    safe, or for use in any application requiring fail-safe
    performance, such as life-support or safety devices or
    systems, Class III medical devices, nuclear facilities,
    applications related to the deployment of airbags, or any
    other applications that could lead to death, personal
    injury, or severe property or environmental damage
    (individually and collectively, "Critical
    Applications"). Customer assumes the sole risk and
    liability of any use of Xilinx products in Critical
    Applications, subject only to applicable laws and
    regulations governing limitations on product liability.
   
    THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
    PART OF THIS FILE AT ALL TIMES.                       */

#include <adf.h>
#include "utils.h"

#include "mixer_const.h"

void mixer_2out_init()
{
    set_rnd(rnd_conv_even); // rounding mode for computations 
    set_sat();
};

#include "aie_api/aie.hpp"
#include "aie_api/utils.hpp"
#include "aie_api/aie_adf.hpp"

void mixer_2out(       
    input_window_cint16 * restrict cb_input0,
    input_window_cint16 * restrict cb_input1,
    output_window_cint16 * restrict cb_output0,
    output_window_cint16 * restrict cb_output1
)
{
    const int shift = MIXER_SHIFT0 ;    
    const int shift1 = MIXER_SHIFT1 ;
    const unsigned output_samples = GET_NUM_SAMPLES(cb_input0);
    //every loop operation produces 8 output samples.
    
    const unsigned lc8 = (output_samples/8) ;
    //phase increment values for -10MHz
    //MATLAB equation used to generate phase increment value: 
    //phase_incr = mod(round(2^32*(fc)*VECTOR_LEN/fs),2^32);
    //phase_incrn10 = mod(round(2^32*(-10)*8/245.76),2^32);
    const uint32_t  phase_updaten50 = 0x5F555555;
    
    
    //dds_out is storage for sine/cos values generated from scalar function sincos
    aie::vector<cint16, 8> dds_out ;
    aie::vector<cint16, 8> buf0;
    aie::vector<cint16, 8> chess_storage(wr0) vdata0;
    aie::accum<cacc48, 8> acc0a; 
    aie::accum<cacc48, 8> acc0b;
    aie::accum<cacc48, 8> acc1;
    
    
    //pointer ww to ph_rot array as data type v8cint16
    buf0 = aie::load_v<8>(ph_rot122);
    
    set_sat() ; 
    
    
    for (unsigned l=0; l<lc8; ++l)
    chess_prepare_for_pipelining
    chess_loop_range(8,) 
        {   
        dds_out.set(sincos(PHASE_INDEX0), 0);
        PHASE_INDEX0 += phase_updaten50 ;
        acc0a = aie::mul(buf0, dds_out[0]);
        
        //multiply sine/cos vector to input channel 1
        vdata0 = window_readincr_v<8>(cb_input0);
        acc1 = aie::mul(vdata0, acc0a.to_vector<cint16>(shift));
        //use mul8_cc for complex conjugate multiplication to generate sine/cos vector for 10MHz
        //multiply sine/cos vector to input channel 2 and add 
        acc0b = aie::mul(aie::op_conj(buf0), aie::op_conj(dds_out[0]));
        vdata0 = window_readincr_v<8>(cb_input1);
        acc1 = aie::mac(acc1, vdata0, acc0b.to_vector<cint16>(shift));
        //write to output buffer
        window_writeincr(cb_output0, acc1.to_vector<cint16>(shift1)) ;
        window_writeincr(cb_output1, acc1.to_vector<cint16>(shift1)) ;
    }
}

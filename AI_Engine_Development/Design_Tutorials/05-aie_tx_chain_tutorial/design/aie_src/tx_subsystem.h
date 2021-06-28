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

#ifndef __TX_SUBSYSTEM_200MHz__
#define __TX_SUBSYSTEM_200MHz__

#include <adf.h>

#include "tx_chain_params.h" 
#include "wcfr_subsystem.h"

#include "commslib.h"

#include "tx_chain_kernels.h" 

using namespace adf ;

class interp_chain : public graph {

    public:
        kernel cf199;
        kernel hb43;

   
    public:
        port<input> in ;
        port<output> out ;
  
    interp_chain() {

        // Create kernels
        cf199 = kernel::create(fir_199t_sym) ;
        hb43 = kernel::create(fir_43t_sym_hb_2i) ;

        initialization_function(cf199) = "fir_199t_sym_init";
        initialization_function(hb43) =  "fir_43t_sym_hb_2i_init";


        // Make connections
        connect<window<CF199_INPUT_SAMPLES*4, CF199_INPUT_MARGIN*4> > (in, cf199.in[0]);
        connect<window<HB43_2I_INPUT_SAMPLES*4, HB43_2I_INPUT_MARGIN*4> >(cf199.out[0], hb43.in[0]);
        connect<window<MIXER_INPUT_SAMPLES*4> >(hb43.out[0], out);
    }; 
};

class mixer_subsystem : public graph {

    public:
        kernel mix_add ;
       
    public:
        port<input> in[MAX_NUM_CHANNEL] ;
        port<output> out[2]; // 2 outputs for PC-CFR datapath and peak detection

    mixer_subsystem() {

        // Create kernels
        mix_add = kernel::create(mixer_2out) ;
        initialization_function(mix_add)="mixer_2out_init";

        // Make connections      
        for(unsigned i=0; i<MAX_NUM_CHANNEL; i++){ 
            connect<window<MIXER_INPUT_SAMPLES*4> > (in[i], mix_add.in[i]);
        }
        connect<window<MIXER_INPUT_SAMPLES*4> > (mix_add.out[0], out[0]) ;
	connect<window<MIXER_INPUT_SAMPLES*4> > (mix_add.out[1], out[1]) ;

    };        
};    

class dpd_upsampler : public graph {

    public:
        port<input> in;
        port<output> out[2]; 

    public:
        // kernels
        kernel upsampler1;
        kernel upsampler2;
        kernel gain_magsqr1;

    dpd_upsampler() {

        upsampler1 = kernel::create(fir_43t_sym_hb_2i);
        upsampler2 = kernel::create(fir_15t_sym_hb_2i);
        gain_magsqr1 = kernel::create(gain_magsqr);

        initialization_function(upsampler1) =  "fir_43t_sym_hb_2i_init";
        initialization_function(upsampler2) =  "fir_15t_sym_hb_2i_init";
        initialization_function(gain_magsqr1) =  "gain_magsqr_kernel_init";

        // connect upsample_by_2 kernels
        connect< window<HB43_2I_2_INPUT_SAMPLES*4, HB43_2I_2_INPUT_MARGIN*4> >(in, upsampler1.in[0]);
        connect< window<HB15_2I_2_INPUT_SAMPLES*4, HB15_2I_2_INPUT_MARGIN*4>  >(upsampler1.out[0], upsampler2.in[0]);
        connect< window<DPD_GAIN_INPUT_SAMPLES*4> >(upsampler2.out[0], gain_magsqr1.in[0]);
		
        connect< window<DPD_GAIN_OUTPUT_SAMPLES*4> >(gain_magsqr1.out[0], out[0]);
        connect< window<DPD_GAIN_OUTPUT_SAMPLES*4> >(gain_magsqr1.out[1], out[1]);
    }; 
};

#endif //__TX_SUBSYSTEM_200MHz__

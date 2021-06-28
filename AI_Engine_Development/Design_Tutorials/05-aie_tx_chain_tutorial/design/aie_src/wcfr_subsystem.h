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
#ifndef __WCFR_SUBSYSTEM_H__
#define __WCFR_SUBSYSTEM_H__
#include <adf.h>
#include "wcfr_subsystem_params.h"
#include "fir_sr_sym_graph.hpp"
#include "filter_kernels.h"
#include "radio_kernels.h"

using namespace adf;

class wcfr_int_mag : public graph {      

    public:
        kernel wcfr_hb23;
        kernel wcfr_hb11;
        kernel wcfr_m_pa;
    
    public:
        port<input> in;
        port<output> out;
    
    wcfr_int_mag() {
        wcfr_hb23    = kernel::create(fir_23t_sym_hb_2i);
        wcfr_hb11    = kernel::create(fir_11t_sym_hb_2i);
        wcfr_m_pa    = kernel::create(wcfr_mag_pack);

        connect< window<WCFR_HB23_2I_INPUT_SAMPLES*4, WCFR_HB23_2I_INPUT_MARGIN*4> >(in, wcfr_hb23.in[0]);
        connect< window<WCFR_HB11_2I_INPUT_SAMPLES*4, WCFR_HB11_2I_INPUT_MARGIN*4> >(wcfr_hb23.out[0], wcfr_hb11.in[0]);
        connect< window<WCFR_M_PA_INPUT_SAMPLES*4, WCFR_M_PA_INPUT_MARGIN*4> >(wcfr_hb11.out[0], wcfr_m_pa.in[0]);
        connect< window<WCFR_M_PA_OUTPUT_SAMPLES*4> >(wcfr_m_pa.out[0], out);

    };
};
 
class wcfr_filt_sub : public graph {      

    public:
    //    kernel wcfr_sub;
        kernel wcfr_correc;
    
    public:
        port<input> in0;
        port<input> in1;
        port<output> out;
        std::vector<int16> m_taps_wcfr_96t={-486,-972,-1458,-1944,-2429,-2915,-3401,-3887,-4373,-4859,-5345,-5831,-6317,-6803,-7288,
       -7774,-8260,-8746,-9232,-9718,-10204,-10690,-11176,-11662,-12147,-12633,-13119,-13605,-14091,-14577,-15063,-15549,-16035,-16521,
-16521,-16521,-16521,-16521,-16521,-16521,-16521,-16521,-16521,-16521,-16521,-16521,-16521,-16521};
        xf::dsp::aie::fir::sr_sym::fir_sr_sym_graph<int16, int16, 96, 20, 0, WCFR_RF_INPUT_SAMPLES/1> firGraph_wcfr_96t;
	
    wcfr_filt_sub():firGraph_wcfr_96t(m_taps_wcfr_96t) {
        wcfr_correc  = kernel::create(wcfr_correction);

        initialization_function(wcfr_correc) = "wcfr_corr_init";

        adf::connect< window<(WCFR_RF_INPUT_SAMPLES/2)*4, (WCFR_RF_INPUT_MARGIN/2)*4> >(in1, firGraph_wcfr_96t.in);
	adf::connect< window<(WCFR_RF_INPUT_SAMPLES/2)*4> >(firGraph_wcfr_96t.out, wcfr_correc.in[0]);
	connect< window<WCFR_RF_INPUT_SAMPLES*4, WCFR_DELAY> >(in0, wcfr_correc.in[1]);
        connect< window<(WCFR_RF_INPUT_SAMPLES)*4> >(wcfr_correc.out[0], out);
    };
};

class wcfr_graph : public graph {

    public:
        port<input> in0;
        port<input> in1;
        port<input> wcfr_pl_out;
        port<output> out; 
        port<output> wcfr_pl_in; 

    public:
        // WCFR sub-graph
        wcfr_int_mag wcfr_mag;
        wcfr_filt_sub wcfr_fs;

    wcfr_graph() {

        // WCFR - upsample and abs
        connect<> (in0, wcfr_mag.in) ;
        // WCFR - real filter and gain  
        connect<> (in1, wcfr_fs.in0) ;

        // WCFR - to PL peak detect 
        connect<> (wcfr_mag.out, wcfr_pl_in);
        // WCFR - from PL
        connect<> (wcfr_pl_out, wcfr_fs.in1); 

        connect<> (wcfr_fs.out, out);
    }; 

  };
#endif /* __WCFR_SUBSYSTEM_H__ */

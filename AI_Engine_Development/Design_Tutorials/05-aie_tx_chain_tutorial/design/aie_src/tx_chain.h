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

#ifndef __TX_CHAIN_200__
#define __TX_CHAIN_200__
#include <adf.h>

#include "tx_subsystem.h" 

#define DPDPLACE(xoff, yoff, dir)		       \
    location<kernel>(dpd_filter.dpd_core0) = tile((xoff),(yoff)); \
    location<kernel>(dpd_filter.c2l[0]) = tile((xoff),(yoff+1)); \
    location<kernel>(dpd_filter.c2l[1]) = tile((xoff+2*dir),(yoff+1))

using namespace adf ;

// ********************************************************************************** //
// ---------------------------------------------------------------------------------- //
//               A I    E N G I N E    G R A P H    D E F I N I T I O N               //
// ---------------------------------------------------------------------------------- //
// ********************************************************************************** //



namespace Adf_DFE {

template <int xoff, int yoff, int dir>
class tx_chain_200MHz : public adf::graph {

    public:

    // ============================================================================
    // Step 1: Define SDF Graph
    // ============================================================================
    // o) Declare input and output ports to graph
    // o) Declare all AI Engine subgraphs and some PL Kernels
    // ============================================================================
    
    // ------------------------------------------------------------------------------------
    // Step 1.1: Declare the input and output ports 
    // ------------------------------------------------------------------------------------

        port<input> in[MAX_NUM_CHANNEL];
        port<output> out;

    // ------------------------------------------------------------------------------------
    // Step 1.2: Declare the AI Engine subgraphs and some PL kernels  
    // ------------------------------------------------------------------------------------


        // Interpolation subgraphs
        interp_chain channel[MAX_NUM_CHANNEL];

        // Mixer sub-graph 
        mixer_subsystem mixer ;

        // PC-CFR sub-graph 
        pc_cfr_chain_itr_win2 pccfr_itr12;
        //PCCFR PL block
        kernel pccfr_pl1;
        kernel pccfr_pl2;

        // WCFR sub-graph
        wcfr_graph wcfr_g;
        // WCFR PL
        kernel wcfr_pl;

        // DPD upsampler of 8by5 and upsampler by 2
        dpd_upsampler dpd_upsamp;

        // DPD filter graph
        DPD_LUT_FILT_1G_32T dpd_filter;


    // ============================================================================
    // Step 2: Define the graph constructor. 
    // ============================================================================
    // 
    //   o) Create PL kernels
    //   o) Connect all kernels and subgraphs according to desired dataflow
    //   o) Define location constraints
    //   o) Link source files of each kernel
    //   o) Specify runtimes  
    // ============================================================================


    tx_chain_200MHz() {

      // ------------------------------------------------------------------------------------
      // Step 2.1: Create the PL kernels and set their clock frequencies
      // ------------------------------------------------------------------------------------

        
        // WCFR PL
        wcfr_pl    = kernel::create(wcfr_peak_d);
        fabric<pl>(wcfr_pl);

        // PCCFR PL
        pccfr_pl1    = kernel::create(pccfr_pl_pds_itr1);
        fabric<pl>(pccfr_pl1);
        pccfr_pl2    = kernel::create(pccfr_pl_pds_itr2);
        fabric<pl>(pccfr_pl2);
        

       pl_frequency(pccfr_pl1)=245.76;
       pl_frequency(pccfr_pl2)=245.76;
       pl_frequency(wcfr_pl)=245.76;

      // ------------------------------------------------------------------------------------
      // Step 2.2: Create nets to connect AI Engine subgraphs and PL kernels to each other and 
      //           to IO ports. These connections will create the desired dataflow. 
      // ------------------------------------------------------------------------------------
      
      // Connect IN[] ports to the Interpolation chain (channel) subgraphs 
      // and connect the CHANNEL subgraphs to the MIXER subgraphs in a for-loop. 

        // Make connections      
        // Interpolation chain
        for(unsigned i=0; i<MAX_NUM_CHANNEL; i++){ 
            connect<> (in[i], channel[i].in);
            connect<> (channel[i].out, mixer.in[i]);
        }

      // Connect MIXER subgraph to PCCFR subgraph
        connect< window<FORK_INPUT_SAMPLES*4> >(mixer.out[0], pccfr_itr12.in0);
        connect< window<FORK_INPUT_SAMPLES*4> >(mixer.out[1], pccfr_itr12.in1);

      // Connect PCCFR subgraph to PCCFR PL kernels
        connect< stream > (pccfr_itr12.pccfr_pl_in0, pccfr_pl1.in[0]);
        connect< stream > (pccfr_itr12.pccfr_pl_in1, pccfr_pl2.in[0]);
        
      // Connect PCCFR PL kernels back to PCCFR subgraph
        connect< stream > (pccfr_pl1.out[0], pccfr_itr12.pccfr_pl1_out0);
        connect< stream > (pccfr_pl1.out[1], pccfr_itr12.pccfr_pl1_out1);
        connect< stream > (pccfr_pl2.out[0], pccfr_itr12.pccfr_pl2_out0);
        connect< stream > (pccfr_pl2.out[1], pccfr_itr12.pccfr_pl2_out1);

      // Connect PCCFR subgraph to WCFR subgraph
        connect<> (pccfr_itr12.out0, wcfr_g.in0) ;
        connect<> (pccfr_itr12.out1, wcfr_g.in1) ;

      // Connect WCFR subgraph to WCFR PL kernel
        connect< stream > (wcfr_g.wcfr_pl_in, wcfr_pl.in[0]) ;
      
      // Connect WCFR PL kernel back WCFR subgraph
        connect< stream> (wcfr_pl.out[0], wcfr_g.wcfr_pl_out) ;
      
      // Connect WCFR subgraph to DPD_UPSAMP subgraph 
        connect<>(wcfr_g.out, dpd_upsamp.in);
	
      // Connect DPD_UPSAMP subgraph to DPD_FILTER subgraph
        connect<window<DPD_GAIN_INPUT_SAMPLES*4>, stream> (dpd_upsamp.out[0], dpd_filter.datain[0]);
        connect<window<DPD_GAIN_INPUT_SAMPLES*4>, stream> (dpd_upsamp.out[0], dpd_filter.datain[1]);
        connect<window<DPD_GAIN_INPUT_SAMPLES*4>, stream> (dpd_upsamp.out[0], dpd_filter.datain[2]);
        connect<window<DPD_GAIN_INPUT_SAMPLES*4>, stream> (dpd_upsamp.out[0], dpd_filter.datain[3]);
        connect<window<DPD_GAIN_INPUT_SAMPLES*4>, stream> (dpd_upsamp.out[1], dpd_filter.magin[0]);
        connect<window<DPD_GAIN_INPUT_SAMPLES*4>, stream> (dpd_upsamp.out[1], dpd_filter.magin[1]);
        connect<window<DPD_GAIN_INPUT_SAMPLES*4>, stream> (dpd_upsamp.out[1], dpd_filter.magin[2]);
        connect<window<DPD_GAIN_INPUT_SAMPLES*4>, stream> (dpd_upsamp.out[1], dpd_filter.magin[3]);

	// Connect DPD_FILTER subgraph to OUT port 
        connect<window<DPD_GAIN_INPUT_SAMPLES*4>, stream> (dpd_filter.dataout, out);


        adf::kernel *kernels_wcfr_96t   = wcfr_g.wcfr_fs.firGraph_wcfr_96t.getKernels();

       // ---------------------------------------------------------------------------------------
       // Step 2.3: Specify relative and absolute location constraints to specify the placement 
       //           of kernels at specific AI Engine tiles. 
       // ---------------------------------------------------------------------------------------
      
       // Absolute and Relative locations constraints for Interpolation chain (CHANNEL) and MIXER subgraphs


        // Core pinning and clustering 
        DPDPLACE(xoff, yoff, dir);
        location<kernel>(channel[0].hb43)                    = location<kernel>(channel[1].hb43) ;
        location<kernel>(mixer.mix_add)                      = location<kernel>(channel[0].hb43) ;
        location<kernel>(mixer.mix_add)                      = location<kernel>(pccfr_itr12.pccfr_sma2.pccfr_hb23_2) ;
        location<kernel>(wcfr_g.wcfr_mag.wcfr_hb23)          = location<kernel>(wcfr_g.wcfr_mag.wcfr_hb11);
        location<kernel>(wcfr_g.wcfr_mag.wcfr_m_pa)          = location<kernel>(wcfr_g.wcfr_mag.wcfr_hb11);
        location<kernel>(channel[0].cf199)                   = tile(xoff+2*dir, yoff+3) ;
        location<kernel>(channel[1].cf199)                   = tile(xoff+3*dir, yoff+1) ;
        location<kernel>(mixer.mix_add)                      = tile(xoff+3*dir, yoff+3) ;

      // Absolute location constraint on the PCCFR subgraph 
	location<kernel>(pccfr_itr12.pccfr_sma2.pccfr_sadd1) = tile(xoff+3*dir, yoff+2) ;
	location<kernel>(pccfr_itr12.pccfr_sma2.pccfr_sadd2) = tile(xoff+2*dir, yoff+2) ;
	     
      // Absolute and Relative location constraints for WCFR subgraph
        location<kernel>(wcfr_g.wcfr_mag.wcfr_m_pa)          = tile(xoff+1*dir, yoff+1) ;
        location<kernel>(kernels_wcfr_96t[0])                = tile(xoff+0*dir, yoff+3) ;
	location<kernel>(wcfr_g.wcfr_fs.wcfr_correc)         = tile(xoff+0*dir, yoff+2) ;
	
      // Absolute location constraint for DPD filter
        location<kernel>(dpd_upsamp.gain_magsqr1)            = tile(xoff+1*dir, yoff+2) ;

        // Buffer relative and absolute pinning
        location<buffer>(channel[0].cf199.in[0])                     = {bank((xoff+2*dir),yoff+3,2),bank((xoff+2*dir),yoff+3,3)};
        location<buffer>(channel[1].cf199.in[0])                     = {bank((xoff+3*dir),yoff+0,2),bank((xoff+3*dir),yoff+0,3)};
        location<buffer>(channel[0].cf199.out[0])                    = {bank((xoff+3*dir),yoff+3,0),bank((xoff+3*dir),yoff+3,2)};
        location<buffer>(channel[1].cf199.out[0])                    = {bank((xoff+3*dir),yoff+2,2),bank((xoff+3*dir),yoff+2,3)};
        location<buffer>(channel[0].hb43.out[0])                     = {bank((xoff+3*dir),yoff+3,2),bank((xoff+3*dir),yoff+3,2)};
        location<buffer>(channel[1].hb43.out[0])                     = {bank((xoff+3*dir),yoff+3,3),bank((xoff+3*dir),yoff+3,3)};
        location<buffer>(mixer.mix_add.out[0])                       = {bank((xoff+3*dir),yoff+3,2),bank((xoff+3*dir),yoff+3,2)};
        location<buffer>(mixer.mix_add.out[1])                       = {bank((xoff+3*dir),yoff+2,0),bank((xoff+3*dir),yoff+2,1)};
        location<buffer>(pccfr_itr12.pccfr_sma2.pccfr_hb23_2.out[0]) = {bank((xoff+3*dir),yoff+2,2),bank((xoff+3*dir),yoff+2,3)};
        location<buffer>(pccfr_itr12.pccfr_sma2.pccfr_sadd1.in[1])   = {bank((xoff+3*dir),yoff+3,1),bank((xoff+3*dir),yoff+3,3)};
        location<buffer>(pccfr_itr12.pccfr_sma2.pccfr_sadd1.in[0])   = location<buffer>(pccfr_itr12.pccfr_sma2.pccfr_sadd1.in[2]);
        location<parameter>(pccfr_itr12.pccfr_sma2.cp_lut0_1)        = bank((xoff+2*dir),yoff+2,2); 
        location<parameter>(pccfr_itr12.pccfr_sma2.cp_lut1_1)        = bank((xoff+2*dir),yoff+2,3);
        location<buffer>(pccfr_itr12.pccfr_sma2.pccfr_sadd1.out[1])  = {bank((xoff+2*dir),yoff+2,0),bank((xoff+2*dir),yoff+2,1)}; 
        location<buffer>(pccfr_itr12.pccfr_sma2.pccfr_hb23_1.out[0]) = {bank((xoff+2*dir),yoff+2,1),bank((xoff+2*dir),yoff+2,0)};
        location<buffer>(pccfr_itr12.pccfr_sma2.pccfr_sadd2.in[1])   = {bank((xoff+1*dir),yoff+2,2),bank((xoff+1*dir),yoff+2,3)};
        location<buffer>(pccfr_itr12.pccfr_sma2.pccfr_sadd2.in[0])   = {bank((xoff+1*dir),yoff+2,1),bank((xoff+1*dir),yoff+2,0)};
        location<buffer>(pccfr_itr12.pccfr_sma2.pccfr_sadd2.out[1])  = {bank((xoff+1*dir),yoff+2,3),bank((xoff+1*dir),yoff+2,2)}; 
        location<buffer>(pccfr_itr12.pccfr_sma2.pccfr_sadd2.out[0])  = {bank((xoff+1*dir),yoff+2,0),bank((xoff+1*dir),yoff+2,1)}; 
        location<parameter>(pccfr_itr12.pccfr_sma2.cp_lut0_2)        = bank((xoff+2*dir),yoff+3,0); 
        location<parameter>(pccfr_itr12.pccfr_sma2.cp_lut1_2)        = bank((xoff+2*dir),yoff+3,1);
        location<buffer>(wcfr_g.wcfr_mag.wcfr_hb23.out[0])           = {bank((xoff+1*dir),yoff+0,0),bank((xoff+1*dir),yoff+0,0)};
        location<buffer>(wcfr_g.wcfr_mag.wcfr_hb11.out[0])           = {bank((xoff+1*dir),yoff+0,1),bank((xoff+1*dir),yoff+0,1)};
        location<buffer>(wcfr_g.wcfr_mag.wcfr_m_pa.out[0])           = {bank((xoff+1*dir),yoff+0,2),bank((xoff+1*dir),yoff+0,3)};
        location<buffer>(kernels_wcfr_96t[0].in[0])                  = {bank((xoff+0*dir),yoff+3,0),bank((xoff+0*dir),yoff+3,1)};
        location<buffer>(kernels_wcfr_96t[0].out[0])                 = {bank((xoff+0*dir),yoff+3,2),bank((xoff+0*dir),yoff+3,3)};
        location<buffer>(wcfr_g.wcfr_fs.wcfr_correc.in[1])           = {bank((xoff+0*dir),yoff+2,0),bank((xoff+0*dir),yoff+2,1)};
        location<buffer>(wcfr_g.wcfr_fs.wcfr_correc.out[0])          = {bank((xoff+0*dir),yoff+2,2),bank((xoff+0*dir),yoff+2,2)};
        location<buffer>(dpd_upsamp.upsampler1.out[0])               = {bank((xoff+0*dir),yoff+2,2),bank((xoff+0*dir),yoff+2,3)};
        location<buffer>(dpd_upsamp.upsampler2.out[0])               = {bank((xoff+0*dir),yoff+2,1),bank((xoff+0*dir),yoff+2,1)};
        location<buffer>(dpd_upsamp.gain_magsqr1.out[0])             = {bank((xoff+1*dir),yoff+3,0),bank((xoff+1*dir),yoff+3,1)};
        location<buffer>(dpd_upsamp.gain_magsqr1.out[1])             = {bank((xoff+1*dir),yoff+3,2),bank((xoff+1*dir),yoff+3,3)};

      // ---------------------------------------------------------------------------------------
      // Step 2.4: Specify kernel source files and tile usage for the AI Engine and PL kernels.
      // o) Specify source files for AI Engine kernels. Paths should be with respect to current 
      //    working or proj path.   
      // o) Specify runtime ratios (between 0 and 1)
      // ---------------------------------------------------------------------------------------
      
      //Interpolation chain (CHANNEL subgraph) kernels source 
        for(unsigned i=0; i<MAX_NUM_CHANNEL; i++){ 
            // Sources for Interpolation chain
            source(channel[i].cf199) = "fir_199t_sym.cc" ;
            source(channel[i].hb43) = "fir_43t_sym_hb_2i.cc";
            // Specify mapping constraints
            runtime<ratio>(channel[i].cf199)  = 0.8  ;
            runtime<ratio>(channel[i].hb43)  = 0.1  ;
        }
      
      //MIXER subgraph kernel source 
        // Mixer sources
        source(mixer.mix_add) = "mixer_2out.cc";

      //PCCFR subgraph kernels sources
        //PCCFR Sources - kernel paths should be with respect to current working or proj path 
        source(pccfr_itr12.pccfr_sma2.pccfr_sadd1) = "pccfr_scaleadd_itr_window2.cc" ;
        source(pccfr_itr12.pccfr_sma2.pccfr_sadd2) = "pccfr_scaleadd_itr_window2.cc" ;
        source(pccfr_itr12.pccfr_sma2.pccfr_hb23_1) = "fir_23t_sym_hb_2i.cc" ;
        source(pccfr_itr12.pccfr_sma2.pccfr_hb23_2) = "fir_23t_sym_hb_2i.cc" ;
	
      //WCFR subgraph kernels Sources         
        //WCFR Sources    
        source(wcfr_g.wcfr_mag.wcfr_hb23) = "fir_23t_sym_hb_2i.cc" ;
        source(wcfr_g.wcfr_mag.wcfr_hb11) = "fir_11t_sym_hb_2i.cc" ;
        source(wcfr_g.wcfr_mag.wcfr_m_pa) = "wcfr_mag_pack.cc" ;
        source(wcfr_g.wcfr_fs.wcfr_correc) = "wcfr_correction.cc" ;


      //WCFR PL kernel source
        //WCFR PL block 
        source(wcfr_pl) = "wcfr_pd.cpp" ;

      //PCCFR PL kernel source
        //PCCFR PL block
        source(pccfr_pl1) = "pccfr_pds_itr1.cpp" ;
        source(pccfr_pl2) = "pccfr_pds_itr2.cpp" ;


        // Utilization constraints
        runtime<ratio>(mixer.mix_add) = 0.2 ;
     

        runtime<ratio>(pccfr_itr12.pccfr_sma2.pccfr_sadd1)  = 0.53 ;
        runtime<ratio>(pccfr_itr12.pccfr_sma2.pccfr_sadd2)  = 0.53 ;
        runtime<ratio>(pccfr_itr12.pccfr_sma2.pccfr_hb23_1)  = 0.1 ;
        runtime<ratio>(pccfr_itr12.pccfr_sma2.pccfr_hb23_2)  = 0.1 ;

        runtime<ratio>(wcfr_g.wcfr_mag.wcfr_hb23)  = 0.21  ;
        runtime<ratio>(wcfr_g.wcfr_mag.wcfr_hb11)  = 0.21  ;
        runtime<ratio>(wcfr_g.wcfr_mag.wcfr_m_pa)  = 0.3  ;
        adf::runtime<ratio>(kernels_wcfr_96t[0])   = 0.82  ;
        runtime<ratio>(wcfr_g.wcfr_fs.wcfr_correc) = 0.1  ;

        // source code for DPD Upsampling subsystem kernels
        source(dpd_upsamp.upsampler1) = "fir_43t_sym_hb_2i.cc";
        source(dpd_upsamp.upsampler2) = "fir_15t_sym_hb_2i.cc";
        source(dpd_upsamp.gain_magsqr1) = "gain_magsqr.cc";

        // Utilization constraints
        runtime<ratio>(dpd_upsamp.upsampler1) = 0.4;
        runtime<ratio>(dpd_upsamp.upsampler2) = 0.3;
        runtime<ratio>(dpd_upsamp.gain_magsqr1) = 0.5;

       // Source code for DPD kernels
       source(dpd_filter.dpd_core0)  = "dpd_1g_core0.c" ;
       source(dpd_filter.dpd_core1)  = "dpd_1g_core1.c" ;
       source(dpd_filter.dpd_core2)  = "dpd_1g_core2.c" ;
       source(dpd_filter.dpd_core3)  = "dpd_1g_core3.c" ;
       source(dpd_filter.c2l[0]) = "c2l_kernel.cc";
       source(dpd_filter.c2l[1]) = "c2l_kernel.cc";

       // Utilization constraints for DPD and C2L
       runtime<ratio>(dpd_filter.dpd_core0) = 0.86;
       runtime<ratio>(dpd_filter.dpd_core1) = 0.86;
       runtime<ratio>(dpd_filter.dpd_core2) = 0.86;
       runtime<ratio>(dpd_filter.dpd_core3) = 0.86;
       runtime<ratio>(dpd_filter.c2l[0]) = 0.85;
       runtime<ratio>(dpd_filter.c2l[1]) = 0.85;
        
    }; 
};

};


#endif // __TX_CHAIN_200__

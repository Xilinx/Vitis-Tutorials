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


#ifndef __TX_CHAIN_KERNELS_200MHz_H__
#define __TX_CHAIN_KERNELS_200MHz_H__

// This file contains kernel prototypes for all the 
// locally used kernels which are not a part of CommsLib

#include <adf.h>
#include "ap_int.h"
#include "hls_stream.h"
void pccfr_pl_pds_itr1(adf::dir::in<hls::stream<ap_axis<64,0,0,0>>&> in_sample1,
                       adf::dir::out<hls::stream<ap_axis<32,0,0,0>>&> out0_itr0, adf::dir::out<hls::stream<ap_axis<32,0,0,0>>&> out0_itr1);
void pccfr_pl_pds_itr2(adf::dir::in<hls::stream<ap_axis<64,0,0,0>>&> in_sample1,
                       adf::dir::out<hls::stream<ap_axis<32,0,0,0>>&> out0_itr0, adf::dir::out<hls::stream<ap_axis<32,0,0,0>>&> out0_itr1);
void wcfr_peak_d(adf::dir::in<hls::stream<ap_axis<64,0,0,0>>&> in_sample, adf::dir::out<hls::stream<ap_axis<32,0,0,0>>&> out);
void mixer_2out(input_window_cint16 * restrict cb_input0, input_window_cint16 * restrict cb_input1, output_window_cint16 * restrict cb_output0, output_window_cint16 * restrict cb_output1);

#endif //__TX_CHAIN_KERNELS_200MHz_H__

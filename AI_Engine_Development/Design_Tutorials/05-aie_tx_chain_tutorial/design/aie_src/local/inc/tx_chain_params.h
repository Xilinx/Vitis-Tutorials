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


#ifndef __TX_CHAIN_PARAMS_200MHz_H__
#define __TX_CHAIN_PARAMS_200MHz_H__

//Configurable parameters for Tx Chain 

// Number of antennas: preferred values 1, 2 & 4
#ifndef NUM_OF_ANT
#define NUM_OF_ANT              1
#endif

//Number of Samples in input Block: preferred values N: 160 or 320.,  N*2/5 must be a divisible by 64
#define  TX_CHAIN_INPUT_SAMPLES         128

//These number of samples are used in 1in-2out kernel
#define  FORK_INPUT_SAMPLES             2*TX_CHAIN_INPUT_SAMPLES


//Constants for TX-Chain

#define CEIL_TO_8(x) (((x+7)/8)*8)

// Number of channels
#define MAX_NUM_CHANNEL                 2

//PC-CFR defs used to compute delays
#define CP_NTAPS_200MHZ                 321
#define DET_WINDOW_ITR1                 16
#define DET_WINDOW_ITR2                 8

// 199tap channel filter 
#define CF199_COEFFICIENTS_REF    	199
#define CF199_INPUT_SAMPLES	        TX_CHAIN_INPUT_SAMPLES
#define CF199_INPUT_MARGIN		208
#define CF199_OUTPUT_SAMPLES		CF199_INPUT_SAMPLES

// 43tap HB-1 filter 
#define HB43_2I_COEFFICIENTS_REF    	43
#define HB43_2I_INPUT_SAMPLES           CF199_OUTPUT_SAMPLES                        
#define HB43_2I_INPUT_MARGIN            (CEIL_TO_8(HB43_2I_COEFFICIENTS_REF/2))
#define HB43_2I_OUTPUT_SAMPLES          HB43_2I_INPUT_SAMPLES*2

// Mixer 
#define MIXER_INPUT_SAMPLES             (HB43_2I_OUTPUT_SAMPLES)
#define MIXER_INPUT_MARGIN              (0)
#define MIXER_OUTPUT_SAMPLES            (MIXER_INPUT_SAMPLES)


//PCCFR_INPUT_SAMPLES value should also be given in pccfr_const.h used in pccfr_pds_itr12.cpp
#define PCCFR_INPUT_SAMPLES             MIXER_OUTPUT_SAMPLES
#define PCCFR_INPUT_SAMPLES_PL          MIXER_OUTPUT_SAMPLES

// 23tap HB 2i for 1st stage upsampling in ME for PC-CFR
#define HB23_2ndstg_COEFFICIENTS_REF    23

#define HB23_2ndstg_INPUT_SAMPLES       MIXER_OUTPUT_SAMPLES                        
#define HB23_2ndstg_INPUT_MARGIN        (CEIL_TO_8(HB23_2ndstg_COEFFICIENTS_REF/2))
#define HB23_2ndstg_OUTPUT_SAMPLES      HB23_2ndstg_INPUT_SAMPLES*2

//delay computation: half of CP length + detection window + PL delay + feedback buffer size delay
#define PCCFR_DELAY_ITR1_WIN            (4*CEIL_TO_8(((CP_NTAPS_200MHZ - 1)>>1) + DET_WINDOW_ITR1 + 16 + PCCFR_INPUT_SAMPLES))
//no feedback buffer size delay is programmed for iteration 2 as it is inherent because of connection
#define PCCFR_DELAY_ITR2_WIN            (4*CEIL_TO_8(((CP_NTAPS_200MHZ - 1)>>1) + DET_WINDOW_ITR2 + 16 + PCCFR_INPUT_SAMPLES))

#define WCFR_INPUT_SAMPLES              MIXER_OUTPUT_SAMPLES

//23tap HB 2I
#define WCFR_HB23_2I_COEFFICIENTS_REF   23
#define WCFR_HB23_2I_INPUT_SAMPLES      MIXER_OUTPUT_SAMPLES                        
#define WCFR_HB23_2I_INPUT_MARGIN       (CEIL_TO_8(WCFR_HB23_2I_COEFFICIENTS_REF/2))
#define WCFR_HB23_2I_OUTPUT_SAMPLES     WCFR_HB23_2I_INPUT_SAMPLES*2

//11tap HB 2i 
#define WCFR_HB11_2I_COEFFICIENTS_REF   11
#define WCFR_HB11_2I_INPUT_SAMPLES      WCFR_HB23_2I_OUTPUT_SAMPLES                        
#define WCFR_HB11_2I_INPUT_MARGIN       (CEIL_TO_8(WCFR_HB11_2I_COEFFICIENTS_REF/2))
#define WCFR_HB11_2I_OUTPUT_SAMPLES     WCFR_HB11_2I_INPUT_SAMPLES*2

//WCFR Magnitude params
#define WCFR_M_PA_INPUT_SAMPLES         WCFR_HB11_2I_OUTPUT_SAMPLES                        
#define WCFR_M_PA_INPUT_MARGIN          (0)
#define WCFR_M_PA_OUTPUT_SAMPLES        WCFR_M_PA_INPUT_SAMPLES/2

//WCFR gain and subtract params
#define WCFR_RF_COEFFICIENTS_REF        96
#define WCFR_RF_INPUT_SAMPLES           MIXER_OUTPUT_SAMPLES                        
#define WCFR_RF_INPUT_MARGIN            (CEIL_TO_8(WCFR_RF_COEFFICIENTS_REF - 1))
#define WCFR_RF_OUTPUT_SAMPLES          WCFR_RF_INPUT_SAMPLES

#define WCFR_DELAY                      (4*(CEIL_TO_8(WCFR_RF_COEFFICIENTS_REF  + 7)))

// 43tap HB-2 filter 
#define HB43_2I_2_COEFFICIENTS_REF    	43
#define HB43_2I_2_INPUT_SAMPLES         MIXER_OUTPUT_SAMPLES                        
#define HB43_2I_2_INPUT_MARGIN          (CEIL_TO_8(HB43_2I_2_COEFFICIENTS_REF/2))
#define HB43_2I_2_OUTPUT_SAMPLES        HB43_2I_2_INPUT_SAMPLES*2

// 15tap HB-2 filter 
#define HB15_2I_2_COEFFICIENTS_REF    	15
#define HB15_2I_2_INPUT_SAMPLES         HB43_2I_2_OUTPUT_SAMPLES                        
#define HB15_2I_2_INPUT_MARGIN          (CEIL_TO_8(HB15_2I_2_COEFFICIENTS_REF/2))
#define HB15_2I_2_OUTPUT_SAMPLES        HB15_2I_2_INPUT_SAMPLES*2

//DPD Pre-Gain parameters
#define DPD_GAIN_INPUT_SAMPLES          HB15_2I_2_OUTPUT_SAMPLES 
#define DPD_GAIN_OUTPUT_SAMPLES         DPD_GAIN_INPUT_SAMPLES 

// Pre-DPD Gain and associated shift parameter is in user_params.h


#endif //__TX_CHAIN_PARAMS_200MHz_H__

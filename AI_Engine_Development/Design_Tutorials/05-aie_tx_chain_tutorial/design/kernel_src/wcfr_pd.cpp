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

#include "wcfr_pl_const.h"
#include <stdint.h>
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define DUMP_VEC 0
#define HLS_VEC 0


//typedef  hls::stream<ap_axis<32, 0, 0, 0>> output_stream_int32;
//typedef  hls::stream<ap_axis<64, 0, 0, 0>> input_stream_int64;

#include <ap_int.h>
typedef ap_int<16>           ap_int16;
typedef ap_int<32>           ap_int32;
typedef ap_int<64>           ap_int64;

/*
template <typename T> T REG(T x) {
    #pragma HLS INTERFACE port=return register
    #pragma HLS inline self off
    return x;
}
*/
template <typename T>
T REG(volatile T x) {
    //#pragma HLS INTERFACE port=return register
    //#pragma HLS inline self off
    #pragma HLS inline off
    #pragma HLS LATENCY min=1 max=1
    return x;
}

//////////////////////////////////////////////////////////////////////
ap_int16 find_gain_new(ap_int16 max)
{
#pragma HLS INLINE
  ap_int32 gain_o;
  ap_uint<15> fp_sel_term1=0,fp_sel_term2=0,fp_sel_term3=0;
  ap_int16 fp_max;
  ap_uint<15> fp_OneByTH = 22370;//2^26*(1/TH) = A(-10,26)
  ap_int<31> fp_max_s;
  ap_int<6> fp_check_max_one;
  ap_int<15> fp_max_s_16;
  ap_int<19> fp_max_s_161;
  ap_int16 fp_max_s_sqr;
  ap_uint<1> check;

  fp_max           = max;                             //Assume A(0,16)
  fp_max_s         = fp_max*fp_OneByTH;               //A(0,15)*U(-10,26) = A(-9,26)
  fp_check_max_one = fp_max_s >> 25;                  //A(-10,1)
  fp_max_s_16      = fp_max_s >> 16;                  //A(5,10)
  fp_max_s_161     = fp_max_s >> 12;                  //A(5,10)
  //fp_max_s_sqr     = ((fp_max_s_16*fp_max_s_16)>>16); //A(5,10)*A(5,10)=A(11,20) >> 16 = A(11,4)//increase
  fp_max_s_sqr     = ((fp_max_s_16*fp_max_s_16)>>12); //A(5,10)*A(5,10)=A(11,20) >> 12 = A(11,8)//increase
  //32-bits variable It occupy maximum 11 bits
  check            = 0;

  if(fp_check_max_one >= 2){           //(>=1) FL > 1
    if(fp_check_max_one < 4){         //(<=2) FL < 2
      fp_sel_term1 = 25721;//1.570 (0,14) 15 bits
      fp_sel_term2 = 23592;//0.720 (0,15) 15 bits
      fp_sel_term3 = 18349;//0.140 (0,17) 14 bits
    }else if(fp_check_max_one < 8){ //(<=4) FL < 4
      fp_sel_term1 = 18185;//1.11  (0,14)
      fp_sel_term2 = 8191; //0.25  (0,15)
      fp_sel_term3 = 3407; //0.026 (0,17)
    }else{
      fp_sel_term1 = 12942;//0.790 (0,14)
      fp_sel_term2 = 2949; //0.090 (0,15)
      fp_sel_term3 = 655;  //0.005 (0,17)
    }
    check = 1;
  }else{
    check = 0;
  }
  //increase pression of acc
  ap_int<19>  mult3;
  ap_int<18>  mult2;
  ap_uint<14> mult1;
  ap_int16 acc,acc_sqr16;
  ap_int32 acc_sqr;
  ap_int<17> acc_sqr_mult_max;
  ap_int<17> acc_sqr_mult_max_minus1;
  ap_int32 acc_sqr_mult_max_minus1_mul_gain;
  ap_int<16> acc_sqr_mult_max_minus1_mul_gain_neg_gain,gain_out; 
  ap_int<31> mult_out;
  mult3 = (REG(fp_max_s_sqr * fp_sel_term3)>>(17-5));
  mult2 = (REG(fp_max_s_16  * fp_sel_term2)>>(16-4));
  mult1 = ((fp_sel_term1)>>1);//U(0,14)         = U(0,14)

  acc = (mult1)-(mult2)+(mult3);
  acc_sqr = REG(acc*acc)>>(5);                        //A(,13)
  acc_sqr16 = REG(acc*acc)>>(16-5);
  mult_out         = REG(acc_sqr16*fp_max_s_161);
  acc_sqr_mult_max = mult_out>>14;
  acc_sqr_mult_max_minus1 = REG(acc_sqr_mult_max-(1<<(13+2))); //A(,13+0)
  acc_sqr_mult_max_minus1_mul_gain = REG(acc_sqr_mult_max_minus1*(acc));
  acc_sqr_mult_max_minus1_mul_gain_neg_gain = REG(acc-(acc_sqr_mult_max_minus1_mul_gain>>16));//A(,13)-A(,28) = A(,13)
  gain_out  = REG((1<<13)-acc_sqr_mult_max_minus1_mul_gain_neg_gain);

  if(check == 0)
    gain_o = 0;
  else
    gain_o = gain_out*8;

  return(gain_o);
}
void wcfr_peak_det_core(
			ap_int64 tempL,
			ap_int32 * out
			)
{
#pragma HLS INLINE
#pragma HLS INTERFACE ap_ctrl_none port=return
  ap_int16 temp0 ;
  ap_int16 temp1 ;
  ap_int16 temp2 ;
  ap_int16 temp3 ;
  static ap_int16 runmax_reg[RUN_LEN];
#pragma HLS ARRAY_PARTITION variable=runmax_reg complete dim=1
  static ap_int16 runmax;
  static ap_int16 s_r_e[4];
  static ap_int16 s_r_o[4];
  //FILE *fp ;
  // MOdified code
  temp0=(ap_int16) (  tempL & 0x000000000000ffff );
  temp1=(ap_int16) ( (tempL & 0x00000000ffff0000)>>16 ) ;
  temp2=(ap_int16) ( (tempL & 0x0000ffff00000000)>>32 ) ;
  temp3=(ap_int16) ( (tempL & 0xffff000000000000)>>48 ) ;
  s_r_e[0] = s_r_e[2];//s_r_e[1];//0
  s_r_e[1] = s_r_e[3];//2
  s_r_e[2] = temp0; //s_r_e[3];//4
  s_r_e[3] = temp2;//6


  s_r_o[0] = s_r_o[2];//s_r_o[1];//1
  s_r_o[1] = s_r_o[3];//s_r_o[2];//3
  s_r_o[2] = temp1;//s_r_o[3];//5
  s_r_o[3] = temp3;


  //find max //1,2,3,4,5
  ap_int16 t0 = REG(s_r_e[1] > s_r_e[2]?s_r_e[1]:s_r_e[2]); //2,4
  ap_int16 t1 = REG(s_r_o[0] > s_r_o[1]?s_r_o[0]:s_r_o[1]); //1,3
  ap_int16 t2 = REG(t0 > t1?t0:t1); //1,2,3,4
  ap_int16 t3 = REG(s_r_o[2] > t2?s_r_o[2]:t2); //1,2,3,4,5

  //find max //0,6
  ap_int16 t4 = REG(s_r_e[3] > s_r_e[0]?s_r_e[3]:s_r_e[0]);
  ap_int16 max = REG(t3 > t4?t3:(ap_int16)(0));
  //writeincr(out, max);


  ap_int16 gain_o;
  gain_o=find_gain_new(max);
        
  //run max
  for(int i=0;i<RUN_LEN-1;i++){
    runmax_reg[i] = runmax_reg[i+1];
  }
  runmax_reg[RUN_LEN-1] = gain_o;
        
  runmax = 0;
  for(int i=0;i<RUN_LEN;i++){
    runmax = runmax > runmax_reg[i]?runmax:runmax_reg[i];
  }
  *out = runmax;
};

extern "C" {
void wcfr_peak_d(
		 hls::stream<ap_axis<64, 0, 0, 0> > & in_sample,
		 hls::stream<ap_axis<32, 0, 0, 0> > & out
		 )
{
#pragma HLS PIPELINE II=1
#pragma HLS INTERFACE axis register both port=out
#pragma HLS INTERFACE axis register both port=in_sample
#pragma HLS INTERFACE ap_ctrl_none port=return


  ap_int64 tempL;
  ap_int32 run_max_out;
  ap_int32 in_pl = 0;
  static ap_uint<1> sample_write = 0;
  static ap_int16 e_o_sample[2];
  ap_int32 eo_sample;
  static ap_int32 runmax_old = 0;
  ap_axis<32, 0, 0, 0> eo_sample_x;

  // Call the core module here
  ap_axis<64, 0, 0, 0> in_sample_x = in_sample.read();
  tempL = in_sample_x.data;

  //WRITE INPUT VECTORS
  wcfr_peak_det_core(tempL,&run_max_out);
  if (sample_write == 0) {
    e_o_sample[0] = runmax_old;
  }else{
    e_o_sample[1] = runmax_old;
    eo_sample = ((ap_int<32>)(e_o_sample[1]) << 16) | (e_o_sample[0] & 0xFFFF);
	eo_sample_x.data = eo_sample;
    out.write(eo_sample_x);
  }
  sample_write ^= (ap_uint<1>)(1);
  runmax_old = run_max_out ;
    
};
}


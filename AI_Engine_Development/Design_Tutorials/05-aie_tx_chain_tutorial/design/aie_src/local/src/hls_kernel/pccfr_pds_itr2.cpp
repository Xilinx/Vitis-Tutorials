/*  (c) Copyright 2014 - 2015 Xilinx, Inc. All rights reserved.
   
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


#include "pccfr_pl_const.h"

#define DUMP_VEC 0
#define HLS_VEC 0

#include <stdint.h>
#include <hls_stream.h>
typedef hls::stream<int64_t> output_stream_int64;
typedef hls::stream<int64_t> input_stream_int64;

#include <ap_int.h>
template <typename ELEMENT> struct complex_types {
  ELEMENT real;
  ELEMENT imag;
};

// Bit accurate data types used for HLS synthesis
typedef complex_types<ap_int<16> > ap_cint16;
typedef ap_int<16>                 ap_int16;
typedef ap_int<32>                 ap_int32;
typedef ap_uint<32>                ap_uint32;
typedef ap_int<64>                 ap_int64;
typedef ap_int<8>                  ap_int8;

struct CPG_data
{
  CPG_data():count((CP_NTAPS+1)/VEC_LEN){}
  ap_int32   ss0;
  ap_cint16  ss1;
  ap_int<6> count;
  ap_int<6> count_CP_half;
  ap_int8    phase;
  ap_uint<6> offset;
};

template <typename T> T REG(T x) {
    #pragma HLS INTERFACE port=return register
    #pragma HLS inline self off
    return x;
}


//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////SUB FUNCTIONS CORDIC TO FIND COS/SIN/ABS/////////////////////
//////////////////////////////////////////////////////////////////////////////////////
void cos_sin_mag (
		  ap_int16  x_I,
		  ap_int16  x_Q,
		  ap_int<16>*  magout,
		  ap_int<16>*  cos_fixed,
		  ap_int<16>*  sin_fixed)
{
  #pragma HLS PIPELINE II=1
  //#pragma HLS INLINE
  const int P_SCALE = 512 ;
  const int C_SCALE = 1 ;
  const int SHIFT_BITS = 16;//9;
  const int P_fixed = 39797;//311 ;
  const int nsteps = 9;//6 ;

  const ap_int<16> rotation_cos_lut[512]={
    -5498,       32303,       22544,       23781,       10350,       31090,       31083,       10374,
     2623,       32663,       27704,       17500,       17685,       27586,       32680,        2404,
    -1433,       32737,       25330,       20788,       14141,       29560,       32132,        6423,
     6669,       32082,       29667,       13914,       20982,       25169,       32725,       -1684,
    -3470,       32584,       23985,       22326,       12271,       30383,       31670,        8413,
     4657,       32435,       28743,       15736,       19373,       26428,       32766,         358,
      614,       32762,       26578,       19166,       15959,       28619,       32471,        4404,
     8659,       31603,       30478,       12034,       22513,       23810,       32556,       -3724,
    -4486,       32459,       23276,       23065,       11317,       30752,       31391,        9398,
     3642,       32565,       28237,       16625,       18538,       27020,       32739,        1381,
     -410,       32765,       25967,       19987,       15058,       29103,       32317,        5416,
     7668,       31858,       30088,       12980,       21758,       24501,       32656,       -2706,
    -2451,       32676,       24671,       21566,       13215,       29985,       31917,        7419,
     5668,       32274,       29220,       14830,       20189,       25810,       32761,        -665,
     1637,       32727,       27164,       18327,       16845,       28106,       32592,        3387,
     9643,       31317,       30839,       11076,       23246,       23095,       32423,       -4739,
    -4993,       32385,       22913,       23425,       10835,       30925,       31241,        9887,
     3133,       32618,       27974,       17065,       18114,       27306,       32713,        1893,
     -921,       32755,       25651,       20390,       14601,       29335,       32229,        5920,
     7170,       31974,       29881,       13449,       21373,       24838,       32694,       -2195,
    -2961,       32634,       24331,       21949,       12745,       30188,       31797,        7917,
     5163,       32359,       28985,       15285,       19783,       26122,       32768,        -154,
     1126,       32749,       26874,       18749,       16404,       28366,       32536,        3896,
     9152,       31464,       30662,       11557,       22882,       23455,       32494,       -4232,
    -3978,       32526,       23633,       22698,       11796,       30571,       31534,        8906,
     4150,       32504,       28493,       16182,       18958,       26727,       32756,         870,
      102,       32768,       26276,       19579,       15511,       28865,       32398,        4910,
     8165,       31734,       30287,       12508,       22138,       24158,       32610,       -3216,
    -1940,       32711,       25005,       21178,       13682,       29775,       32029,        6920,
     6172,       32182,       29448,       14372,       20590,       25491,       32747,       -1177,
     2148,       32698,       27447,       17900,       17283,       27840,       32641,        2878,
    10131,       31163,       31009,       10593,       23604,       22729,       32345,       -5245,
    -5245,       32345,       22729,       23604,       10593,       31009,       31163,       10131,
     2878,       32641,       27840,       17283,       17900,       27447,       32698,        2148,
    -1177,       32747,       25491,       20590,       14372,       29448,       32182,        6172,
     6920,       32029,       29775,       13682,       21178,       25005,       32711,       -1940,
    -3216,       32610,       24158,       22138,       12508,       30287,       31734,        8165,
     4910,       32398,       28865,       15511,       19579,       26276,       32768,         102,
      870,       32756,       26727,       18958,       16182,       28493,       32504,        4150,
     8906,       31534,       30571,       11796,       22698,       23633,       32526,       -3978,
    -4232,       32494,       23455,       22882,       11557,       30662,       31464,        9152,
     3896,       32536,       28366,       16404,       18749,       26874,       32749,        1126,
     -154,       32768,       26122,       19783,       15285,       28985,       32359,        5163,
     7917,       31797,       30188,       12745,       21949,       24331,       32634,       -2961,
    -2195,       32694,       24838,       21373,       13449,       29881,       31974,        7170,
     5920,       32229,       29335,       14601,       20390,       25651,       32755,        -921,
     1893,       32713,       27306,       18114,       17065,       27974,       32618,        3133,
     9887,       31241,       30925,       10835,       23425,       22913,       32385,       -4993,
    -4739,       32423,       23095,       23246,       11076,       30839,       31317,        9643,
     3387,       32592,       28106,       16845,       18327,       27164,       32727,        1637,
     -665,       32761,       25810,       20189,       14830,       29220,       32274,        5668,
     7419,       31917,       29985,       13215,       21566,       24671,       32676,       -2451,
    -2706,       32656,       24501,       21758,       12980,       30088,       31858,        7668,
     5416,       32317,       29103,       15058,       19987,       25967,       32765,        -410,
     1381,       32739,       27020,       18538,       16625,       28237,       32565,        3642,
     9398,       31391,       30752,       11317,       23065,       23276,       32459,       -4486,
    -3724,       32556,       23810,       22513,       12034,       30478,       31603,        8659,
     4404,       32471,       28619,       15959,       19166,       26578,       32762,         614,
      358,       32766,       26428,       19373,       15736,       28743,       32435,        4657,
     8413,       31670,       30383,       12271,       22326,       23985,       32584,       -3470,
    -1684,       32725,       25169,       20982,       13914,       29667,       32082,        6669,
     6423,       32132,       29560,       14141,       20788,       25330,       32737,       -1433,
     2404,       32680,       27586,       17685,       17500,       27704,       32663,        2623,
    10374,       31083,       31090,       10350,       23781,       22544,       32303,       -5498};
  
  const ap_int<16> rotation_sin_lut[512]={
    -32303 ,      -5498 ,     -23781 ,      22544 ,     -31090 ,      10350 ,     -10374 ,      31083,
    -32663 ,       2623 ,     -17500 ,      27704 ,     -27586 ,      17685 ,      -2404 ,      32680,
    -32737 ,      -1433 ,     -20788 ,      25330 ,     -29560 ,      14141 ,      -6423 ,      32132,
    -32082 ,       6669 ,     -13914 ,      29667 ,     -25169 ,      20982 ,       1684 ,      32725,
    -32584 ,      -3470 ,     -22326 ,      23985 ,     -30383 ,      12271 ,      -8413 ,      31670,
    -32435 ,       4657 ,     -15736 ,      28743 ,     -26428 ,      19373 ,       -358 ,      32766,
    -32762 ,        614 ,     -19166 ,      26578 ,     -28619 ,      15959 ,      -4404 ,      32471,
    -31603 ,       8659 ,     -12034 ,      30478 ,     -23810 ,      22513 ,       3724 ,      32556,
    -32459 ,      -4486 ,     -23065 ,      23276 ,     -30752 ,      11317 ,      -9398 ,      31391,
    -32565 ,       3642 ,     -16625 ,      28237 ,     -27020 ,      18538 ,      -1381 ,      32739,
    -32765 ,       -410 ,     -19987 ,      25967 ,     -29103 ,      15058 ,      -5416 ,      32317,
    -31858 ,       7668 ,     -12980 ,      30088 ,     -24501 ,      21758 ,       2706 ,      32656,
    -32676 ,      -2451 ,     -21566 ,      24671 ,     -29985 ,      13215 ,      -7419 ,      31917,
    -32274 ,       5668 ,     -14830 ,      29220 ,     -25810 ,      20189 ,        665 ,      32761,
    -32727 ,       1637 ,     -18327 ,      27164 ,     -28106 ,      16845 ,      -3387 ,      32592,
    -31317 ,       9643 ,     -11076 ,      30839 ,     -23095 ,      23246 ,       4739 ,      32423,
    -32385 ,      -4993 ,     -23425 ,      22913 ,     -30925 ,      10835 ,      -9887 ,      31241,
    -32618 ,       3133 ,     -17065 ,      27974 ,     -27306 ,      18114 ,      -1893 ,      32713,
    -32755 ,       -921 ,     -20390 ,      25651 ,     -29335 ,      14601 ,      -5920 ,      32229,
    -31974 ,       7170 ,     -13449 ,      29881 ,     -24838 ,      21373 ,       2195 ,      32694,
    -32634 ,      -2961 ,     -21949 ,      24331 ,     -30188 ,      12745 ,      -7917 ,      31797,
    -32359 ,       5163 ,     -15285 ,      28985 ,     -26122 ,      19783 ,        154 ,      32768,
    -32749 ,       1126 ,     -18749 ,      26874 ,     -28366 ,      16404 ,      -3896 ,      32536,
    -31464 ,       9152 ,     -11557 ,      30662 ,     -23455 ,      22882 ,       4232 ,      32494,
    -32526 ,      -3978 ,     -22698 ,      23633 ,     -30571 ,      11796 ,      -8906 ,      31534,
    -32504 ,       4150 ,     -16182 ,      28493 ,     -26727 ,      18958 ,       -870 ,      32756,
    -32768 ,        102 ,     -19579 ,      26276 ,     -28865 ,      15511 ,      -4910 ,      32398,
    -31734 ,       8165 ,     -12508 ,      30287 ,     -24158 ,      22138 ,       3216 ,      32610,
    -32711 ,      -1940 ,     -21178 ,      25005 ,     -29775 ,      13682 ,      -6920 ,      32029,
    -32182 ,       6172 ,     -14372 ,      29448 ,     -25491 ,      20590 ,       1177 ,      32747,
    -32698 ,       2148 ,     -17900 ,      27447 ,     -27840 ,      17283 ,      -2878 ,      32641,
    -31163 ,      10131 ,     -10593 ,      31009 ,     -22729 ,      23604 ,       5245 ,      32345,
    -32345 ,      -5245 ,     -23604 ,      22729 ,     -31009 ,      10593 ,     -10131 ,      31163,
    -32641 ,       2878 ,     -17283 ,      27840 ,     -27447 ,      17900 ,      -2148 ,      32698,
    -32747 ,      -1177 ,     -20590 ,      25491 ,     -29448 ,      14372 ,      -6172 ,      32182,
    -32029 ,       6920 ,     -13682 ,      29775 ,     -25005 ,      21178 ,       1940 ,      32711,
    -32610 ,      -3216 ,     -22138 ,      24158 ,     -30287 ,      12508 ,      -8165 ,      31734,
    -32398 ,       4910 ,     -15511 ,      28865 ,     -26276 ,      19579 ,       -102 ,      32768,
    -32756 ,        870 ,     -18958 ,      26727 ,     -28493 ,      16182 ,      -4150 ,      32504,
    -31534 ,       8906 ,     -11796 ,      30571 ,     -23633 ,      22698 ,       3978 ,      32526,
    -32494 ,      -4232 ,     -22882 ,      23455 ,     -30662 ,      11557 ,      -9152 ,      31464,
    -32536 ,       3896 ,     -16404 ,      28366 ,     -26874 ,      18749 ,      -1126 ,      32749,
    -32768 ,       -154 ,     -19783 ,      26122 ,     -28985 ,      15285 ,      -5163 ,      32359,
    -31797 ,       7917 ,     -12745 ,      30188 ,     -24331 ,      21949 ,       2961 ,      32634,
    -32694 ,      -2195 ,     -21373 ,      24838 ,     -29881 ,      13449 ,      -7170 ,      31974,
    -32229 ,       5920 ,     -14601 ,      29335 ,     -25651 ,      20390 ,        921 ,      32755,
    -32713 ,       1893 ,     -18114 ,      27306 ,     -27974 ,      17065 ,      -3133 ,      32618,
    -31241 ,       9887 ,     -10835 ,      30925 ,     -22913 ,      23425 ,       4993 ,      32385,
    -32423 ,      -4739 ,     -23246 ,      23095 ,     -30839 ,      11076 ,      -9643 ,      31317,
    -32592 ,       3387 ,     -16845 ,      28106 ,     -27164 ,      18327 ,      -1637 ,      32727,
    -32761 ,       -665 ,     -20189 ,      25810 ,     -29220 ,      14830 ,      -5668 ,      32274,
    -31917 ,       7419 ,     -13215 ,      29985 ,     -24671 ,      21566 ,       2451 ,      32676,
    -32656 ,      -2706 ,     -21758 ,      24501 ,     -30088 ,      12980 ,      -7668 ,      31858,
    -32317 ,       5416 ,     -15058 ,      29103 ,     -25967 ,      19987 ,        410 ,      32765,
    -32739 ,       1381 ,     -18538 ,      27020 ,     -28237 ,      16625 ,      -3642 ,      32565,
    -31391 ,       9398 ,     -11317 ,      30752 ,     -23276 ,      23065 ,       4486 ,      32459,
    -32556 ,      -3724 ,     -22513 ,      23810 ,     -30478 ,      12034 ,      -8659 ,      31603,
    -32471 ,       4404 ,     -15959 ,      28619 ,     -26578 ,      19166 ,       -614 ,      32762,
    -32766 ,        358 ,     -19373 ,      26428 ,     -28743 ,      15736 ,      -4657 ,      32435,
    -31670 ,       8413 ,     -12271 ,      30383 ,     -23985 ,      22326 ,       3470 ,      32584,
    -32725 ,      -1684 ,     -20982 ,      25169 ,     -29667 ,      13914 ,      -6669 ,      32082,
    -32132 ,       6423 ,     -14141 ,      29560 ,     -25330 ,      20788 ,       1433 ,      32737,
    -32680 ,       2404 ,     -17685 ,      27586 ,     -27704 ,      17500 ,      -2623 ,      32663,
    -31083 ,      10374 ,     -10350 ,      31090 ,     -22544 ,      23781 ,       5498 ,      32303};



  // int sign_I,sign_Q,X_step,Y_step,abs_I,abs_Q, rotation_index, flag;
  ap_uint<1> sign_I,sign_Q,flag;
  ap_int<17> X_step,Y_step;
  ap_int<16> abs_I,abs_Q;
  ap_uint<9> rotation_index;
  ap_int<17> next_X_step,next_Y_step ;

  sign_I = ( x_I >> 16 ) & 0x1 ;
  sign_Q = ( x_Q >> 16 ) & 0x1 ;
  if (sign_I > 0 ) {
    abs_I = REG(-x_I) ;
  } else {
    abs_I = REG(x_I) ;
  }
   
  if (sign_Q > 0 ) {
    abs_Q = REG(-x_Q) ;
  } else {
    abs_Q = REG(x_Q) ;
  }
  
  X_step = REG(abs_Q + abs_I) ;
  bool absCond = abs_I <= abs_Q;
  
  flag = REG((absCond) ? 1 : 0);
  
  // if its in second octant
  Y_step = (absCond) ? REG(abs_I - abs_Q) : REG(abs_Q - abs_I) ;
  
  rotation_index = 1;
  for(int i = 1;i<nsteps;i++) {
    bool YCond = (Y_step >= 0);
    rotation_index = YCond ? REG((ap_uint<9>)(rotation_index + (1<<i))) :
      rotation_index;
    ap_int<17> Y_step_shift = (Y_step>>i);
    ap_int<17> X_step_shift = (X_step>>i);
    next_X_step = X_step + REG(((YCond) ? ( Y_step_shift) : (ap_int<17>)(-Y_step_shift)));
    next_Y_step = Y_step + REG(((YCond) ? (ap_int<17>)(-X_step_shift) : ( X_step_shift)));
    X_step = REG(next_X_step);
    Y_step = REG(next_Y_step);
  }

  ap_int<32> mag = X_step*P_fixed;
  *magout = (mag>>SHIFT_BITS);

  ap_int16 sinlut = rotation_sin_lut[rotation_index];
  ap_int16 sinlut_neg = REG(-sinlut);

  ap_int16 coslut = rotation_cos_lut[rotation_index];
  ap_int16 coslut_neg = REG(-coslut);

  *cos_fixed = REG((flag) ? ((sign_I) ? sinlut_neg : sinlut) :
		   ((sign_I) ? coslut_neg : coslut));

  *sin_fixed = REG((flag) ? ((sign_Q) ? coslut_neg : coslut) :
		   ((sign_Q) ? sinlut_neg : sinlut));


}

//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////SUB FUNCTIONS PCCFR ITERATION1///////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
void pccfr_pl_pds_ant1_itr2(bool process,
			    ap_int64 in_samp,
			    ap_int32 *out0,
			    ap_cint16 *out1
			    )
{
#pragma HLS PIPELINE II=1
#pragma HLS INTERFACE ap_ctrl_none port=return
  static ap_cint16 sf, peak_sample;
  static ap_uint32 max_val, max_loc;
  static ap_int8 phase;
  bool detect_flag = false;
  int center_position = (CP_NTAPS+1)/VEC_LEN/2;

  //storage for 3 magnitude value which should be stored for local maxima detetion and shifted for each new entry
  static ap_uint32  mag_vals[2];
  static ap_cint16 sample_vals[1];
  static ap_int8 phase_vals[1];
  static ap_cint16 s_v_in[5];
  static ap_cint16 sa_int[6];
  static ap_int<6> count_t = -4;
  static int count_global = -4 - 12;
  static ap_uint32  max_val_t = PCCFR_THRESHOLD*PCCFR_THRESHOLD;
  static ap_cint16 peak_sample_t = {0, 0};
  static ap_int32 max_loc_t = 0;
  static ap_int8 phase_t = 0;
  static bool detect_flag_t = false;
  static bool new_cpg_allocated = false;
  static CPG_data cpg[8];
#pragma HLS ARRAY_PARTITION variable=cpg complete dim=1

  ap_int32 val0;
  ap_int32 val1;
  ap_cint16 cval0;
  ap_cint16 cval1;

  val0 =        (ap_int32) ( in_samp & 0x00000000ffffffff );
  val1 =        (ap_int32) ( (in_samp>>32) & 0x00000000ffffffff );
  cval0.real  = (ap_int16) ( val0 & 0x0000ffff) ;
  cval0.imag =  (ap_int16) ( (val0>>16) & 0x0000ffff) ;
  cval1.real  = (ap_int16) ( val1 & 0x0000ffff) ;
  cval1.imag =  (ap_int16) ( (val1>>16) & 0x0000ffff) ;
  ap_cint16 in_pl0 ;
  ap_cint16 in_pl1 ;
  in_pl0.real  = cval0.real ;
  in_pl0.imag  = cval0.imag ;
  in_pl1.real  = cval1.real ;
  in_pl1.imag  = cval1.imag ;
  ap_cint16 in_val, out_val;
  ap_cint16 sample_val = sample_vals[0]; 
  ap_int8 phase_val = phase_vals[0];

  ap_int<6> tmp_count0 = cpg[0].count;
  ap_int<6> tmp_count1 = cpg[1].count;
  ap_int<6> tmp_count2 = cpg[2].count;
  ap_int<6> tmp_count3 = cpg[3].count;
  ap_int<6> tmp_count4 = cpg[4].count;
  ap_int<6> tmp_count5 = cpg[5].count;
  ap_int<6> tmp_count6 = cpg[6].count;
  ap_int<6> tmp_count7 = cpg[7].count;

  if(!process) {
    *out0 = 0;
    out1->real = 0;
    out1->imag = 0;
    return;
  }
  
  
  count_global+=2;
  int cpgload_index = count_global & 0xf;
  int current_index = (count_global >> 1) & 0x7;

  //  ***************************************************
  // changes for 64-bit IO
  //  ***************************************************
  s_v_in[4]=s_v_in[2];
  s_v_in[3]=s_v_in[1]; 
  s_v_in[2]=s_v_in[0]; 
  s_v_in[1]=in_pl0; 
  s_v_in[0]= in_pl1;

  //interpolate data
  sa_int[5].real =    s_v_in[3].real;
  sa_int[5].imag =    s_v_in[3].imag;
  sa_int[4].real =    ((s_v_in[4].real   + s_v_in[1].real) * -5961   + (s_v_in[3].real   +  s_v_in[2].real    ) * 20518)>>15;
  sa_int[4].imag =    ((s_v_in[4].imag   + s_v_in[1].imag) * -5961   + (s_v_in[3].imag   +  s_v_in[2].imag    ) * 20518)>>15;
  sa_int[3].real =    s_v_in[2].real;
  sa_int[3].imag =    s_v_in[2].imag;
  sa_int[2].real =    ((s_v_in[3].real   + s_v_in[0].real) * -5961   + (s_v_in[2].real   +  s_v_in[1].real    ) * 20518)>>15;
  sa_int[2].imag =    ((s_v_in[3].imag   + s_v_in[0].imag) * -5961   + (s_v_in[2].imag   +  s_v_in[1].imag    ) * 20518)>>15;
  sa_int[1].real =    s_v_in[1].real;
  sa_int[1].imag =    s_v_in[1].imag;
  
  ap_cint16 max_sample = {0, 0};
  ap_int8 max_phase = 0;
  ap_int32 max_mag = 0;
  
  //search max
  for(int i=5;i>=2;i--){
    ap_int32 mag = sa_int[i] .real * sa_int[i].real + sa_int[i].imag * sa_int[i].imag;
    bool maxCond = REG(max_mag < mag);
    max_mag = maxCond ? mag : max_mag;
    max_phase = maxCond?(ap_int8)(i==5?48:i==4?32:i==3?31:0):max_phase;//(i-2)*31;
    max_sample = maxCond? sa_int[i] : max_sample;
  }
  ap_int32 mval_left = mag_vals[1];
  ap_int32 mval_mid =  mag_vals[0];
  ap_int32 mval_right = REG(max_mag);
  
  mag_vals[1]     =   mag_vals[0];
  mag_vals[0]     =   max_mag;
  sample_vals[0]  =   max_sample; 
  phase_vals[0]   =   max_phase; 
  

  bool cond1 = REG((mval_mid>mval_left) && (mval_mid>=mval_right) && (mval_mid>max_val_t));
  bool cond2 = REG((count_t==(DET_WINDOW_ITR2-1)));
  bool cond3 = detect_flag_t;
  bool cond4 = REG(cond2 && cond3);
  bool cond5 = REG(((!cond1) && cond4));
  
  count_t = (cond1) || (cond2) ? (ap_int<6>)(0) : (ap_int<6>)(count_t+1);
  detect_flag_t = (cond1) ? true : ((cond2) ? false : detect_flag_t);
  max_loc = cond5 ? (ap_uint32)(max_loc_t) : max_loc;
  max_val = cond5 ? (ap_uint32)(max_val_t) : max_val;
  ap_cint16 peak_sample_t_old = peak_sample_t;
  peak_sample_t = (cond1) ? sample_val : peak_sample_t;
  peak_sample = cond5 ? peak_sample_t_old : peak_sample;
  ap_int8 phase_t_old = phase_t;
  phase_t = (cond1) ? phase_val : phase_t;
  phase = cond5 ? phase_t_old : phase;

  detect_flag = cond5 ?
    (max_val_t>(PCCFR_THRESHOLD*PCCFR_THRESHOLD+PCCFR_MAX_VAL_TOL)) :
    detect_flag;
  
  max_loc_t = (cond1) ?  ap_int32((count_global >> 1)%8) : max_loc_t;
  max_val_t = (cond1) ? (ap_uint32)(mval_mid) : ((cond2) ? (ap_uint32)(PCCFR_THRESHOLD*PCCFR_THRESHOLD) : max_val_t);

  // adding CORDIC here
  ap_int<16> magout;
  ap_int<16> cs_fixed_real,cs_fixed_imag;
  cos_sin_mag(peak_sample.real,peak_sample.imag,&magout,&cs_fixed_real,&cs_fixed_imag);
  ap_int<16> magout_reg = REG(magout);
  ap_int<16> cs_fixed_real_reg = REG(cs_fixed_real);
  ap_int<16> cs_fixed_imag_reg = REG(cs_fixed_imag);
  ap_int32  res_real = (ap_int32)cs_fixed_real_reg*(PCCFR_THRESHOLD - magout_reg);
  ap_int32  res_imag = (ap_int32)cs_fixed_imag_reg*(PCCFR_THRESHOLD - magout_reg);
  
  sf.real = (detect_flag) ? (ap_int<16>)(res_real>>15) : sf.real;
  sf.imag = (detect_flag) ? (ap_int<16>)(res_imag>>15) : sf.imag;
  new_cpg_allocated =  (detect_flag) ? true : new_cpg_allocated;

  
  ap_int<9> ci = 1 << current_index;
  ap_int<6> tmp_count = 0;
  tmp_count[0] = ((ci[0] & tmp_count0[0]) | (ci[1] & tmp_count1[0]) | (ci[2] & tmp_count2[0]) | (ci[3] & tmp_count3[0]) |
		  (ci[4] & tmp_count4[0]) | (ci[5] & tmp_count5[0]) | (ci[6] & tmp_count6[0]) | (ci[7] & tmp_count7[0]));
  tmp_count[1] = ((ci[0] & tmp_count0[1]) | (ci[1] & tmp_count1[1]) | (ci[2] & tmp_count2[1]) | (ci[3] & tmp_count3[1]) |
		  (ci[4] & tmp_count4[1]) | (ci[5] & tmp_count5[1]) | (ci[6] & tmp_count6[1]) | (ci[7] & tmp_count7[1]));
  tmp_count[2] = ((ci[0] & tmp_count0[2]) | (ci[1] & tmp_count1[2]) | (ci[2] & tmp_count2[2]) | (ci[3] & tmp_count3[2]) |
		  (ci[4] & tmp_count4[2]) | (ci[5] & tmp_count5[2]) | (ci[6] & tmp_count6[2]) | (ci[7] & tmp_count7[2]));
  tmp_count[3] = ((ci[0] & tmp_count0[3]) | (ci[1] & tmp_count1[3]) | (ci[2] & tmp_count2[3]) | (ci[3] & tmp_count3[3]) |
		  (ci[4] & tmp_count4[3]) | (ci[5] & tmp_count5[3]) | (ci[6] & tmp_count6[3]) | (ci[7] & tmp_count7[3]));
  tmp_count[4] = ((ci[0] & tmp_count0[4]) | (ci[1] & tmp_count1[4]) | (ci[2] & tmp_count2[4]) | (ci[3] & tmp_count3[4]) |
		  (ci[4] & tmp_count4[4]) | (ci[5] & tmp_count5[4]) | (ci[6] & tmp_count6[4]) | (ci[7] & tmp_count7[4]));
  tmp_count[5] = ((ci[0] & tmp_count0[5]) | (ci[1] & tmp_count1[5]) | (ci[2] & tmp_count2[5]) | (ci[3] & tmp_count3[5]) |
		  (ci[4] & tmp_count4[5]) | (ci[5] & tmp_count5[5]) | (ci[6] & tmp_count6[5]) | (ci[7] & tmp_count7[5]));

  ap_int<6> tmp_count_tmp = tmp_count;
  tmp_count = (tmp_count != (CP_NTAPS+1)/VEC_LEN) ? (ap_int<6>)(tmp_count+1) : tmp_count;

  ap_int8 tmp_phase = cpg[current_index].phase;
  ap_int32  tmp_ss0;
  ap_int16 tmp_ss1_real = cpg[current_index].ss1.real;
  ap_int16 tmp_ss1_imag = cpg[current_index].ss1.imag;
  ap_uint<6> tmp_offset = cpg[current_index].offset;
  ap_int<6> tmp_count_CP_half = cpg[current_index].count_CP_half;


  ap_uint<4> tmp_offset_4 = (tmp_offset & 0xf);
  ap_uint<6> tmp_offset_15m4t = ((15 - tmp_offset & 0xf) | 0x10);
  ap_uint<6> tmp_offset_16m4t = ((16 - tmp_offset & 0xf) | 0x10);
  
  ap_uint<1> lef_en = (int)((tmp_count_tmp+1) <= center_position);
  ap_int<2> mypos = lef_en ? 1 : -1;
  ap_int<6> tmp_count_CP_half_plus = (tmp_count_CP_half + mypos);

  bool countCond = tmp_count_tmp != (CP_NTAPS+1)/VEC_LEN;
  ap_int<6> newCount = tmp_count_tmp+1;


  bool halfCond = REG((!((tmp_phase == 32) || (tmp_phase == 0))) && (tmp_count_tmp != (CP_NTAPS+1)/VEC_LEN));
  tmp_count_CP_half =  halfCond ?  tmp_count_CP_half_plus : tmp_count_CP_half;
    
  ap_uint<6>  tmp_ss0_inter15 = REG(lef_en?(ap_uint<6>)tmp_offset_4:(ap_uint<6>)tmp_offset_15m4t);
  ap_uint<6>  tmp_ss0_inter16 = REG(lef_en?(ap_uint<6>)tmp_offset_4:(ap_uint<6>)tmp_offset_16m4t);

  tmp_ss0 = (countCond) ?
    ((tmp_phase == 32) ? (ap_int32)(tmp_offset_15m4t + ((-newCount + 30)  << 10)) :
     ((tmp_phase == 0)  ? (ap_int32)(tmp_offset_4 + ((ap_int32)(newCount  + tmp_phase)  << 10)) :
      ((tmp_phase == 48) ? (ap_int32)(tmp_ss0_inter15 + ((ap_int32)(tmp_count_CP_half_plus + tmp_phase)  << 10)) :
       (ap_int32)(tmp_ss0_inter16 + ((ap_int32)(tmp_count_CP_half_plus + tmp_phase)  << 10)))))
    : (ap_int32)(0);

  tmp_ss1_real = (countCond) ? tmp_ss1_real : (ap_int16)(0);
  tmp_ss1_imag = (countCond) ? tmp_ss1_imag : (ap_int16)(0);

  ap_uint<9> bitvec;
  bitvec[0] = (tmp_count0==(CP_NTAPS+1)/VEC_LEN) ? 1 : 0;
  bitvec[1] = (tmp_count1==(CP_NTAPS+1)/VEC_LEN) ? 1 : 0;
  bitvec[2] = (tmp_count2==(CP_NTAPS+1)/VEC_LEN) ? 1 : 0;
  bitvec[3] = (tmp_count3==(CP_NTAPS+1)/VEC_LEN) ? 1 : 0;
  bitvec[4] = (tmp_count4==(CP_NTAPS+1)/VEC_LEN) ? 1 : 0;
  bitvec[5] = (tmp_count5==(CP_NTAPS+1)/VEC_LEN) ? 1 : 0;
  bitvec[6] = (tmp_count6==(CP_NTAPS+1)/VEC_LEN) ? 1 : 0;
  bitvec[7] = (tmp_count7==(CP_NTAPS+1)/VEC_LEN) ? 1 : 0;
  bitvec[8] = 1;

  ap_uint<9> plusOne = (-bitvec);
  ap_uint<9> out = bitvec & plusOne;
  ap_uint<4> bnl =0;

  bnl[0] = out[1] | out[3] | out[5] | out[7];
  bnl[1] = out[2] | out[3] | out[6] | out[7];
  bnl[2] = out[4] | out[5] | out[6] | out[7];
  bnl[3] = out[8];

  bool updateCond = (new_cpg_allocated) && (cpgload_index == 14) && (bnl[3] == 0);

  ap_int<5> updateIndex = (updateCond) ? (ap_int<5>)bnl : (ap_int<5>)current_index;

  cpg[updateIndex].ss0 = (updateCond)? (ap_int32)(8 - max_loc) : tmp_ss0;
  cpg[updateIndex].ss1.real = (updateCond)? sf.real : tmp_ss1_real;
  cpg[updateIndex].ss1.imag = (updateCond)? sf.imag : tmp_ss1_imag;
  cpg[updateIndex].count_CP_half = (updateCond)? (ap_int<6>)-1 : (ap_int<6>)tmp_count_CP_half;
  cpg[updateIndex].count = (updateCond)? (ap_int<6>)-1 : (ap_int<6>)tmp_count;
  cpg[updateIndex].offset = (updateCond)? (ap_uint<6>)(8 - max_loc) : tmp_offset;
  cpg[updateIndex].phase = (updateCond)? phase : tmp_phase;

  new_cpg_allocated = (updateCond) ? false : new_cpg_allocated;

    
  if((count_global >= 0)){
    *out0 = tmp_ss0;
    out1->real = tmp_ss1_real;
    out1->imag = tmp_ss1_imag;
  }
  else if(count_global < 0 && count_global > -9){
    *out0 = tmp_ss0;
    out1->real = tmp_ss1_real;
    out1->imag = tmp_ss1_imag;
  }
  else {
    *out0 = 0;
    out1->real = 0;
    out1->imag = 0;
  }

    
};

//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////TOP LEVEL FUNCTION///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
void pccfr_pl_pds_itr2(
			input_stream_int64 &in_sample1,
			output_stream_int64 & out0_itr1
			)
{
#pragma HLS PIPELINE II=1
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis register both port=out0_itr1
#pragma HLS INTERFACE axis register both port=in_sample1
  ap_int64 in_samp0 ; // Iteration-1: 2 complex samples concatenated to 64-bit
  ap_int64 out0 ;     // Look up table value stream LSB is Iter-1, MSBs are Iter-2
  static ap_int64 in_s0[4] ;
  static ap_int64 in_s1[4] ;

  static ap_uint<10> counter_to_avoid_stall_for_feedback_path = 0;
#pragma HLS RESET variable=counter_to_avoid_stall_for_feedback_path
  
  // Pumping out zeros till PCCFR_INPUT_SAMPLES*6 
  // every LTERNET CLOCK
  if (counter_to_avoid_stall_for_feedback_path < (PCCFR_INPUT_SAMPLES+1+4))
    counter_to_avoid_stall_for_feedback_path++;
  bool processCond = counter_to_avoid_stall_for_feedback_path > (PCCFR_INPUT_SAMPLES+1+3);
  bool sampleCond = counter_to_avoid_stall_for_feedback_path >= (PCCFR_INPUT_SAMPLES+1);
  in_samp0 = sampleCond ? in_sample1.read() : 0;

  ap_int32  out0_itr1_l =0 ;
  ap_cint16 out1_itr1_l;
  out1_itr1_l.real = 0;
  out1_itr1_l.imag = 0;

  pccfr_pl_pds_ant1_itr2(processCond, in_s0[3], &out0_itr1_l, &out1_itr1_l);

  out0.range(31, 0) = out0_itr1_l;
  out0.range(47,32) = out1_itr1_l.real;
  out0.range(63,48) = out1_itr1_l.imag;

  out0_itr1.write(out0);

  in_s0[3]=in_s0[2];
  in_s0[2]=in_s0[1];
  in_s0[1]=in_s0[0];
  in_s0[0]=in_samp0;

};


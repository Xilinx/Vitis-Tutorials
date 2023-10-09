/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


//#include "adf/wrapper/wrapper.h"
//#include<iostream> 
#include "adf.h"
#include "../include.h"
#define CFR_THRESHOLD (11626)
#define SIGN(x) (x>0 ? 0.5 : -0.5)


// Mag & cos, sin calc function -CORDIC
void cos_sin_mag (
 int  x_I,
 int  x_Q,
 int*  magout,
 int*  cos_fixed,
 int*  sin_fixed)
{
   const int P_SCALE = 512 ;
   const int C_SCALE = 1 ;
   const int SHIFT_BITS = 9;
   const int P_fixed = 311 ;
   const int nsteps = 6 ;

   const int rotation_cos_lut[64]={-4613,32442,23186,23155,11197,30796,31355,9520,3515,32579,28172,16736,18433,27092,32733,1509,-538,32764,25888,20088,14944,29162,32296,5542,7544,31888,30037,13098,21662,24586,32666,-2578,-2578,32666,24586,21662,13098,30037,31888,7544,5542,32296,29162,14944,20088,25888,32764,-538,1509,32733,27092,18433,16736,28172,32579,3515,9520,31355,30796,11197,23155,23186,32442,-4613} ;

   const int rotation_sin_lut[64]={-32442,-4613,-23155,23186,-30796,11197,-9520,31355,-32579,3515,-16736,28172,-27092,18433,-1509,32733,-32764,-538,-20088,25888,-29162,14944,-5542,32296,-31888,7544,-13098,30037,-24586,21662,2578,32666,-32666,-2578,-21662,24586,-30037,13098,-7544,31888,-32296,5542,-14944,29162,-25888,20088,538,32764,-32733,1509,-18433,27092,-28172,16736,-3515,32579,-31355,9520,-11197,30796,-23186,23155,4613,32442} ;
   
   int sign_I,sign_Q,X_step,Y_step,abs_I,abs_Q, rotation_index, flag;
   int next_X_step,next_Y_step ;

   sign_I = ( x_I >> 16 ) & 0x1 ;
   sign_Q = ( x_Q >> 16 ) & 0x1 ;
   if (sign_I > 0 ) {
      abs_I = ~(x_I)+1 ;
   } else {
       abs_I = x_I ;
   }
   
   if (sign_Q > 0 ) {
      abs_Q = ~(x_Q)+1 ;
   } else {
       abs_Q = x_Q ;
   }
   // Invert the signs now, positive =1, needed for later calc ??
   //sign_I = 1-sign_I;
   //sign_Q = 1-sign_Q;
   X_step = abs_Q + abs_I ;
   flag = 0 ;
   // if its in second octant
   if ( abs_I <= abs_Q ) {
      Y_step = abs_I - abs_Q ;
      flag=1;
   } else {
      Y_step = abs_Q - abs_I ;
   }

   rotation_index = 1;
   for(int i = 1;i<nsteps;i++) {
       if (Y_step >= 0) {
          rotation_index = rotation_index + (1<<i);
          next_X_step = X_step + (Y_step>>i);
          next_Y_step = Y_step - (X_step>>i);
       } else {
          next_X_step = X_step - (Y_step>>i) ;
          next_Y_step = Y_step + (X_step>>i) ;
       }
       X_step = next_X_step;
       Y_step = next_Y_step;
   }


   //mag = floor(X_step*P_fixed/norm);
   int mag = X_step*P_fixed;
   *magout = mag>>SHIFT_BITS;

 if (flag) {
       if (sign_I) {
         *cos_fixed = -rotation_sin_lut[rotation_index];
       } else {
         *cos_fixed = rotation_sin_lut[rotation_index];
       }
       if (sign_Q) {
         *sin_fixed = -rotation_cos_lut[rotation_index];
       } else {
         *sin_fixed = rotation_cos_lut[rotation_index];
       }
   } else {
       if (sign_I) {
         *cos_fixed = -rotation_cos_lut[rotation_index];
       } else {
         *cos_fixed = rotation_cos_lut[rotation_index];
       }
       if (sign_Q) {
         *sin_fixed = -rotation_sin_lut[rotation_index];
       } else {
         *sin_fixed = rotation_sin_lut[rotation_index];
       }
   }

}

void polar_clip(input_stream_cint16* in, output_stream_cint16 * out) {
  cint16 sample;
  cint16 ovalue;
  int16_t value_real, value_imag;
  int32_t value_real_1, value_imag_1;
  int32_t value_real_sq, value_imag_sq, mag_sq;
  int32_t res_real, res_imag;

  int32_t magout,cs_fixed_real,cs_fixed_imag;

  int loopct = 0;
  for (int i =0;i<POLAR_CLIP_INPUT_SAMPLES;i++){
    sample = readincr(in);
    value_real = sample.real;
    value_imag = sample.imag;

    value_real_1 = (int) sample.real;
    value_imag_1 = (int) sample.imag;

    value_real_sq = value_real * value_real;
    value_imag_sq = value_imag * value_imag;
    mag_sq = value_real_sq + value_imag_sq;

    cos_sin_mag(value_real_1,value_imag_1,&magout,&cs_fixed_real,&cs_fixed_imag);

    if(mag_sq>CFR_THRESHOLD*CFR_THRESHOLD){
   
      res_real = (int32_t)cs_fixed_real*(magout-CFR_THRESHOLD);
      res_imag = (int32_t)cs_fixed_imag*(magout-CFR_THRESHOLD);
    
      ovalue.real = (short) (res_real & 0xFFFF);
      ovalue.imag = (short) (res_imag & 0xFFFF);
   
    }else {
        ovalue.real = 0 ;
        ovalue.imag = 0 ;      
    }	  
    writeincr(out,ovalue);
  }
  
}

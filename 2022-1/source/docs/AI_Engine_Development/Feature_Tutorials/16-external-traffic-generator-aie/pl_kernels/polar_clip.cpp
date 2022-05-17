/**********
© Copyright 2022 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/



#include "pl_kernels.h"

#define CFR_THRESHOLD (11626)
#define SIGN(x) (x>0 ? 0.5 : -0.5)

#include <stdint.h>

template <typename ELEMENT> struct complex_types {
  ELEMENT real;
  ELEMENT imag;
};

// Bit accurate data types used for HLS synthesis
typedef complex_types<int16_t> ap_cint16;
typedef int     ap_int32;
typedef int16_t ap_int16;
typedef long    ap_int64;


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


inline void process_polar_clip(int &in_sample, int &out_sample) {

  short value_real, value_imag;
  short value_real_1, value_imag_1;

  value_real = short(in_sample & 0xFFFF);
  value_imag = short((in_sample >> 16) & 0xFFFF);

  value_real_1 = short(in_sample & 0xFFFF);
  value_imag_1 = short((in_sample >> 16) & 0xFFFF);

  ap_cint16 ovalue;

  ap_int32 value_real_sq, value_imag_sq, mag_sq;
  ap_int32 res_real, res_imag;

  value_real_sq = value_real * value_real;
  value_imag_sq = value_imag * value_imag;
  mag_sq = value_real_sq + value_imag_sq;

  ap_int32 magout,cs_fixed_real,cs_fixed_imag;
  cos_sin_mag(value_real_1,value_imag_1,&magout,&cs_fixed_real,&cs_fixed_imag);

 if(mag_sq>CFR_THRESHOLD*CFR_THRESHOLD){

     res_real = (ap_int32)cs_fixed_real*(magout-CFR_THRESHOLD);
     res_imag = (ap_int32)cs_fixed_imag*(magout-CFR_THRESHOLD);

     ovalue.real = (short) (res_real & 0xFFFF);
     ovalue.imag = (short) (res_imag & 0xFFFF);

  }
  else {
     ovalue.real = 0 ;
     ovalue.imag = 0 ;
  }


  out_sample = ( (( ((short)ovalue.real) & 0x0000FFFF) << 0) | (( ((short)ovalue.imag) & 0x0000FFFF) << 16) );
}



void polar_clip(hls::stream<pliopkt>& sin,hls::stream<pliopkt>& sout) {

  pliopkt sample;
  pliopkt ovalue;
  int insample,outsample;

  int loopct = 0;
  for (int i =0;i<POLAR_CLIP_INPUT_SAMPLES;i++){
    sin >> sample;

    insample = sample.data;
    process_polar_clip(insample,outsample);
    ovalue.data = outsample;
    ovalue.last = sample.last;

    sout << ovalue;
  }

}

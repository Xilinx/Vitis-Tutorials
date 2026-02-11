//#include "adf/wrapper/wrapper.h"
#include<iostream>

#define CFR_THRESHOLD (11626)
#define SIGN(x) (x>0 ? 0.5 : -0.5)

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <stdint.h>

template <typename ELEMENT> struct complex_types {
  ELEMENT real;
  ELEMENT imag;
};

// Bit accurate data types used for HLS synthesis
typedef complex_types<ap_int<16> > ap_cint16;
typedef ap_int<32>                 ap_int32;
typedef ap_int<16>                 ap_int16;
typedef ap_int<64>                 ap_int64;

// Mag & cos, sin calc function -CORDIC
void cos_sin_mag (
 ap_int32  x_I,
 ap_int32  x_Q,
 ap_int32*  magout,
 ap_int32*  cos_fixed,
 ap_int32*  sin_fixed)
{
   const ap_int32 P_SCALE = 512 ;
   const ap_int32 C_SCALE = 1 ;
   const ap_int32 SHIFT_BITS = 9;
   const ap_int32 P_fixed = 311 ;
   const ap_int32 nsteps = 6 ;

   const ap_int32 rotation_cos_lut[64]={-4613,32442,23186,23155,11197,30796,31355,9520,3515,32579,28172,16736,18433,27092,32733,1509,-538,32764,25888,20088,14944,29162,32296,5542,7544,31888,30037,13098,21662,24586,32666,-2578,-2578,32666,24586,21662,13098,30037,31888,7544,5542,32296,29162,14944,20088,25888,32764,-538,1509,32733,27092,18433,16736,28172,32579,3515,9520,31355,30796,11197,23155,23186,32442,-4613} ;

   const ap_int32 rotation_sin_lut[64]={-32442,-4613,-23155,23186,-30796,11197,-9520,31355,-32579,3515,-16736,28172,-27092,18433,-1509,32733,-32764,-538,-20088,25888,-29162,14944,-5542,32296,-31888,7544,-13098,30037,-24586,21662,2578,32666,-32666,-2578,-21662,24586,-30037,13098,-7544,31888,-32296,5542,-14944,29162,-25888,20088,538,32764,-32733,1509,-18433,27092,-28172,16736,-3515,32579,-31355,9520,-11197,30796,-23186,23155,4613,32442} ;

   ap_int32 sign_I,sign_Q,X_step,Y_step,abs_I,abs_Q, rotation_index, flag;
   ap_int32 next_X_step,next_Y_step ;

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
   for(ap_int32 i = 1;i<nsteps;i++) {
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
   ap_int32 mag = X_step*P_fixed;
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




extern "C" {
void polar_clip(hls::stream<ap_axis<32, 0, 0, 0> > &input, hls::stream<ap_axis<32, 0, 0, 0> > &output, int size) {
//#pragma HLS PIPELINE II=1
#pragma HLS INTERFACE ap_ctrl_hs port=return
#pragma HLS interface s_axilite port=return bundle=control
#pragma HLS interface s_axilite port=size bundle=control
#pragma HLS INTERFACE axis port=output
#pragma HLS INTERFACE axis port=input

for (int i=0; i<size; i++)
{
  //std::cerr << "Waiting for a value" << "\n";
  ap_cint16 sample;

  ap_axis<32, 0, 0, 0> out_x;
  ap_axis<32, 0, 0, 0> in_x = input.read();

  //sample = readincr(in);
  //sample = in.read();
  short value_real, value_imag;
  short value_real_1, value_imag_1;

  value_real = short(in_x.data & 0xFFFF); //sample.real;
  value_imag = short((in_x.data >> 16) & 0xFFFF); // sample.imag;

 // printf( "-[KERNEL]: Inside the kernel input  real and imag respectively  %d %d \n",(int) value_real, (int)value_imag);


  value_real_1 = short(in_x.data & 0xFFFF); //(ap_int32) sample.real;
  value_imag_1 = short((in_x.data >> 16) & 0xFFFF); // (ap_int32) sample.imag;

 // printf( "-[KERNEL]: Inside the kernel input  real and imag respectively  %d %d \n",(int) value_real_1, (int)value_imag_1);

  ap_cint16 ovalue;

  //std::cerr << "Received " << value.real << " + " << value.imag  << "j\n";
  //double ph = atan2((double)value.imag,(double)value.real) ;
  ap_int32 value_real_sq, value_imag_sq, mag_sq;
  ap_int32 res_real, res_imag;

  value_real_sq = value_real * value_real;
  value_imag_sq = value_imag * value_imag;
  mag_sq = value_real_sq + value_imag_sq;

  //double mag = sqrt(double(mag_sq)) ;

  ap_int32 magout,cs_fixed_real,cs_fixed_imag;

  cos_sin_mag(value_real_1,value_imag_1,&magout,&cs_fixed_real,&cs_fixed_imag);

 // printf("[KERNEL]: real and imaginary After CORDIC %d  %d \n",(int)cs_fixed_real ,(int) cs_fixed_imag );

  if(mag_sq>CFR_THRESHOLD*CFR_THRESHOLD){

     res_real = (ap_int32)cs_fixed_real*(magout-CFR_THRESHOLD);
     res_imag = (ap_int32)cs_fixed_imag*(magout-CFR_THRESHOLD);
    // printf("[KERNEL]: real and imaginary after threshold %d  %d \n",(int)res_real ,(int) res_imag );

	 ovalue.real = (short) (res_real & 0xFFFF);
     ovalue.imag = (short) (res_imag & 0xFFFF);
    // printf("[KERNEL]: real and imaginary after threshold %d  %d \n",(int)ovalue.real ,(int) ovalue.imag );

  }else {
     ovalue.real = 0 ;
     ovalue.imag = 0 ;
  }


  out_x.data = ( (( ((short)ovalue.real) & 0x0000FFFF) << 0) | (( ((short)ovalue.imag) & 0x0000FFFF) << 16) );

//  printf("[KERNEL}: real and imaginary at Output %d  %d \n",(int)ovalue.real ,(int) ovalue.imag );

  //****out_x.keep_all();
  output.write(out_x);
}
};

}

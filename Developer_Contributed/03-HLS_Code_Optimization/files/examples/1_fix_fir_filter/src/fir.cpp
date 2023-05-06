/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

Author: Daniele Bagni, Xilinx Inc
*/

#include "fir.h"

out_data_t fir_filter (inp_data_t x,   coef_t c[N])
{

  static inp_data_t shift_reg[N];

  acc_t acc = 0;
  acc_t mult;
  out_data_t y;



  Shift_Accum_Loop: for (int i=N-1;i>=0;i--)
  {
#pragma HLS LOOP_TRIPCOUNT min=1 max=16 avg=8


     if (i==0)
     {
       //acc+=x*c[0];
       shift_reg[0]=x;
     }
     else
     {
       shift_reg[i]=shift_reg[i-1];
       //acc+=shift_reg[i]*c[i];
     }
     //acc = acc + shift_reg[i]*c[i];
     mult = shift_reg[i]*c[i];
     acc  = acc + mult;

  }

  y = (out_data_t) acc;

  return y;

}

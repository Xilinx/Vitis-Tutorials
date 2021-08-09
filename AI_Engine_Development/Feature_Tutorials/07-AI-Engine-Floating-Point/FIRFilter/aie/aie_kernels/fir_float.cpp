/*
 * (c) Copyright 2020 Xilinx, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <adf.h>
#include "system_settings.h"

// Real floating point operations are on 8 lanes in parallel
#define VECTOR_LEN 8

alignas(32) float coeff_float[COEFF_NUM] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};


void firfloat(input_window_float *datain, output_window_float *dataout)
{
   unsigned int xoffsets = 0x76543210; // Data Lane offset
   unsigned int zoffsets  = 0x00000000; // Process the same coefficient on all lanes

   v16float sbuff = undef_v16float();
   v8float *coeff_ = (v8float *)coeff_float;

   v8float coe;
   v8float acc0;
   v8float acc1;
   v8float acc2;

   const unsigned LSIZE = (NSAMPLES_WINDOW / VECTOR_LEN);
   sbuff = upd_w(sbuff,0, window_readincr_v8(datain));

   for(unsigned i=0; i<LSIZE; i++)
      chess_prepare_for_pipelining
         chess_loop_range(8,)
         {
           sbuff = upd_w(sbuff,1, window_readincr_v8(datain));
            coe = *coeff_++;

            acc0 = fpmul(       sbuff, 1,xoffsets, coe, 0, zoffsets);
            acc1 = fpmul(       sbuff, 2,xoffsets, coe, 1, zoffsets);
            acc0 = fpmac( acc0, sbuff, 3,xoffsets, coe, 2, zoffsets);
            acc1 = fpmac( acc1, sbuff, 4,xoffsets, coe, 3, zoffsets);
            acc0 = fpmac( acc0, sbuff, 5,xoffsets, coe, 4, zoffsets);
            acc1 = fpmac( acc1, sbuff, 6,xoffsets, coe, 5, zoffsets);
            acc0 = fpmac( acc0, sbuff, 7,xoffsets, coe, 6, zoffsets);
            acc1 = fpmac( acc1, sbuff, 8,xoffsets, coe, 7, zoffsets);

            sbuff = upd_w(sbuff,0 ,window_readdecr_v8(datain));

            coe = *coeff_--;
            acc0 = fpmac( acc0, sbuff, 9, xoffsets, coe, 0, zoffsets);
            acc1 = fpmac( acc1, sbuff, 10, xoffsets, coe, 1, zoffsets);
            acc0 = fpmac( acc0, sbuff, 11,xoffsets, coe, 2, zoffsets);
            acc1 = fpmac( acc1, sbuff, 12,xoffsets, coe, 3, zoffsets);
            acc0 = fpmac( acc0, sbuff, 13,xoffsets, coe, 4, zoffsets);
            acc1 = fpmac( acc1, sbuff, 14,xoffsets, coe, 5, zoffsets);
            acc0 = fpmac( acc0, sbuff, 15,xoffsets, coe, 6, zoffsets);
            acc1 = fpmac( acc1, sbuff, 0,xoffsets, coe, 7, zoffsets);

            sbuff = upd_w(sbuff,0, window_readincr_v8(datain));

            acc2 = fpadd( acc1, xset_w(0,acc0), 0, xoffsets);
            window_writeincr(dataout,acc2);
         }

}

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

cfloat coeff_cfloat[COEFF_NUM] = {
    {1,16},{2,15},{3,14},{4,13},{5,12},{6,11},{7,10},{8,9},
    {9,8},{10,7},{11,6},{12,5},{13,4},{14,3},{15,2},{16,1}};

    // Complex floating point operations are on 4 lanes in parallel
    #define VECTOR_LEN 4


    void fircfloat(input_window_cfloat *datain, output_window_cfloat *dataout)
    {
       unsigned int xoffsets = 0x3210; // Data Lane offset
       unsigned int zoffsets  = 0x0000; // Process the same coefficient on all lanes

       v8cfloat sbuff = undef_v8cfloat();
       v4cfloat *coeff_ = (v4cfloat *)coeff_cfloat;

       v4cfloat coe;
       v4cfloat acc0;
       v4cfloat acc1;
       v4cfloat acc2;

       const unsigned LSIZE = (NSAMPLES_WINDOW / VECTOR_LEN);
       sbuff = upd_w(sbuff,0, window_readincr_v4(datain));
       sbuff = upd_w(sbuff,1, window_readincr_v4(datain));

       for(unsigned i=0; i<LSIZE; i++)
          chess_prepare_for_pipelining
             chess_loop_range(8,)
             {
                coe = *coeff_++;

                acc0 = fpmul(       sbuff, 1,xoffsets, coe, 0, zoffsets);
                acc1 = fpmul(       sbuff, 2,xoffsets, coe, 1, zoffsets);
                acc0 = fpmac( acc0, sbuff, 3,xoffsets, coe, 2, zoffsets);
                acc1 = fpmac( acc1, sbuff, 4,xoffsets, coe, 3, zoffsets);

                sbuff = upd_w(sbuff,0 ,window_readincr_v4(datain));
                coe = *coeff_++;

                acc0 = fpmac( acc0, sbuff, 5,xoffsets, coe, 0, zoffsets);
                acc1 = fpmac( acc1, sbuff, 6,xoffsets, coe, 1, zoffsets);
                acc0 = fpmac( acc0, sbuff, 7,xoffsets, coe, 2, zoffsets);
                acc1 = fpmac( acc1, sbuff, 0,xoffsets, coe, 3, zoffsets);

                sbuff = upd_w(sbuff,1 ,window_readincr_v4(datain));
                coe = *coeff_++;

                acc0 = fpmac( acc0, sbuff, 1, xoffsets, coe, 0, zoffsets);
                acc1 = fpmac( acc1, sbuff, 2, xoffsets, coe, 1, zoffsets);
                acc0 = fpmac( acc0, sbuff, 3,xoffsets, coe, 2, zoffsets);
                acc1 = fpmac( acc1, sbuff, 4,xoffsets, coe, 3, zoffsets);

                sbuff = upd_w(sbuff,0 ,window_read_v4(datain));
                coe = *coeff_;

                acc0 = fpmac( acc0, sbuff, 5,xoffsets, coe, 0, zoffsets);
                coeff_ -= 3;
                acc1 = fpmac( acc1, sbuff, 6,xoffsets, coe, 1, zoffsets);
                window_decr_v4(datain,3);
                acc0 = fpmac( acc0, sbuff, 7,xoffsets, coe, 2, zoffsets);
                acc1 = fpmac( acc1, sbuff, 0,xoffsets, coe, 3, zoffsets);

                sbuff = upd_w(sbuff,0, window_readincr_v4(datain));
                sbuff = upd_w(sbuff,1, window_readincr_v4(datain));

                acc2 = fpadd( acc1, xset_w(0,acc0), 0, xoffsets);
                window_writeincr(dataout,acc2);
             }

    }

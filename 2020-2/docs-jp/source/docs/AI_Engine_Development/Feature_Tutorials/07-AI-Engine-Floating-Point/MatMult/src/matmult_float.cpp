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


// 16x8  x  8x8
// Can be extended to (2L)x(8M)  (8M)x(8N)
void matmult_float(
    input_window_float* restrict matA,
    input_window_float* restrict matB,
    output_window_float* restrict matC)
{

   v16float buf_matA = window_readincr_v16(matA);      // holds the first 16 elements of matA
   v8float buf_matB = undef_v8float();  // Holds 8 elements of matB
   v8float acc1 = undef_v8float();   // 8 accumulation values on even rows
   v8float acc2 = undef_v8float();   // 8 accumulation values on odd rows

   #define FPMUL   \
   buf_matB = window_readincr_v8(matB); \
   acc1 = fpmul(buf_matA,0,0x00000000,buf_matB,0,0x76543210);\
   acc2 = fpmul(buf_matA,8,0x00000000,buf_matB,0,0x76543210)

   #define FPMAC(Start1, Start2)\
   buf_matB = window_readincr_v8(matB);\
   acc1 = fpmac(acc1,buf_matA,Start1,0x00000000,buf_matB,0,0x76543210);\
   acc2 = fpmac(acc2,buf_matA,Start2,0x00000000,buf_matB,0,0x76543210)


   for (unsigned int i=0;i<F_Ra/2;i++)  // Each iteration computes 2 rows of C
   chess_prepare_for_pipelining
   chess_loop_range(8,)
   {
     FPMUL;
     FPMAC(1,9);
     FPMAC(2,10);
     FPMAC(3,11);
     FPMAC(4,12);
     FPMAC(5,13);
     FPMAC(6,14);
     FPMAC(7,15);

     buf_matA = window_readincr_v16(matA); // reads the next 2 rows of A
     window_decr_v8(matB,8); // reset B pointer
     window_writeincr(matC,acc1); // Writes 1 row of C
     window_writeincr(matC,acc2); // Writes the next row of C
   }
}

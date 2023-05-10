/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "aie_kernels.h"


// 16x8  x  8x8
// Can be extended to (2L)x(8M)  (8M)x(8N)
void matmult_float(
    input_buffer<float,adf::extents<NSAMPLES_WINDOW_F_A>> &  __restrict matA,
    input_buffer<float,adf::extents<NSAMPLES_WINDOW_F_B>> &  __restrict matB,
    output_buffer<float,adf::extents<NSAMPLES_WINDOW_F_C>> &  __restrict matC)

{
    auto pA = begin_vector_random_circular<16>(matA);
    auto pB = begin_vector_random_circular<8>(matB);
    auto pC = begin_vector_random_circular<8>(matC);

   vector<float,16> buf_matA = *pA++;      // holds the first 16 elements of matA
   vector<float,8> buf_matB ;  // Holds 8 elements of matB
   vector<float,8> acc1;   // 8 accumulation values on even rows
   vector<float,8> acc2;   // 8 accumulation values on odd rows

   #define FPMUL   \
   buf_matB = *pB++; \
   acc1 = fpmul(buf_matA,0,0x00000000,buf_matB,0,0x76543210);\
   acc2 = fpmul(buf_matA,8,0x00000000,buf_matB,0,0x76543210)

   #define FPMAC(Start1, Start2)\
   buf_matB = *pB++;\
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

     buf_matA = *pA++; // reads the next 2 rows of A
     pB -= 8; // reset B pointer
     *pC++ = acc1;
     *pC++ = acc2;
   }
}

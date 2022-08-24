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


// 16x4  x  4x4
// Can be extended to (2L)x(4M)  (4M)x(4N)
void matmult_cfloat(
    input_window_cfloat* restrict matA,
    input_window_cfloat* restrict matB,
    output_window_cfloat* restrict matC)
{

   v8cfloat buf_matA = window_readincr_v8(matA);      // holds the first 8 elements of matA
   v4cfloat buf_matB = undef_v4cfloat();  // Holds 4 elements of matB
   v4cfloat acc1 = undef_v4cfloat();   // 4 accumulation values on even rows
   v4cfloat acc2 = undef_v4cfloat();   // 4 accumulation values on odd rows

   #define FPMUL   \
   buf_matB = window_readincr_v4(matB); \
   acc1 = fpmul(buf_matA,0,0x0000,buf_matB,0,0x3210);\
   acc2 = fpmul(buf_matA,4,0x0000,buf_matB,0,0x3210)

   #define FPMAC(Start1, Start2)\
   buf_matB = window_readincr_v4(matB);\
   acc1 = fpmac(acc1,buf_matA,Start1,0x0000,buf_matB,0,0x3210);\
   acc2 = fpmac(acc2,buf_matA,Start2,0x0000,buf_matB,0,0x3210)


   for (unsigned int i=0;i<CF_Ra/2;i++)  // Each iteration computes 2 rows of C
   chess_prepare_for_pipelining
   chess_loop_range(8,)
   {
     FPMUL;
     FPMAC(1,5);
     FPMAC(2,6);
     FPMAC(3,7);

     buf_matA = window_readincr_v8(matA); // reads the next 2 rows of A
     window_decr_v4(matB,4); // reset B pointer
     window_writeincr(matC,acc1); // Writes 1 row of C
     window_writeincr(matC,acc2); // Writes the next row of C
   }

   #undef FPMUL
   #undef FPMAC
}


void matmult_cfloat_conf(
    input_window_cfloat* restrict matA,
    input_window_cfloat* restrict matB,
    output_window_cfloat* restrict matC)
{

   v16float buf_matA = as_v16float(window_readincr_v8(matA));      // holds the first 8 elements of matA
   v8float buf_matB = undef_v8float();  // Holds 4 elements of matB
   v8float acc1 = undef_v8float();   // 4 complex accumulation values on even rows
   v8float acc2 = undef_v8float();   // 4 complex accumulation values on odd rows

   const bool flag_ones = false;
   const bool flag_abs = false;
   const unsigned int addmask = 0x0000;
   const unsigned int cmpmode = fpcmp_nrm;

   /* Complex Multiplication
    a = ar + i.ai
    b = br + i.bi

    c = a.b = (ar.br - ai.bi) + i (ar.bi + ai.br)
    Compute in 2 stages
    Stage 1
    cr = ar.br
    ci = ar.bi
    Stage 2
    cr = cr - ai.bi
    ci = ci + ai.br

    Let's use the fpmul_conf/fpmac_conf functions
    v8float fpmac_conf(v8float acc, v32float xbuf, int xstart, unsigned int xoffs, v8float zbuf, int zstart, unsigned int zoffs, bool ones, bool abs, unsigned int addmode, unsigned int addmask, unsigned int cmpmode)


    vector content
    A : a0r a0i a1r a1i a2r a2i a3r a3i a4r a4i a5r a5i a6r a6i a7r a7i
    B : b0r b0i b1r b1i b2r b2i b3r b3i

    Stage 1
    Extract a0r and the complete B vector to compute
    acc1 :  a0r.b0r a0r.b0i a0r.b1r a0r.b1i a0r.b2r a0r.b2i a0r.b3r a0r.b3i

    A selection:  buf_matA Start=0    Offset=0x00000000
    B selection:  buf_matB Start=0    Offset=0x76543210

    ones = false
    abs = false
    addmask = 0x00000000
    addmode = fpadd_add  --> only additions in this first stages
    cmpmode = fpcmp_nrm  --> no comparison before selection, just standard additions

    Stage 2
    Extract a0i and the complete B (with br bi order change) vector to compute
    acc1 :  acc1 +  -a0i.b0i a0i.b0r -a0i.b1i a0i.b1r -a0i.b2i a0i.b2r -a0i.b3i a0i.b3r

    A selection:  buf_matA Start=0    Offset=0x11111111
    B selection:  buf_matB Start=0    Offset=0x67452301

    ones = false
    abs = false
    addmask = 0x00000000
    addmode = fpadd_mixsub  --> a mix between adds and subs 0x55 = 0x01010101
                            --> Lane 0 (left handside of the vector) is the LSB (right handside bit of the integer)
    cmpmode = fpcmp_nrm  --> no comparison before selection, just standard additions
  */


   #define FPMUL   \
   buf_matB = as_v8float(window_readincr_v4(matB)); \
   acc1 = fpmul_conf(buf_matA,0,0x00000000,buf_matB,0,0x76543210,flag_ones,flag_abs,fpadd_add,addmask,cmpmode);\
   acc2 = fpmul_conf(buf_matA,8,0x00000000,buf_matB,0,0x76543210,flag_ones,flag_abs,fpadd_add,addmask,cmpmode);\
   acc1 = fpmac_conf(acc1,buf_matA,0,0x11111111,buf_matB,0,0x67452301,flag_ones,flag_abs,fpadd_mixsub,addmask,cmpmode);\
   acc2 = fpmac_conf(acc2,buf_matA,8,0x11111111,buf_matB,0,0x67452301,flag_ones,flag_abs,fpadd_mixsub,addmask,cmpmode)

   #define FPMAC(Start1, Start2)\
   buf_matB = as_v8float(window_readincr_v4(matB));\
   acc1 = fpmac_conf(acc1,buf_matA,Start1,0x00000000,buf_matB,0,0x76543210,flag_ones,flag_abs,fpadd_add,addmask,cmpmode);\
   acc2 = fpmac_conf(acc2,buf_matA,Start2,0x00000000,buf_matB,0,0x76543210,flag_ones,flag_abs,fpadd_add,addmask,cmpmode);\
   acc1 = fpmac_conf(acc1,buf_matA,Start1,0x11111111,buf_matB,0,0x67452301,flag_ones,flag_abs,fpadd_mixsub,addmask,cmpmode);\
   acc2 = fpmac_conf(acc2,buf_matA,Start2,0x11111111,buf_matB,0,0x67452301,flag_ones,flag_abs,fpadd_mixsub,addmask,cmpmode)


   for (unsigned int i=0;i<CF_Ra/2;i++)  // Each iteration computes 2 rows of C
   chess_prepare_for_pipelining
   chess_loop_range(8,)
   {
     FPMUL;
     FPMAC(2,10);
     FPMAC(4,12);
     FPMAC(6,14);

     buf_matA = as_v16float(window_readincr_v8(matA)); // reads the next 2 rows of A
     window_decr_v4(matB,4); // reset B pointer
     window_writeincr(matC,as_v4cfloat(acc1)); // Writes 1 row of C
     window_writeincr(matC,as_v4cfloat(acc2)); // Writes the next row of C
   }
   #undef FPMUL
   #undef FPMAC

}



void matmult_cfloat_conj(
    input_window_cfloat* restrict matA,
    input_window_cfloat* restrict matB,
    output_window_cfloat* restrict matC)
{

   v16float buf_matA = as_v16float(window_readincr_v8(matA));      // holds the first 8 elements of matA
   v8float buf_matB = undef_v8float();  // Holds 4 elements of matB
   v8float acc1 = undef_v8float();   // 4 complex accumulation values on even rows
   v8float acc2 = undef_v8float();   // 4 complex accumulation values on odd rows

   const bool flag_ones = false;
   const bool flag_abs = false;
   const unsigned int addmask = 0x0000;
   const unsigned int cmpmode = fpcmp_nrm;

   /* Complex Multiplication
    a = ar + i.ai
    b = br + i.bi

    c = a.conj(b) = (ar.br + ai.bi) + i (-ar.bi + ai.br)
    Compute in 2 stages
    Stage 1
    cr = ar.br
    ci = -ar.bi
    Stage 2
    cr = cr + ai.bi
    ci = ci + ai.br

    Compared to previous case the data extraction is the same but the subtract is not at the same place

    Let's use the fpmul_conf/fpmac_conf functions
    v8float fpmac_conf(v8float acc, v32float xbuf, int xstart, unsigned int xoffs, v8float zbuf, int zstart, unsigned int zoffs, bool ones, bool abs, unsigned int addmode, unsigned int addmask, unsigned int cmpmode)


    vector content
    A : a0r a0i a1r a1i a2r a2i a3r a3i a4r a4i a5r a5i a6r a6i a7r a7i
    B : b0r b0i b1r b1i b2r b2i b3r b3i

    Stage 1
    Extract a0r and the complete B vector to compute
    acc1 :  a0r.b0r -a0r.b0i a0r.b1r -a0r.b1i a0r.b2r -a0r.b2i a0r.b3r -a0r.b3i

    A selection:  buf_matA Start=0    Offset=0x00000000
    B selection:  buf_matB Start=0    Offset=0x76543210

    ones = false
    abs = false
    addmask = 0x00000000
    addmode = fpadd_mixadd  --> a mix between adds and subs 0xAA = 0x10101010
                            --> Lane 0 (left handside of the vector) is the LSB (right handside bit of the integer)
    cmpmode = fpcmp_nrm  --> no comparison before selection, just standard additions

    Stage 2
    Extract a0i and the complete B (with br bi order change) vector to compute
    acc1 :  acc1 +  a0i.b0i a0i.b0r a0i.b1i a0i.b1r a0i.b2i a0i.b2r a0i.b3i a0i.b3r

    A selection:  buf_matA Start=0    Offset=0x11111111
    B selection:  buf_matB Start=0    Offset=0x67452301

    ones = false
    abs = false
    addmask = 0x00000000
    addmode = fpadd_add  --> only additions in this first stages
    cmpmode = fpcmp_nrm  --> no comparison before selection, just standard additions
  */


   #define FPMUL   \
   buf_matB = as_v8float(window_readincr_v4(matB)); \
   acc1 = fpmul_conf(buf_matA,0,0x00000000,buf_matB,0,0x76543210,flag_ones,flag_abs,fpadd_mixadd,addmask,cmpmode);\
   acc2 = fpmul_conf(buf_matA,8,0x00000000,buf_matB,0,0x76543210,flag_ones,flag_abs,fpadd_mixadd,addmask,cmpmode);\
   acc1 = fpmac_conf(acc1,buf_matA,0,0x11111111,buf_matB,0,0x67452301,flag_ones,flag_abs,fpadd_add,addmask,cmpmode);\
   acc2 = fpmac_conf(acc2,buf_matA,8,0x11111111,buf_matB,0,0x67452301,flag_ones,flag_abs,fpadd_add,addmask,cmpmode)

   #define FPMAC(Start1, Start2)\
   buf_matB = as_v8float(window_readincr_v4(matB));\
   acc1 = fpmac_conf(acc1,buf_matA,Start1,0x00000000,buf_matB,0,0x76543210,flag_ones,flag_abs,fpadd_mixadd,addmask,cmpmode);\
   acc2 = fpmac_conf(acc2,buf_matA,Start2,0x00000000,buf_matB,0,0x76543210,flag_ones,flag_abs,fpadd_mixadd,addmask,cmpmode);\
   acc1 = fpmac_conf(acc1,buf_matA,Start1,0x11111111,buf_matB,0,0x67452301,flag_ones,flag_abs,fpadd_add,addmask,cmpmode);\
   acc2 = fpmac_conf(acc2,buf_matA,Start2,0x11111111,buf_matB,0,0x67452301,flag_ones,flag_abs,fpadd_add,addmask,cmpmode)


   for (unsigned int i=0;i<CF_Ra/2;i++)  // Each iteration computes 2 rows of C
   chess_prepare_for_pipelining
   chess_loop_range(8,)
   {
     FPMUL;
     FPMAC(2,10);
     FPMAC(4,12);
     FPMAC(6,14);

     buf_matA = as_v16float(window_readincr_v8(matA)); // reads the next 2 rows of A
     window_decr_v4(matB,4); // reset B pointer
     window_writeincr(matC,as_v4cfloat(acc1)); // Writes 1 row of C
     window_writeincr(matC,as_v4cfloat(acc2)); // Writes the next row of C
   }
   #undef FPMUL
   #undef FPMAC

}

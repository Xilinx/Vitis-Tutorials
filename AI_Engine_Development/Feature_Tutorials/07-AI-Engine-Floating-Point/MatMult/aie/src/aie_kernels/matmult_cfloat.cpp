/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "aie_kernels.h"


// 16x4  x  4x4
// Can be extended to (2L)x(4M)  (4M)x(4N)
void matmult_cfloat(
        input_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_A>> &  __restrict matA,
        input_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_B>> &  __restrict matB,
        output_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_C>> &  __restrict matC)
    {
        auto pA = begin_vector_random_circular<8>(matA);
        auto pB = begin_vector_random_circular<4>(matB);
        auto pC = begin_vector_random_circular<4>(matC);


        vector<cfloat,8> buf_matA = *pA++;      // holds the first 8 elements of matA
        vector<cfloat,4> buf_matB;  // Holds 4 elements of matB
        vector<cfloat,4> acc1;   // 4 accumulation values on even rows
        vector<cfloat,4> acc2;   // 4 accumulation values on odd rows

        #define FPMUL   \
        buf_matB = *pB++; \
        acc1 = fpmul(buf_matA,0,0x0000,buf_matB,0,0x3210);\
        acc2 = fpmul(buf_matA,4,0x0000,buf_matB,0,0x3210)

        #define FPMAC(Start1, Start2)\
        buf_matB = *pB++;\
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

            buf_matA = *pA++; // reads the next 2 rows of A
            pB -= 4; // reset B pointer
            *pC++ = acc1;
            *pC++ = acc2;
        }

        #undef FPMUL
        #undef FPMAC
    }


    void matmult_cfloat_conf(
        input_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_A>> &  __restrict matA,
        input_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_B>> &  __restrict matB,
        output_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_C>> &  __restrict matC)
        {
            auto pA = begin_vector_random_circular<8>(matA);
            auto pB = begin_vector_random_circular<4>(matB);
            auto pC = begin_vector_random_circular<4>(matC);

            vector<float,16> buf_matA = aie::vector_cast<float>( *pA++);      // holds the first 8 elements of matA
            vector<float,8> buf_matB ;  // Holds 4 elements of matB
            vector<float,8> acc1;   // 4 accumulation values on even rows
            vector<float,8> acc2;   // 4 accumulation values on odd rows

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
            buf_matB = aie::vector_cast<float>(*pB++); \
            acc1 = fpmul_conf(buf_matA,0,0x00000000,buf_matB,0,0x76543210,flag_ones,flag_abs,fpadd_add,addmask,cmpmode);\
            acc2 = fpmul_conf(buf_matA,8,0x00000000,buf_matB,0,0x76543210,flag_ones,flag_abs,fpadd_add,addmask,cmpmode);\
            acc1 = fpmac_conf(acc1,buf_matA,0,0x11111111,buf_matB,0,0x67452301,flag_ones,flag_abs,fpadd_mixsub,addmask,cmpmode);\
            acc2 = fpmac_conf(acc2,buf_matA,8,0x11111111,buf_matB,0,0x67452301,flag_ones,flag_abs,fpadd_mixsub,addmask,cmpmode)

            #define FPMAC(Start1, Start2)\
            buf_matB = aie::vector_cast<float>(*pB++); \
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

                buf_matA = aie::vector_cast<float>(*pA++);  // reads the next 2 rows of A
                pB -= 4;
                *pC++ = aie::vector_cast<cfloat>(acc1);
                *pC++ = aie::vector_cast<cfloat>(acc2);
            }
            #undef FPMUL
            #undef FPMAC

        }



        void matmult_cfloat_conj(
        input_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_A>> &  __restrict matA,
        input_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_B>> &  __restrict matB,
        output_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_C>> &  __restrict matC)
            {

                auto pA = begin_vector_random_circular<8>(matA);
                auto pB = begin_vector_random_circular<4>(matB);
                auto pC = begin_vector_random_circular<4>(matC);

                vector<float,16> buf_matA = aie::vector_cast<float>(*pA++);      // holds the first 8 elements of matA
                vector<float,8> buf_matB ;  // Holds 4 elements of matB
                vector<float,8> acc1;   // 4 accumulation values on even rows
                vector<float,8> acc2;   // 4 accumulation values on odd rows

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
                buf_matB = aie::vector_cast<float>(*pB++); \
                acc1 = fpmul_conf(buf_matA,0,0x00000000,buf_matB,0,0x76543210,flag_ones,flag_abs,fpadd_mixadd,addmask,cmpmode);\
                acc2 = fpmul_conf(buf_matA,8,0x00000000,buf_matB,0,0x76543210,flag_ones,flag_abs,fpadd_mixadd,addmask,cmpmode);\
                acc1 = fpmac_conf(acc1,buf_matA,0,0x11111111,buf_matB,0,0x67452301,flag_ones,flag_abs,fpadd_add,addmask,cmpmode);\
                acc2 = fpmac_conf(acc2,buf_matA,8,0x11111111,buf_matB,0,0x67452301,flag_ones,flag_abs,fpadd_add,addmask,cmpmode)

                #define FPMAC(Start1, Start2)\
                buf_matB = aie::vector_cast<float>(*pB++); \
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

                    buf_matA = aie::vector_cast<float>(*pA++);  // reads the next 2 rows of A
                    pB -= 4;
                    *pC++ = aie::vector_cast<cfloat>(acc1);
                    *pC++ = aie::vector_cast<cfloat>(acc2);

                }
                #undef FPMUL
                #undef FPMAC

            }

/*
Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "dma_hls.h"
#include <stdio.h>

//No of Samples in PLIO bit width(128 bit)
#define WRD_LN 8

#define SPLIT 3
#define CASC_LN 8
#define N_SAMPLES 1

//defining DIM_A, DIM_B, DIM_AB
#if GEMM_SIZE == 32
#define DIM_A 16 
#define DIM_B 12
#elif GEMM_SIZE == 64
#define DIM_A 32 
#define DIM_B 24
#elif GEMM_SIZE == 128
#define DIM_A 44 
#define DIM_B 44 
#elif GEMM_SIZE == 256
#define DIM_A 44 
#define DIM_B 44 
#elif GEMM_SIZE == 512
#define DIM_A 16 
#define DIM_B 16
#elif GEMM_SIZE == 1024
#define DIM_A 16 
#define DIM_B 16
#endif
#define DIM_AB (GEMM_SIZE)

//defining GEMM_SIZE_ZP_A, zero-padded Matrix-A size
#if (GEMM_SIZE % DIM_A) == 0 
    #define GEMM_SIZE_ZP_A GEMM_SIZE			
#else 
    #define GEMM_SIZE_ZP_A (GEMM_SIZE - (GEMM_SIZE % DIM_A) + DIM_A)	
#endif

//defining GEMM_SIZE_ZP_B, zero-padded Matrix-B size
#if (GEMM_SIZE % (DIM_B*SPLIT)) == 0 
    #define GEMM_SIZE_ZP_B GEMM_SIZE			
#else 
    #define GEMM_SIZE_ZP_B ((GEMM_SIZE) - ((GEMM_SIZE) % (DIM_B*SPLIT)) + (DIM_B*SPLIT))
#endif

//rows in each Cascade
#define GEMM_SZ_CASC (GEMM_SIZE/CASC_LN)

//No.of Blocks of A which does not requires Zero-padding	
#define NON_ZP_BLK_A (int(GEMM_SIZE/DIM_A)) 

//No.of Blocks of A which requires Zero-padding	
#define ZP_BLK_A (((GEMM_SIZE_ZP_A)/DIM_A) - (NON_ZP_BLK_A))

//No.of rows of zero in Block of A which requires Zero-padding	
#define ROWS_ZP_BLK_A ((GEMM_SZ_CASC)*(DIM_A*(ZP_BLK_A) - (GEMM_SIZE_ZP_A - GEMM_SIZE)))

//No.of cols in Block of B which requires Zero-padding	
#define ROWS_ZP_BLK_B  (((GEMM_SIZE_ZP_B)/SPLIT) - ((GEMM_SIZE_ZP_B) - GEMM_SIZE))												

//No.of Blocks of B which does not requires Zero-padding	
#define NON_ZP_BLK_B (int((ROWS_ZP_BLK_B)/DIM_B))

//No.of Blocks of B which requires Zero-padding	
#define ZP_BLK_B (((GEMM_SIZE_ZP_B)/SPLIT/DIM_B) - (NON_ZP_BLK_B))

//cols in each split of B-Matrix
#define GEMM_SZ_SPLIT (GEMM_SIZE_ZP_B/SPLIT)

//No.of zero in block of C-Matrix
#define GEMM_ZP_ROW_C (DIM_A*(ZP_BLK_A)*DIM_B -(GEMM_SIZE_ZP_A - GEMM_SIZE)*DIM_B)

//No.of elements in each channel of A
#define TOTAL_A_PER_CH ((GEMM_SZ_CASC)*((ZP_BLK_A)+(NON_ZP_BLK_A))*DIM_A/WRD_LN)

//No.of non-zero elements in each channel of A
#define NON_ZP_A_PER_CH (((GEMM_SZ_CASC)*DIM_A*(NON_ZP_BLK_A)/WRD_LN) +(ROWS_ZP_BLK_A/WRD_LN))

//No.of elements in each channel of B
#define TOTAL_B_PER_CH ((GEMM_SZ_SPLIT)*(GEMM_SZ_CASC)/WRD_LN)

//non-zero block elements in each channel of B
#define NON_ZP_B_PER_CH (GEMM_SZ_CASC*DIM_B*(NON_ZP_BLK_B)/WRD_LN)

//No.of elements in sub-block channel of B
#define TOTAL_SUB_B_PER_CH ((ZP_BLK_B)*DIM_B/2)

//non-zero block elements in sub-block channel of B
#define NON_ZP_SUB_PER_CH (((ZP_BLK_B)*DIM_B - (GEMM_SIZE_ZP_B - GEMM_SIZE))/2)

//No.of elements in each channel of C
#define TOTAL_C_PER_CH (DIM_A*DIM_B*((NON_ZP_BLK_A)+(ZP_BLK_A))/WRD_LN)

//non-zero elements in each channel of C
#define NON_ZP_C_PER_CH ((DIM_A*DIM_B*(NON_ZP_BLK_A))/WRD_LN + ((GEMM_ZP_ROW_C)/WRD_LN))

//No.of elements in each block of C
#define TOTAL_C_PER_BLK ((GEMM_SZ_SPLIT)*DIM_A/WRD_LN)

//non-zero elements in each block of C
#define NON_ZP_C_PER_BLK ((NON_ZP_BLK_B)*DIM_B*DIM_A/WRD_LN)

//No.of elements in each sub-block of C
#define TOTAL_C_PER_SUB_BLK ((ZP_BLK_B)*DIM_B/2)

//non-zero elements in each sub-block of C
#define NON_ZP_C_PER_SUB_BLK (((ZP_BLK_B)*DIM_B -(GEMM_SIZE_ZP_B - GEMM_SIZE))/2)
////////////////////////////////////////////////////////////
// Input to A...
////////////////////////////////////////////////////////////
void inp_A(
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A0,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A1,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A2,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A3,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A4,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A5,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A6,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A7,
   ap_int<32> matSz_A
  )
{
    ap_int<32> i = 0;
   //inp_A:for(ap_int<32> i =0;i< matSz_A;i++) {
   inp_A:for(ap_int<32> j =matSz_A;j;j--) {
             if( (matSz_A == -1 ) && (j==1) ) {
                 j = -1;
             }
      #pragma HLS PIPELINE II=1
      //#pragma HLS DATAFLOW
      #pragma HLS loop_tripcount min=256 max=8192
      ap_axiu<128, 0, 0, 0> A;
      ap_axiu<128, 0, 0, 0> A_ZP;
      A.data = ap_uint<128>("0x00010001000100010001000100010001",16);
      A_ZP.data = ap_uint<128>("0x00000000000000000000000000000000",16);
      A.keep=-1; 
      A_ZP.keep=-1; 
     // Generating Zero-paddig
    if (i%(TOTAL_A_PER_CH) > (NON_ZP_A_PER_CH - 1))  {
      strmOut_to_A0.write(A_ZP);
      strmOut_to_A1.write(A_ZP);
      strmOut_to_A2.write(A_ZP);
      strmOut_to_A3.write(A_ZP);
      strmOut_to_A4.write(A_ZP);
      strmOut_to_A5.write(A_ZP);
      strmOut_to_A6.write(A_ZP);
      strmOut_to_A7.write(A_ZP);
      }
      else {
      strmOut_to_A0.write(A);
      strmOut_to_A1.write(A);
      strmOut_to_A2.write(A);
      strmOut_to_A3.write(A);
      strmOut_to_A4.write(A);
      strmOut_to_A5.write(A);
      strmOut_to_A6.write(A);
      strmOut_to_A7.write(A);
   }
      i++;
  }
}

////////////////////////////////////////////////////////////
// Input to B...
////////////////////////////////////////////////////////////
void inp_B(
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B0,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B1,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B2,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B3,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B4,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B5,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B6,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B7,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B8,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B9,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B10,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B11,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B12,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B13,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B14,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B15,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B16,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B17,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B18,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B19,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B20,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B21,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B22,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B23,
   ap_int<32> matSz_B
  )
{
    ap_int<32> i = 0;
   inp_B:for(ap_int<32> j = matSz_B;j; j--) {
             if( (matSz_B == -1 ) && (j==1) ) {
                 j = -1;
             }
      #pragma HLS PIPELINE II=1
      //#pragma HLS DATAFLOW
      #pragma HLS loop_tripcount min=0 max=8192
      
      ap_axiu<128, 0, 0, 0> B;
      ap_axiu<128, 0, 0, 0> B_ZP;
      B.data= ap_uint<128>("0x00020002000200020002000200020002",16);
      B_ZP.data= ap_uint<128>("0x00000000000000000000000000000000",16);
      
      B.keep=-1;
      B_ZP.keep=-1;
      strmOut_to_B0.write(B);
      strmOut_to_B1.write(B);
      strmOut_to_B2.write(B);
      strmOut_to_B3.write(B);
      strmOut_to_B4.write(B);
      strmOut_to_B5.write(B);
      strmOut_to_B6.write(B);
      strmOut_to_B7.write(B);
      strmOut_to_B8.write(B);
      strmOut_to_B9.write(B);
      strmOut_to_B10.write(B);
      strmOut_to_B11.write(B);
      strmOut_to_B12.write(B);
      strmOut_to_B13.write(B);
      strmOut_to_B14.write(B);
      strmOut_to_B15.write(B);
     // Generating Zero-paddig
      if (((i%(TOTAL_B_PER_CH)) > (((NON_ZP_B_PER_CH) - 1))) && \
          (i%(TOTAL_SUB_B_PER_CH) > (NON_ZP_SUB_PER_CH - 1)))	{
      strmOut_to_B16.write(B_ZP);
      strmOut_to_B17.write(B_ZP);
      strmOut_to_B18.write(B_ZP);
      strmOut_to_B19.write(B_ZP);
      strmOut_to_B20.write(B_ZP);
      strmOut_to_B21.write(B_ZP);
      strmOut_to_B22.write(B_ZP);
      strmOut_to_B23.write(B_ZP);
      }
      else {
      strmOut_to_B16.write(B);
      strmOut_to_B17.write(B);
      strmOut_to_B18.write(B);
      strmOut_to_B19.write(B);
      strmOut_to_B20.write(B);
      strmOut_to_B21.write(B);
      strmOut_to_B22.write(B);
      strmOut_to_B23.write(B);
   }
      i++;
   }
}

////////////////////////////////////////////////////////////
// Output C...
////////////////////////////////////////////////////////////
void out_C(
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C0,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C1,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C2,
   ap_int<32> matSz_C, ap_uint<21> &errCnt, ap_uint<128> goldenVal, ap_uint<128> goldenVal_ZP
   )
{
    ap_int<32> i = 0;
   out_C:for(ap_int<32> j = matSz_C;j; j--) {
             if( (matSz_C == -1 ) && (j==1) ) {
                 j = -1;
             }
      #pragma HLS PIPELINE II=1
      //#pragma HLS DATAFLOW
      #pragma HLS loop_tripcount min=0 max=32768
      
      ap_axiu<128, 0, 0, 0> C0 = strmInp_from_C0.read();
      ap_axiu<128, 0, 0, 0> C1 = strmInp_from_C1.read();
      ap_axiu<128, 0, 0, 0> C2 = strmInp_from_C2.read();
      
    // Checking zero-padded value
    if ((i%(TOTAL_C_PER_CH)) > (NON_ZP_C_PER_CH -1)) {	
      if((C0.data != goldenVal_ZP) || (C1.data != goldenVal_ZP) || \
         (C2.data != goldenVal_ZP) )
      {
         ++errCnt;
      }
      }
    
    // Checking zero-padded value
    else if ((i%(TOTAL_C_PER_BLK) > (NON_ZP_C_PER_BLK -1)) && \											
    ((i%(TOTAL_C_PER_SUB_BLK)) > (NON_ZP_C_PER_SUB_BLK -1))) {												
      
      if((C0.data != goldenVal) || (C1.data != goldenVal) || \
         (C2.data != goldenVal_ZP) )
      {
         ++errCnt;

      }
         }
    // Checking Golden value
     else if((C0.data != goldenVal) || (C1.data != goldenVal) || \
         (C2.data != goldenVal))
      {
         ++errCnt;
      }
     i++;
   }
   }


////////////////////////////////////////////////////////////
// Top Function of Final Datamover unit for design without
// ddr, it provides input matrix A and B and checks for the 
// output in pl itself.
////////////////////////////////////////////////////////////
int dma_hls(
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A0,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A1,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A2,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A3,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A4,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A5,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A6,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A7,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B0,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B1,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B2,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B3,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B4,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B5,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B6,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B7,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B8,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B9,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B10,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B11,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B12,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B13,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B14,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B15,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B16,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B17,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B18,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B19,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B20,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B21,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B22,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B23,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C0,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C1,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C2,
   ap_int<32> matSz_A, ap_int<32> matSz_B, ap_int<32> matSz_C
   )
{
   #pragma HLS INTERFACE axis port=strmOut_to_A0
   #pragma HLS INTERFACE axis port=strmOut_to_A1
   #pragma HLS INTERFACE axis port=strmOut_to_A2
   #pragma HLS INTERFACE axis port=strmOut_to_A3
   #pragma HLS INTERFACE axis port=strmOut_to_A4
   #pragma HLS INTERFACE axis port=strmOut_to_A5
   #pragma HLS INTERFACE axis port=strmOut_to_A6
   #pragma HLS INTERFACE axis port=strmOut_to_A7
   #pragma HLS INTERFACE axis port=strmOut_to_B0  
   #pragma HLS INTERFACE axis port=strmOut_to_B1
   #pragma HLS INTERFACE axis port=strmOut_to_B2
   #pragma HLS INTERFACE axis port=strmOut_to_B3
   #pragma HLS INTERFACE axis port=strmOut_to_B4  
   #pragma HLS INTERFACE axis port=strmOut_to_B5
   #pragma HLS INTERFACE axis port=strmOut_to_B6
   #pragma HLS INTERFACE axis port=strmOut_to_B7
   #pragma HLS INTERFACE axis port=strmOut_to_B8  
   #pragma HLS INTERFACE axis port=strmOut_to_B9
   #pragma HLS INTERFACE axis port=strmOut_to_B10
   #pragma HLS INTERFACE axis port=strmOut_to_B11
   #pragma HLS INTERFACE axis port=strmOut_to_B12 
   #pragma HLS INTERFACE axis port=strmOut_to_B13
   #pragma HLS INTERFACE axis port=strmOut_to_B14
   #pragma HLS INTERFACE axis port=strmOut_to_B15
   #pragma HLS INTERFACE axis port=strmOut_to_B16
   #pragma HLS INTERFACE axis port=strmOut_to_B17
   #pragma HLS INTERFACE axis port=strmOut_to_B18
   #pragma HLS INTERFACE axis port=strmOut_to_B19
   #pragma HLS INTERFACE axis port=strmOut_to_B20
   #pragma HLS INTERFACE axis port=strmOut_to_B21
   #pragma HLS INTERFACE axis port=strmOut_to_B22
   #pragma HLS INTERFACE axis port=strmOut_to_B23
   #pragma HLS INTERFACE axis port=strmInp_from_C0 
   #pragma HLS INTERFACE axis port=strmInp_from_C1
   #pragma HLS INTERFACE axis port=strmInp_from_C2
   
   #pragma HLS INTERFACE s_axilite port=matSz_A bundle=control
   #pragma HLS INTERFACE s_axilite port=matSz_B bundle=control
   #pragma HLS INTERFACE s_axilite port=matSz_C bundle=control
   //#pragma HLS INTERFACE s_axilite port=iterCnt bundle=control
   #pragma HLS INTERFACE s_axilite port=return bundle=control  
   
   #pragma HLS DATAFLOW
   
   ap_uint<21> errCnt = 0;
      ap_uint<128> goldenVal_ZP = ap_uint<128> \
      ("0x00000000000000000000000000000000", 16);
   
   #if GEMM_SIZE == 32
      ap_uint<128> goldenVal = ap_uint<128> \
      ("0x00400040004000400040004000400040", 16);
   
   #elif GEMM_SIZE == 64
      ap_uint<128> goldenVal = ap_uint<128> \
      ("0x00800080008000800080008000800080", 16);
   
   #elif GEMM_SIZE == 128
      ap_uint<128> goldenVal = ap_uint<128> \
      ("0x01000100010001000100010001000100", 16);
   
   #elif GEMM_SIZE == 256
      ap_uint<128> goldenVal = ap_uint<128> \
      ("0x02000200020002000200020002000200", 16);
   
   #elif GEMM_SIZE == 512
      ap_uint<128> goldenVal = ap_uint<128> \
      ("0x04000400040004000400040004000400", 16);
   
   #elif GEMM_SIZE == 1024
      ap_uint<128> goldenVal = ap_uint<128> \
      ("0x08000800080008000800080008000800", 16);
   
   #endif
   
   //ITER_inp_A:for(int i = 0; i < iterCnt; ++i)
   //{
   //   #pragma HLS loop_tripcount min=1 max=16
   //   
   //   inp_A(strmOut_to_A0, strmOut_to_A1, strmOut_to_A2, strmOut_to_A3,
   //         matSz_A);
   //}
   
   //ITER_inp_B:for(int i = 0; i < iterCnt; ++i)
   //{
   //   #pragma HLS loop_tripcount min=1 max=16
   //   
   //   inp_B(strmOut_to_B0, strmOut_to_B1, strmOut_to_B2, strmOut_to_B3,
   //         strmOut_to_B4, strmOut_to_B5, strmOut_to_B6, strmOut_to_B7,
   //         strmOut_to_B8, strmOut_to_B9, strmOut_to_B10, strmOut_to_B11,
   //         strmOut_to_B12, strmOut_to_B13, strmOut_to_B14, strmOut_to_B15,
   //         strmOut_to_B16, strmOut_to_B17, strmOut_to_B18, strmOut_to_B19,
   //         strmOut_to_B20, strmOut_to_B21, strmOut_to_B22, strmOut_to_B23,
   //         strmOut_to_B24, strmOut_to_B25, strmOut_to_B26, strmOut_to_B27,
   //         strmOut_to_B28, strmOut_to_B29, strmOut_to_B30, strmOut_to_B31, matSz_B);
   //}
   //
   //ITER_out_C:for(int i = 0; i < iterCnt; ++i)
   //{
   //   #pragma HLS loop_tripcount min=1 max=16
   //
   //   out_C(strmInp_from_C0, strmInp_from_C1, strmInp_from_C2, strmInp_from_C3,strmInp_from_C4,\
   //   strmInp_from_C5, strmInp_from_C6, strmInp_from_C7, matSz_C, errCnt, goldenVal);
   //}
   
   inp_A(strmOut_to_A0, strmOut_to_A1, strmOut_to_A2, strmOut_to_A3, strmOut_to_A4, strmOut_to_A5, strmOut_to_A6, strmOut_to_A7,
         matSz_A);
   
   inp_B(strmOut_to_B0, strmOut_to_B1, strmOut_to_B2, strmOut_to_B3,
         strmOut_to_B4, strmOut_to_B5, strmOut_to_B6, strmOut_to_B7,
         strmOut_to_B8, strmOut_to_B9, strmOut_to_B10, strmOut_to_B11,
         strmOut_to_B12, strmOut_to_B13, strmOut_to_B14, strmOut_to_B15,
         strmOut_to_B16, strmOut_to_B17, strmOut_to_B18, strmOut_to_B19,
         strmOut_to_B20, strmOut_to_B21, strmOut_to_B22, strmOut_to_B23,
         matSz_B);
   
   out_C(strmInp_from_C0, strmInp_from_C1, strmInp_from_C2, matSz_C, errCnt, goldenVal, goldenVal_ZP);
   
   return errCnt;
}

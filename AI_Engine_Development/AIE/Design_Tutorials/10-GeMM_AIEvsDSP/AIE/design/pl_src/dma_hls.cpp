/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "dma_hls.h"

////////////////////////////////////////////////////////////
// Input to A...
////////////////////////////////////////////////////////////
void inp_A(
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A0,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A1,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A2,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_A3,
   ap_int<32> matSz_A
  )
{
   inp_A:for(ap_int<32> i = matSz_A; i; --i) {
      #pragma HLS PIPELINE II=1
      //#pragma HLS DATAFLOW
      #pragma HLS loop_tripcount min=256 max=8192
      ap_axiu<128, 0, 0, 0> A;
      A.data = ap_uint<128>("0x00010001000100010001000100010001",16);
      A.keep=-1; 
      strmOut_to_A0.write(A);
      strmOut_to_A1.write(A);
      strmOut_to_A2.write(A);
      strmOut_to_A3.write(A);
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
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B24,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B25,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B26,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B27,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B28,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B29,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B30,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B31,
   ap_int<32> matSz_B
  )
{
   inp_B:for(ap_int<32> i = matSz_B; i; --i) {
      #pragma HLS PIPELINE II=1
      //#pragma HLS DATAFLOW
      #pragma HLS loop_tripcount min=0 max=8192
      
      ap_axiu<128, 0, 0, 0> B;
      B.data= ap_uint<128>("0x00020002000200020002000200020002",16);
      
      B.keep=-1;
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
      strmOut_to_B16.write(B);
      strmOut_to_B17.write(B);
      strmOut_to_B18.write(B);
      strmOut_to_B19.write(B);
      strmOut_to_B20.write(B);
      strmOut_to_B21.write(B);
      strmOut_to_B22.write(B);
      strmOut_to_B23.write(B);
      strmOut_to_B24.write(B);
      strmOut_to_B25.write(B);
      strmOut_to_B26.write(B);
      strmOut_to_B27.write(B);
      strmOut_to_B28.write(B);
      strmOut_to_B29.write(B);
      strmOut_to_B30.write(B);
      strmOut_to_B31.write(B);
   }
}

////////////////////////////////////////////////////////////
// Output C...
////////////////////////////////////////////////////////////
void out_C(
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C0,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C1,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C2,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C3,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C4,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C5,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C6,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C7,
   ap_int<32> matSz_C, ap_uint<21> &errCnt, ap_uint<128> goldenVal
   )
{
   out_C:for(ap_int<32> i = matSz_C; i; --i) {
      #pragma HLS PIPELINE II=1
      //#pragma HLS DATAFLOW
      #pragma HLS loop_tripcount min=0 max=32768
      
      ap_axiu<128, 0, 0, 0> C0 = strmInp_from_C0.read();
      ap_axiu<128, 0, 0, 0> C1 = strmInp_from_C1.read();
      ap_axiu<128, 0, 0, 0> C2 = strmInp_from_C2.read();
      ap_axiu<128, 0, 0, 0> C3 = strmInp_from_C3.read();
      ap_axiu<128, 0, 0, 0> C4 = strmInp_from_C4.read();
      ap_axiu<128, 0, 0, 0> C5 = strmInp_from_C5.read();
      ap_axiu<128, 0, 0, 0> C6 = strmInp_from_C6.read();
      ap_axiu<128, 0, 0, 0> C7 = strmInp_from_C7.read();
      
      if((C0.data != goldenVal) || (C1.data != goldenVal) || \
         (C2.data != goldenVal) || (C3.data != goldenVal) || \
         (C4.data != goldenVal) || (C5.data != goldenVal) || \
         (C6.data != goldenVal) || (C7.data != goldenVal))
      {
         ++errCnt;
      }
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
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B24,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B25,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B26,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B27,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B28,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B29,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B30,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_B31,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C0,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C1,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C2,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C3,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C4,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C5,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C6,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_C7,
   ap_int<32> matSz_A, ap_int<32> matSz_B, ap_int<32> matSz_C
   )
{
   #pragma HLS INTERFACE axis port=strmOut_to_A0
   #pragma HLS INTERFACE axis port=strmOut_to_A1
   #pragma HLS INTERFACE axis port=strmOut_to_A2
   #pragma HLS INTERFACE axis port=strmOut_to_A3
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
   #pragma HLS INTERFACE axis port=strmOut_to_B24
   #pragma HLS INTERFACE axis port=strmOut_to_B25
   #pragma HLS INTERFACE axis port=strmOut_to_B26
   #pragma HLS INTERFACE axis port=strmOut_to_B27
   #pragma HLS INTERFACE axis port=strmOut_to_B28
   #pragma HLS INTERFACE axis port=strmOut_to_B29
   #pragma HLS INTERFACE axis port=strmOut_to_B30
   #pragma HLS INTERFACE axis port=strmOut_to_B31
   #pragma HLS INTERFACE axis port=strmInp_from_C0 
   #pragma HLS INTERFACE axis port=strmInp_from_C1
   #pragma HLS INTERFACE axis port=strmInp_from_C2
   #pragma HLS INTERFACE axis port=strmInp_from_C3
   #pragma HLS INTERFACE axis port=strmInp_from_C4 
   #pragma HLS INTERFACE axis port=strmInp_from_C5
   #pragma HLS INTERFACE axis port=strmInp_from_C6
   #pragma HLS INTERFACE axis port=strmInp_from_C7
   
   #pragma HLS INTERFACE s_axilite port=matSz_A bundle=control
   #pragma HLS INTERFACE s_axilite port=matSz_B bundle=control
   #pragma HLS INTERFACE s_axilite port=matSz_C bundle=control
   //#pragma HLS INTERFACE s_axilite port=iterCnt bundle=control
   #pragma HLS INTERFACE s_axilite port=return bundle=control  
   
   #pragma HLS DATAFLOW
   
   ap_uint<21> errCnt = 0;
   
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
   
   inp_A(strmOut_to_A0, strmOut_to_A1, strmOut_to_A2, strmOut_to_A3,
         matSz_A);
   
   inp_B(strmOut_to_B0, strmOut_to_B1, strmOut_to_B2, strmOut_to_B3,
         strmOut_to_B4, strmOut_to_B5, strmOut_to_B6, strmOut_to_B7,
         strmOut_to_B8, strmOut_to_B9, strmOut_to_B10, strmOut_to_B11,
         strmOut_to_B12, strmOut_to_B13, strmOut_to_B14, strmOut_to_B15,
         strmOut_to_B16, strmOut_to_B17, strmOut_to_B18, strmOut_to_B19,
         strmOut_to_B20, strmOut_to_B21, strmOut_to_B22, strmOut_to_B23,
         strmOut_to_B24, strmOut_to_B25, strmOut_to_B26, strmOut_to_B27,
         strmOut_to_B28, strmOut_to_B29, strmOut_to_B30, strmOut_to_B31, matSz_B);
   
   out_C(strmInp_from_C0, strmInp_from_C1, strmInp_from_C2, strmInp_from_C3,strmInp_from_C4,
         strmInp_from_C5, strmInp_from_C6, strmInp_from_C7, matSz_C, errCnt, goldenVal);
   
   return errCnt;
}

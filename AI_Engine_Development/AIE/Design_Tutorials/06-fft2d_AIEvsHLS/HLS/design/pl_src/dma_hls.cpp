/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "dma_hls.h"

////////////////////////////////////////////////////////////
// FFT_2D_DT=0(cint16)
// INP_DATA is Impulse input
// GOLDEN_DATA is all 1's (both real and imaginary)
// MIN_TRIPCOUNT =32x64/4 MAX_TRIPCOUNT=1024X2048/4 for cint16 
// since 4 samples are passed through 128 bit
////////////////////////////////////////////////////////////
#if FFT_2D_DT == 0
   #define INP_DATA 0X00010001
   #define GOLDEN_DATA 0X0001000100010001

////////////////////////////////////////////////////////////
// FFT_2D_DT=1(cfloat)
// INP_DATA is 1.5(real) and 1.5(imaginary)
// GOLDEN_DATA is all 1.5's (both real and imaginary)
// The INP_DATA and GOLDEN_DATA are provided under IEEE 754 format
// MIN_TRIPCOUNT =32x64/2 MAX_TRIPCOUNT=1024X2048/2 for cfloat 
// since 2 samples are passed through 128 bits
////////////////////////////////////////////////////////////
#elif FFT_2D_DT == 1
   #define INP_DATA 0x3fc000003fc00000
   #define GOLDEN_DATA 0x3fc000003fc00000

#endif

////////////////////////////////////////////////////////////
// Input to Rowise FFT...
////////////////////////////////////////////////////////////
void mm2s0(
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_rowiseFFT,
   ap_uint<25> matSz, ap_int<16> iterCnt
  )
{
   MM2S0_ITER:while(iterCnt--) {
      #pragma HLS loop_tripcount min=1 max=16
      
      MM2S0:for(ap_uint<25> i = 0; i < matSz; ++i) {
         #pragma HLS PIPELINE II=1
         
         #if FFT_2D_DT == 0   //cint16 - 32x64/4 to 1024x2048/4
            #pragma HLS loop_tripcount min=512 max=524288
         
         #else   //cfloat - 32x64/2 to 1024x2048/2
            #pragma HLS loop_tripcount min=1024 max=1048576
         
         #endif
         
         ap_axiu<128, 0, 0, 0> fftRow_inp;
         
         if(i == 0)
         {
            fftRow_inp.data = INP_DATA;
         }
         else
         {
            fftRow_inp.data = 0;
         }
          
         fftRow_inp.keep=-1;
         strmOut_to_rowiseFFT.write(fftRow_inp);
      }
      // If iterCnt is -1 keeping running the design infinitely...
      if(iterCnt == -2) {
         iterCnt = -1;
      }
      else {
         continue;
      }
      
      }
   }

////////////////////////////////////////////////////////////
// Datamover From Rowise FFT output to Colwise FFT Input...
////////////////////////////////////////////////////////////
void dmaHls_rowsToCols(
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_rowiseFFT,
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_colwiseFFT,
      ap_uint<25> matSz, ap_uint<13> rows, ap_uint<13> cols,
      ap_uint<25> &stg0_errCnt, ap_uint<128> goldenVal,
      ap_int<16> iterCnt
     )
{
   S2MM0_TO_MM2S1_ITER:while(iterCnt--) {
      #pragma HLS loop_tripcount min=1 max=16
      
      S2MM0:for(ap_uint<25> i = 0; i < matSz; ++i) {
         #pragma HLS PIPELINE II=1

         #if FFT_2D_DT == 0   //cint16 - 32x64/4 to 1024x2048/4
            #pragma HLS loop_tripcount min=512 max=524288
         
         #else   //cfloat - 32x64/2 to 1024x2048/2
            #pragma HLS loop_tripcount min=1024 max=1048576
         
         #endif
         
         ap_axiu<128, 0, 0, 0> fftRow_out = strmInp_from_rowiseFFT.read();
         
         // First row in the matrix should be all 1s and remaining 0s...
         if(i < cols)
         {
            if(fftRow_out.data != goldenVal)
            {
               ++stg0_errCnt;
            }
         }
         else if(fftRow_out.data != 0)
         {
            ++stg0_errCnt;
         }
      }
      
      MM2S1:for(ap_uint<25> i = 0, idx = 0; i < matSz; ++i) {
         #pragma HLS PIPELINE II=1

         #if FFT_2D_DT == 0   //cint16 - 32x64/4 to 1024x2048/4
            #pragma HLS loop_tripcount min=512 max=524288
         
         #else   //cfloat - 32x64/2 to 1024x2048/2
            #pragma HLS loop_tripcount min=1024 max=1048576
         
         #endif

         
         ap_axiu<128, 0, 0, 0> fftCol_inp;

         if(i == idx)
         {
            fftCol_inp.data = INP_DATA;
            idx += rows;
         }
         else
         {
            fftCol_inp.data = 0;
         }
      
         fftCol_inp.keep=-1;
         strmOut_to_colwiseFFT.write(fftCol_inp);
      }
      // If iterCnt is -1 keeping running the design infinitely...
      if(iterCnt == -2) {
         iterCnt = -1;
      }
      else {
         continue;
      }
    }  
}

////////////////////////////////////////////////////////////
// Output from Colwise FFT...
////////////////////////////////////////////////////////////
void s2mm1(
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_colwiseFFT,
      ap_uint<25> matSz, ap_uint<25> &stg1_errCnt, ap_uint<128> goldenVal,
      ap_int<16> iterCnt
     )
{
   S2MM1_ITER:while(iterCnt--) {
      #pragma HLS loop_tripcount min=1 max=16
      
      S2MM1:for(ap_uint<25> i = 0; i < matSz; ++i) {
         #pragma HLS PIPELINE II=1

         #if FFT_2D_DT == 0   //cint16 - 32x64/4 to 1024x2048/4
            #pragma HLS loop_tripcount min=512 max=524288
         
         #else   //cfloat - 32x64/2 to 1024x2048/2
            #pragma HLS loop_tripcount min=1024 max=1048576
         
         #endif

         ap_axiu<128, 0, 0, 0> fftCol_out = strmInp_from_colwiseFFT.read();
         
         // All Values should 1...
         if(fftCol_out.data != goldenVal)
         {
            ++stg1_errCnt;
         }
      }
      // If iterCnt is -1 keeping running the design infinitely...
      if(iterCnt == -2) {
         iterCnt = -1;
      }
      else {
         continue;
      }
      
   }
}

////////////////////////////////////////////////////////////
// Top Function of Final Datamover unit for design without
// ddr, it provides impulse input and checks for the output
// in pl itself. Done to see the max FFT2D throughput
// without any NoC/DDR bandwidth bottlenecks...
////////////////////////////////////////////////////////////
int dma_hls(
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_rowiseFFT,
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_rowiseFFT,
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_colwiseFFT,
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_colwiseFFT,
      ap_uint<25> matSz, ap_uint<13> rows, ap_uint<13> cols,
      ap_int<16> iterCnt
     )
{
   #pragma HLS INTERFACE axis port=strmOut_to_rowiseFFT
   #pragma HLS INTERFACE axis port=strmInp_from_rowiseFFT
   #pragma HLS INTERFACE axis port=strmOut_to_colwiseFFT
   #pragma HLS INTERFACE axis port=strmInp_from_colwiseFFT
   
   #pragma HLS INTERFACE s_axilite port=matSz bundle=control
   #pragma HLS INTERFACE s_axilite port=rows bundle=control
   #pragma HLS INTERFACE s_axilite port=cols bundle=control
   #pragma HLS INTERFACE s_axilite port=iterCnt bundle=control
   #pragma HLS INTERFACE s_axilite port=return bundle=control  
   
   #pragma HLS DATAFLOW
   
   ap_uint<25> stg0_errCnt = 0, stg1_errCnt = 0;
   
   ap_uint<128> goldenVal;

   goldenVal.range(127, 64) = GOLDEN_DATA;
   goldenVal.range( 63,  0) = GOLDEN_DATA;

   mm2s0(strmOut_to_rowiseFFT, matSz, iterCnt);
   dmaHls_rowsToCols(strmInp_from_rowiseFFT, strmOut_to_colwiseFFT, \
                     matSz, rows, cols, stg0_errCnt, goldenVal, iterCnt);
   s2mm1(strmInp_from_colwiseFFT, matSz, stg1_errCnt, goldenVal, iterCnt);

   return (stg0_errCnt + stg1_errCnt);
}

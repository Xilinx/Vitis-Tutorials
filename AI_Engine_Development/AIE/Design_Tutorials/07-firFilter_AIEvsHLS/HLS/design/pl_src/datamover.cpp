/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "datamover.h"

////////////////////////////////////////////////////////////
// Input to FIR...
////////////////////////////////////////////////////////////
void mm2s(
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOutToFIR,
   int size
  )
{
   MM2S:for(int i = 0; i < size; ++i) {
      #pragma HLS PIPELINE II=1
      #pragma HLS loop_tripcount min=512 max=5120
      
      ap_axiu<128, 0, 0, 0> firInp;
      
      if(i == 0)
      {
         firInp.data = INP_DATA;
      }
      else
      {
         firInp.data = 0x0;
      }
      firInp.keep =-1; 
      strmOutToFIR.write(firInp);
   }
}

////////////////////////////////////////////////////////////
// Output from FIR...
////////////////////////////////////////////////////////////
void s2mm(
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmInpFromFIR,
      int size, int &errCnt
     )
{
   S2MM:for(int i = 0, goldenCtr = 0; i < size; ++i) {
      #pragma HLS PIPELINE II=1
      #pragma HLS loop_tripcount min=512 max=5120

      ap_axiu<128, 0, 0, 0> firOut = strmInpFromFIR.read();

      #if N_FIR_FILTERS == 1
         // All Values should 1...
         if(i < CHK_CTR)
         {
            if(firOut.data != golden_128b[i])
               ++errCnt;
         }
         else if(firOut.data != 0x0)
            ++errCnt;

      #elif N_FIR_FILTERS == 10
         if(i< CHK_ZERO)
         {
            if(firOut.data != 0x0)
               ++errCnt;
         }
         else if(i < CHK_CTR)
         {
            if(firOut.data != golden_128b[i])
               ++errCnt;
         }
         else if(firOut.data != 0x0)
            ++errCnt;   
      #endif

   }
}

////////////////////////////////////////////////////////////
// Top Function of Final Datamover unit for design without
// ddr, it provides impulse input and checks for the output
// in pl itself. Done to see the max FIR throughput
// without any NoC/DDR bandwidth bottlenecks...
////////////////////////////////////////////////////////////
int datamover(
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmOutToFIR,
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmInpFromFIR,
      int size, int iterCnt
     )
{
   #pragma HLS INTERFACE axis port=strmOutToFIR
   #pragma HLS INTERFACE axis port=strmInpFromFIR
   
   #pragma HLS INTERFACE s_axilite port=size bundle=control
   #pragma HLS INTERFACE s_axilite port=iterCnt bundle=control
   #pragma HLS INTERFACE s_axilite port=return bundle=control  
   
   #pragma HLS DATAFLOW
   
   int errCnt = 0;
   
   ITER_MM2S:for(int i = iterCnt; i ; --i)
   {
      #pragma HLS loop_tripcount min=1 max=8
      
      mm2s(strmOutToFIR, size);
   }
   
   ITER_S2MM:for(int j = iterCnt; j ; --j)
   {
      #pragma HLS loop_tripcount min=1 max=8
      
      s2mm(strmInpFromFIR, size, errCnt);
   }

   return errCnt;
}

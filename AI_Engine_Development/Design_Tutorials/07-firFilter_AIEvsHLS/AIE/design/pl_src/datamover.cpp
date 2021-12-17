// © Copyright 2021 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
      #pragma HLS loop_tripcount min=64
      
      ap_axiu<128, 0, 0, 0> firInp;
      
      if(i == 0)
      {
         firInp.data = INP_DATA;
      }
      else
      {
         firInp.data = 0x0;
      }
       
      
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
   S2MM:for(int i = 0; i < size; ++i) {
      #pragma HLS PIPELINE II=1
      #pragma HLS loop_tripcount min=64

      ap_axiu<128, 0, 0, 0> firOut = strmInpFromFIR.read();
      
      // All Values should 1...
      if(i < CHK_CTR)
      {
         if(firOut.data != golden_128b[i])
            ++errCnt;
      }
      else if(firOut.data != 0x0)
         ++errCnt;
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
   
   ITER_MM2S:for(int i = 0; i < iterCnt; ++i)
   {
      #pragma HLS loop_tripcount min=1 max=8
      
      mm2s(strmOutToFIR, size);
   }
   
   ITER_S2MM:for(int j = 0; j < iterCnt; ++j)
   {
      #pragma HLS loop_tripcount min=1 max=8
      
      s2mm(strmInpFromFIR, size, errCnt);
   }

   return errCnt;
}

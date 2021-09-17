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

#pragma once

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

////////////////////////////////////////////////////////////
// Input to Rowise FFT...
////////////////////////////////////////////////////////////
void mm2s0(
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_rowiseFFT,
   int matSz
  );

////////////////////////////////////////////////////////////
// Datamover From Rowise FFT output to Colwise FFT Input...
////////////////////////////////////////////////////////////
void dmaHls_rowsToCols(
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_rowiseFFT,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_colwiseFFT,
      int matSz, int rows, int cols, int &stg0_errCnt,
      ap_uint<128> goldenVal
     );

////////////////////////////////////////////////////////////
// Output from Colwise FFT...
////////////////////////////////////////////////////////////
void s2mm1(
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_colwiseFFT,
      int matSz, int &stg1_errCnt, ap_uint<128> goldenVal
     );

////////////////////////////////////////////////////////////
// Top Function of Final Datamover unit for design without
// ddr, it provides impulse input and checks for the output
// in pl itself. Done to see the max FFT2D throughput
// without any NoC/DDR bandwidth bottlenecks...
////////////////////////////////////////////////////////////
int dma_hls(
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_rowiseFFT,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_rowiseFFT,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_colwiseFFT,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_colwiseFFT,
      int matSz, int rows, int cols, int iterCnt
     );

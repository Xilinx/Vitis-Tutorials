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
// Top Function of Final Datamover unit for design without
// ddr, it provides input matrix A and B and checks for the 
// output in pl itself.
////////////////////////////////////////////////////////////
int dma_hls(
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_A0,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_A1,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_A2,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_A3,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B0,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B1,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B2,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B3,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B4,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B5,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B6,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B7,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B8,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B9,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B10,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B11,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B12,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B13,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B14,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B15,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B16,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B17,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B18,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B19,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B20,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B21,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B22,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B23,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B24,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B25,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B26,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B27,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B28,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B29,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B30,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_B31,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_C0,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_C1,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_C2,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_C3,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_C4,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_C5,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_C6,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_C7,
   ap_int<32> matSz_A, ap_int<32> matSz_B, ap_int<32> matSz_C
   );

/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

////////////////////////////////////////////////////////////
// Input to Rowise FFT...
////////////////////////////////////////////////////////////
void mm2s0(
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_rowiseFFT,
   int matSz
  );

////////////////////////////////////////////////////////////
// Datamover From Rowise FFT output to Colwise FFT Input...
////////////////////////////////////////////////////////////
void dmaHls_rowsToCols(
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_rowiseFFT,
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_colwiseFFT,
      int matSz, int rows, int cols, int &stg0_errCnt,
      ap_uint<128> goldenVal
     );

////////////////////////////////////////////////////////////
// Output from Colwise FFT...
////////////////////////////////////////////////////////////
void s2mm1(
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_colwiseFFT,
      int matSz, int &stg1_errCnt, ap_uint<128> goldenVal
     );

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
      int matSz, int rows, int cols, int iterCnt
     );

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
   ap_uint<25> matSz, ap_int<16> iterCnt
  );

////////////////////////////////////////////////////////////
// Datamover From Rowise FFT output to Colwise FFT Input...
////////////////////////////////////////////////////////////
void dmaHls_rowsToCols(
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_rowiseFFT,
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut_to_colwiseFFT,
      ap_uint<25> matSz, ap_uint<13> rows, ap_uint<13> cols,
      ap_uint<25> &stg0_errCnt, ap_uint<128> goldenVal,
      ap_int<16> iterCnt
     );

////////////////////////////////////////////////////////////
// Output from Colwise FFT...
////////////////////////////////////////////////////////////
void s2mm1(
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp_from_colwiseFFT,
      ap_uint<25> matSz, ap_uint<25> &stg1_errCnt, ap_uint<128> goldenVal,
      ap_int<16> iterCnt
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
      ap_uint<25> matSz, ap_uint<13> rows, ap_uint<13> cols,
      ap_int<16> iterCnt
     );

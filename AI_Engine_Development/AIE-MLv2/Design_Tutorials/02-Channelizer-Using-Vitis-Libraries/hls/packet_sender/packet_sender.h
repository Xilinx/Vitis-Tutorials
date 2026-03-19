//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <complex>
#include <ap_fixed.h>
#include <hls_stream.h>
#include <hls_streamofblocks.h>
#include "ap_axi_sdata.h"
#include "packet_ids_c.h"

namespace packet_sender {
  static constexpr unsigned    NSTREAM_I = 2;
  static constexpr unsigned    NSTREAM_O = 2;
  static constexpr unsigned     NSAMPLES = 4096;
  static constexpr unsigned        DEPTH = NSAMPLES/2/4;// We shall have 2 stream-of-blocks, buffer depth in 128-bit words
  static constexpr unsigned        NBITS = 128;         // Size of PLIO bus on PL side @ 312.5 MHz
  typedef ap_uint<NBITS>                 TT_DATA;       // Equals four 'cint16' samples
  typedef ap_uint<NBITS/4>               TT_SAMPLE;     // Single 'cint16' sample (32-bit)
  typedef hls::stream<TT_DATA>           TT_STREAM;
  typedef hls::stream<ap_axiu<NBITS,0,0,0>> TT_STREAM_OUT;
  typedef TT_DATA                        TT_BLOCK[DEPTH];

  static const unsigned int pktType=0;
  static const int PACKET_NUM=16;     //How many kernels do packet switching
  static const unsigned int packet_ids_0[PACKET_NUM]={PLIO_i_0_0,  PLIO_i_0_1,  PLIO_i_0_2,  PLIO_i_0_3,
                                                      PLIO_i_0_4,  PLIO_i_0_5,  PLIO_i_0_6,  PLIO_i_0_7,
						      PLIO_i_0_8,  PLIO_i_0_9,  PLIO_i_0_10, PLIO_i_0_11,
						      PLIO_i_0_12, PLIO_i_0_13, PLIO_i_0_14, PLIO_i_0_15}; //macro values are generated in packet_ids_c.h
  static const unsigned int packet_ids_1[PACKET_NUM]={PLIO_i_1_0,  PLIO_i_1_1,  PLIO_i_1_2,  PLIO_i_1_3,
                                                      PLIO_i_1_4,  PLIO_i_1_5,  PLIO_i_1_6,  PLIO_i_1_7,
						      PLIO_i_1_8,  PLIO_i_1_9,  PLIO_i_1_10, PLIO_i_1_11,
						      PLIO_i_1_12, PLIO_i_1_13, PLIO_i_1_14, PLIO_i_1_15}; //macro values are generated in packet_ids_c.h
};

// Run:
void packet_sender_wrapper( packet_sender::TT_STREAM     sig_i[packet_sender::NSTREAM_I],
                            packet_sender::TT_STREAM_OUT sig_o[packet_sender::NSTREAM_O] );

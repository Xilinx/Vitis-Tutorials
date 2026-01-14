//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "split_1x16.h"
using namespace split_1x16;

// ------------------------------------------------------------
// Consumer
// ------------------------------------------------------------

void consumer( TT_STREAM& sig_i, hls::stream_of_blocks<TT_BLOCK>& ss )
{
#pragma HLS pipeline II=16
  // We consume 64 samples over 16 cycles (four cint16 @ 312.5 MHz)
  hls::write_lock<TT_BLOCK> WL(ss);
 CONSUME: for (unsigned ii=0; ii < DEPTH; ii++) {
    TT_DATA val = sig_i.read();
    WL[ii] = val;
  } // ii
}

// ------------------------------------------------------------
// Producer
// ------------------------------------------------------------

void producer( hls::stream_of_blocks<TT_BLOCK>& ss, TT_STREAM sig_o[NSTREAM_O] )
{
#pragma HLS pipeline II=16
  hls::read_lock<TT_BLOCK> RL(ss);
  TT_SAMPLE v00, v01, v02, v03;    (v03,v02,v01,v00) = RL[ 0];
  TT_SAMPLE v04, v05, v06, v07;    (v07,v06,v05,v04) = RL[ 1];
  TT_SAMPLE v08, v09, v10, v11;    (v11,v10,v09,v08) = RL[ 2];
  TT_SAMPLE v12, v13, v14, v15;    (v15,v14,v13,v12) = RL[ 3];
  TT_SAMPLE v16, v17, v18, v19;    (v19,v18,v17,v16) = RL[ 4];
  TT_SAMPLE v20, v21, v22, v23;    (v23,v22,v21,v20) = RL[ 5];
  TT_SAMPLE v24, v25, v26, v27;    (v27,v26,v25,v24) = RL[ 6];
  TT_SAMPLE v28, v29, v30, v31;    (v31,v30,v29,v28) = RL[ 7];
  TT_SAMPLE v32, v33, v34, v35;    (v35,v34,v33,v32) = RL[ 8];
  TT_SAMPLE v36, v37, v38, v39;    (v39,v38,v37,v36) = RL[ 9];
  TT_SAMPLE v40, v41, v42, v43;    (v43,v42,v41,v40) = RL[10];
  TT_SAMPLE v44, v45, v46, v47;    (v47,v46,v45,v44) = RL[11];
  TT_SAMPLE v48, v49, v50, v51;    (v51,v50,v49,v48) = RL[12];
  TT_SAMPLE v52, v53, v54, v55;    (v55,v54,v53,v52) = RL[13];
  TT_SAMPLE v56, v57, v58, v59;    (v59,v58,v57,v56) = RL[14];
  TT_SAMPLE v60, v61, v62, v63;    (v63,v62,v61,v60) = RL[15];

  // Deal outputs in polyphase order:
  sig_o[ 0].write( (v48,v32,v16,v00) );
  sig_o[ 1].write( (v49,v33,v17,v01) );
  sig_o[ 2].write( (v50,v34,v18,v02) );
  sig_o[ 3].write( (v51,v35,v19,v03) );
  sig_o[ 4].write( (v52,v36,v20,v04) );
  sig_o[ 5].write( (v53,v37,v21,v05) );
  sig_o[ 6].write( (v54,v38,v22,v06) );
  sig_o[ 7].write( (v55,v39,v23,v07) );
  sig_o[ 8].write( (v56,v40,v24,v08) );
  sig_o[ 9].write( (v57,v41,v25,v09) );
  sig_o[10].write( (v58,v42,v26,v10) );
  sig_o[11].write( (v59,v43,v27,v11) );
  sig_o[12].write( (v60,v44,v28,v12) );
  sig_o[13].write( (v61,v45,v29,v13) );
  sig_o[14].write( (v62,v46,v30,v14) );
  sig_o[15].write( (v63,v47,v31,v15) );
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void split_1x16_wrapper( TT_STREAM& sig_i, TT_STREAM sig_o[NSTREAM_O] )
{
#pragma HLS interface mode=ap_ctrl_none port=return
#pragma HLS interface axis       port=sig_i
#pragma HLS interface axis       port=sig_o

  hls::stream_of_blocks<TT_BLOCK> ss;
  //#pragma HLS array_partition variable=ss dim=1

#pragma HLS dataflow

  consumer( sig_i, ss );
  producer( ss, sig_o );

}




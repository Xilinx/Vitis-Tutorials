//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "packet_sender.h"
using namespace packet_sender;

// ------------------------------------------------------------
// Generate Header
// ------------------------------------------------------------

ap_uint<32> generateHeader(unsigned int pktType, unsigned int ID){
#pragma HLS inline
	ap_uint<32> header=0;
	header(4,0)=ID;
	header(11,5)=0;
	header(14,12)=pktType;
	header[15]=0;
	header(20,16)=-1;//source row
	header(27,21)=-1;//source column
	header(30,28)=0;
	header[31]=header(30,0).xor_reduce()?(ap_uint<1>)0:(ap_uint<1>)1;
	return header;
}

// ------------------------------------------------------------
// Consumer
// ------------------------------------------------------------

void consumer( TT_STREAM sig_i[NSTREAM_I], hls::stream_of_blocks<TT_BLOCK>& ss0, hls::stream_of_blocks<TT_BLOCK>& ss1 )
{
  hls::write_lock<TT_BLOCK> WL0(ss0);
  hls::write_lock<TT_BLOCK> WL1(ss1);
 CONSUME: for (unsigned ii=0; ii < DEPTH/16; ii++) {
#pragma HLS pipeline II=16
    TT_SAMPLE v00, v02, v04, v06;      (v06,v04,v02,v00)   = sig_i[ 0].read();
    TT_SAMPLE v01, v03, v05, v07;      (v07,v05,v03,v01)   = sig_i[ 1].read();
    TT_SAMPLE v08, v10, v12, v14;      (v14,v12,v10,v08)   = sig_i[ 0].read();
    TT_SAMPLE v09, v11, v13, v15;      (v15,v13,v11,v09)   = sig_i[ 1].read();
    TT_SAMPLE v16, v18, v20, v22;      (v22,v20,v18,v16)   = sig_i[ 0].read();
    TT_SAMPLE v17, v19, v21, v23;      (v23,v21,v19,v17)   = sig_i[ 1].read();
    TT_SAMPLE v24, v26, v28, v30;      (v30,v28,v26,v24)   = sig_i[ 0].read();
    TT_SAMPLE v25, v27, v29, v31;      (v31,v29,v27,v25)   = sig_i[ 1].read();
    TT_SAMPLE v32, v34, v36, v38;      (v38,v36,v34,v32)   = sig_i[ 0].read();
    TT_SAMPLE v33, v35, v37, v39;      (v39,v37,v35,v33)   = sig_i[ 1].read();
    TT_SAMPLE v40, v42, v44, v46;      (v46,v44,v42,v40)   = sig_i[ 0].read();
    TT_SAMPLE v41, v43, v45, v47;      (v47,v45,v43,v41)   = sig_i[ 1].read();
    TT_SAMPLE v48, v50, v52, v54;      (v54,v52,v50,v48)   = sig_i[ 0].read();
    TT_SAMPLE v49, v51, v53, v55;      (v55,v53,v51,v49)   = sig_i[ 1].read();
    TT_SAMPLE v56, v58, v60, v62;      (v62,v60,v58,v56)   = sig_i[ 0].read();
    TT_SAMPLE v57, v59, v61, v63;      (v63,v61,v59,v57)   = sig_i[ 1].read();
    TT_SAMPLE v64, v66, v68, v70;      (v70,v68,v66,v64)   = sig_i[ 0].read();
    TT_SAMPLE v65, v67, v69, v71;      (v71,v69,v67,v65)   = sig_i[ 1].read();
    TT_SAMPLE v72, v74, v76, v78;      (v78,v76,v74,v72)   = sig_i[ 0].read();
    TT_SAMPLE v73, v75, v77, v79;      (v79,v77,v75,v73)   = sig_i[ 1].read();
    TT_SAMPLE v80, v82, v84, v86;      (v86,v84,v82,v80)   = sig_i[ 0].read();
    TT_SAMPLE v81, v83, v85, v87;      (v87,v85,v83,v81)   = sig_i[ 1].read();
    TT_SAMPLE v88, v90, v92, v94;      (v94,v92,v90,v88)   = sig_i[ 0].read();
    TT_SAMPLE v89, v91, v93, v95;      (v95,v93,v91,v89)   = sig_i[ 1].read();
    TT_SAMPLE v96, v98, v100, v102;    (v102,v100,v98,v96)   = sig_i[ 0].read();
    TT_SAMPLE v97, v99, v101, v103;    (v103,v101,v99,v97)   = sig_i[ 1].read();
    TT_SAMPLE v104, v106, v108, v110;  (v110,v108,v106,v104) = sig_i[ 0].read();
    TT_SAMPLE v105, v107, v109, v111;  (v111,v109,v107,v105) = sig_i[ 1].read();
    TT_SAMPLE v112, v114, v116, v118;  (v118,v116,v114,v112) = sig_i[ 0].read();
    TT_SAMPLE v113, v115, v117, v119;  (v119,v117,v115,v113) = sig_i[ 1].read();
    TT_SAMPLE v120, v122, v124, v126;  (v126,v124,v122,v120) = sig_i[ 0].read();
    TT_SAMPLE v121, v123, v125, v127;  (v127,v125,v123,v121) = sig_i[ 1].read();
    
    WL0[ii    ] = (v96,v64,v32,v00);
    WL0[ii+32 ] = (v97,v65,v33,v01);
    WL0[ii+64 ] = (v98,v66,v34,v02);
    WL0[ii+96 ] = (v99,v67,v35,v03);
    WL0[ii+128] = (v100,v68,v36,v04);
    WL0[ii+160] = (v101,v69,v37,v05);
    WL0[ii+192] = (v102,v70,v38,v06);
    WL0[ii+224] = (v103,v71,v39,v07);
    WL0[ii+256] = (v104,v72,v40,v08);
    WL0[ii+288] = (v105,v73,v41,v09);
    WL0[ii+320] = (v106,v74,v42,v10);
    WL0[ii+352] = (v107,v75,v43,v11);
    WL0[ii+384] = (v108,v76,v44,v12);
    WL0[ii+416] = (v109,v77,v45,v13);
    WL0[ii+448] = (v110,v78,v46,v14);
    WL0[ii+480] = (v111,v79,v47,v15);

    WL1[ii    ] = (v112,v80,v48,v16);
    WL1[ii+32 ] = (v113,v81,v49,v17);
    WL1[ii+64 ] = (v114,v82,v50,v18);
    WL1[ii+96 ] = (v115,v83,v51,v19);
    WL1[ii+128] = (v116,v84,v52,v20);
    WL1[ii+160] = (v117,v85,v53,v21);
    WL1[ii+192] = (v118,v86,v54,v22);
    WL1[ii+224] = (v119,v87,v55,v23);
    WL1[ii+256] = (v120,v88,v56,v24);
    WL1[ii+288] = (v121,v89,v57,v25);
    WL1[ii+320] = (v122,v90,v58,v26);
    WL1[ii+352] = (v123,v91,v59,v27);
    WL1[ii+384] = (v124,v92,v60,v28);
    WL1[ii+416] = (v125,v93,v61,v29);
    WL1[ii+448] = (v126,v94,v62,v30);
    WL1[ii+480] = (v127,v95,v63,v31);
 }
}

// ------------------------------------------------------------
// Producer
// ------------------------------------------------------------

void producer( hls::stream_of_blocks<TT_BLOCK>& ss0, hls::stream_of_blocks<TT_BLOCK>& ss1, TT_STREAM_OUT sig_o[NSTREAM_O] )
{
  //std::cout << "CMD, D, D, TKEEP, TLAST" << std::endl;
  hls::read_lock<TT_BLOCK> RL0(ss0);
  hls::read_lock<TT_BLOCK> RL1(ss1);

  ap_axiu<NBITS,0,0,0> tmp0;
  ap_axiu<NBITS,0,0,0> tmp1;
 
  OUTER: for (unsigned ii=0; ii < PACKET_NUM; ii++) {
#pragma HLS pipeline II=33
    // Stride-8 pattern: send packets in order 0,8,1,9,2,10,3,11,4,12,5,13,6,14,7,15
    unsigned int pkt_idx = (ii / 2) + ((ii % 2) * 8);
    TT_SAMPLE s00, s01, s02, s03;
    TT_SAMPLE s04, s05, s06, s07;
    (s03,s02,s01,s00) = RL0[0+pkt_idx*32];  // Extract 4 samples from 128-bit word (32 words per packet)
    (s07,s06,s05,s04) = RL1[0+pkt_idx*32];

    unsigned int ID0=packet_ids_0[pkt_idx];
    ap_uint<32> header0=generateHeader(pktType,ID0); //packet header
    tmp0.data=(s02,s01,s00,header0);  // header + first 3 samples
    tmp0.keep=0xFFFF;
    tmp0.last=0;
    sig_o[0].write( tmp0 );
    //std::cout << "DATA, " << header0 << ", " << s00 << ", " << std::hex << tmp0.keep << ", " << std::dec << tmp0.last << std::endl;
    
    unsigned int ID1=packet_ids_1[pkt_idx];
    ap_uint<32> header1=generateHeader(pktType,ID1); //packet header
    tmp1.data=(s06,s05,s04,header1);  // header + first 3 samples
    tmp1.keep=0xFFFF;
    tmp1.last=0;
    sig_o[1].write( tmp1 );

    INNER: for (unsigned jj=1; jj < NSAMPLES/PACKET_NUM/4/2; jj++) {
      TT_SAMPLE v00, v01, v02, v03;
      TT_SAMPLE v04, v05, v06, v07;
      (v03,v02,v01,v00) = RL0[jj+pkt_idx*32];
      (v07,v06,v05,v04) = RL1[jj+pkt_idx*32];
      tmp0.data=(v02,v01,v00,s03);
      tmp0.keep=0xFFFF;
      tmp0.last=0;
      tmp1.data=(v06,v05,v04,s07);
      tmp1.keep=0xFFFF;
      tmp1.last=0;
      sig_o[0].write( tmp0 );
      sig_o[1].write( tmp1 );
      //std::cout << "DATA, " << s01 << ", " << v00 << ", " << std::hex << tmp0.keep << ", " << std::dec << tmp0.last << std::endl;
      s03=v03;
      s07=v07;
    }
    tmp0.data=(TT_SAMPLE(0),TT_SAMPLE(0),TT_SAMPLE(0),s03);
    tmp0.keep=0x000F;  // 0x000F - only lower 4 bytes (one cint16 sample) valid
    tmp0.last=1;

    tmp1.data=(TT_SAMPLE(0),TT_SAMPLE(0),TT_SAMPLE(0),s07);
    tmp1.keep=0x000F;  // 0x000F - only lower 4 bytes valid
    tmp1.last=1;

    sig_o[0].write( tmp0 );
    sig_o[1].write( tmp1 );
    //std::cout << "DATA, " << s01 << ", " << TT_SAMPLE(0) << ", " << std::hex << tmp0.keep << ", " << std::dec << tmp0.last << std::endl;
  }
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void packet_sender_wrapper( TT_STREAM sig_i[NSTREAM_I],TT_STREAM_OUT sig_o[NSTREAM_O] )
{
#pragma HLS interface mode=ap_ctrl_none port=return
#pragma HLS interface axis       port=sig_i
#pragma HLS interface axis       port=sig_o

  hls::stream_of_blocks<TT_BLOCK> ss0,ss1;
#pragma HLS bind_storage variable=ss0 type=ram_s2p impl=lutram
#pragma HLS bind_storage variable=ss1 type=ram_s2p impl=lutram

#pragma HLS dataflow

  consumer( sig_i, ss0, ss1 );
  producer( ss0, ss1, sig_o );
}




/*Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
  
#ifndef _PACKET_RECEIVER_H_
#define _PACKET_RECEIVER_H_

#include <cstring>
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include "../../Module_02_aie/build/Work_x4_x100/temp/packet_ids_c.h"

#define PACKET_NUM 4 
#define PKTTYPE 0 
#define PACKET_LEN 224
#define BURST_SIZE PACKET_LEN * PACKET_NUM //4 packets 

typedef ap_uint<32> data_t;
typedef ap_axiu<32, 0, 0, 0> axis_pkt;

static const unsigned int packet_ids[PACKET_NUM]={in_i0_0, in_i0_1, in_i0_2, in_i0_3}; //macro values are generated in packet_ids_c.h

unsigned int getPacketId(ap_uint<32> header);
void packet_receiver(hls::stream<axis_pkt>& rx0,
    hls::stream<axis_pkt>& rx1,
    hls::stream<axis_pkt>& rx2,
    hls::stream<axis_pkt>& rx3,
    hls::stream<axis_pkt>& rx4,
    hls::stream<axis_pkt>& rx5,
    hls::stream<axis_pkt>& rx6,
    hls::stream<axis_pkt>& rx7,
    hls::stream<axis_pkt>& rx8,
    hls::stream<axis_pkt>& rx9,
    hls::stream<axis_pkt>& rx10,
    hls::stream<axis_pkt>& rx11,
    hls::stream<axis_pkt>& rx12,
    hls::stream<axis_pkt>& rx13,
    hls::stream<axis_pkt>& rx14,
    hls::stream<axis_pkt>& rx15,
    hls::stream<axis_pkt>& rx16,
    hls::stream<axis_pkt>& rx17,
    hls::stream<axis_pkt>& rx18,
    hls::stream<axis_pkt>& rx19,
    hls::stream<axis_pkt>& rx20,
    hls::stream<axis_pkt>& rx21,
    hls::stream<axis_pkt>& rx22,
    hls::stream<axis_pkt>& rx23,
    hls::stream<axis_pkt>& rx24,
    hls::stream<axis_pkt>& rx25,
    hls::stream<axis_pkt>& rx26,
    hls::stream<axis_pkt>& rx27,
    hls::stream<axis_pkt>& rx28,
    hls::stream<axis_pkt>& rx29,
    hls::stream<axis_pkt>& rx30,
    hls::stream<axis_pkt>& rx31,
    hls::stream<axis_pkt>& rx32,
    hls::stream<axis_pkt>& rx33,
    hls::stream<axis_pkt>& rx34,
    hls::stream<axis_pkt>& rx35,
    hls::stream<axis_pkt>& rx36,
    hls::stream<axis_pkt>& rx37,
    hls::stream<axis_pkt>& rx38,
    hls::stream<axis_pkt>& rx39,
    hls::stream<axis_pkt>& rx40,
    hls::stream<axis_pkt>& rx41,
    hls::stream<axis_pkt>& rx42,
    hls::stream<axis_pkt>& rx43,
    hls::stream<axis_pkt>& rx44,
    hls::stream<axis_pkt>& rx45,
    hls::stream<axis_pkt>& rx46,
    hls::stream<axis_pkt>& rx47,
    hls::stream<axis_pkt>& rx48,
    hls::stream<axis_pkt>& rx49,
    hls::stream<axis_pkt>& rx50,
    hls::stream<axis_pkt>& rx51,
    hls::stream<axis_pkt>& rx52,
    hls::stream<axis_pkt>& rx53,
    hls::stream<axis_pkt>& rx54,
    hls::stream<axis_pkt>& rx55,
    hls::stream<axis_pkt>& rx56,
    hls::stream<axis_pkt>& rx57,
    hls::stream<axis_pkt>& rx58,
    hls::stream<axis_pkt>& rx59,
    hls::stream<axis_pkt>& rx60,
    hls::stream<axis_pkt>& rx61,
    hls::stream<axis_pkt>& rx62,
    hls::stream<axis_pkt>& rx63,
    hls::stream<axis_pkt>& rx64,
    hls::stream<axis_pkt>& rx65,
    hls::stream<axis_pkt>& rx66,
    hls::stream<axis_pkt>& rx67,
    hls::stream<axis_pkt>& rx68,
    hls::stream<axis_pkt>& rx69,
    hls::stream<axis_pkt>& rx70,
    hls::stream<axis_pkt>& rx71,
    hls::stream<axis_pkt>& rx72,
    hls::stream<axis_pkt>& rx73,
    hls::stream<axis_pkt>& rx74,
    hls::stream<axis_pkt>& rx75,
    hls::stream<axis_pkt>& rx76,
    hls::stream<axis_pkt>& rx77,
    hls::stream<axis_pkt>& rx78,
    hls::stream<axis_pkt>& rx79,
    hls::stream<axis_pkt>& rx80,
    hls::stream<axis_pkt>& rx81,
    hls::stream<axis_pkt>& rx82,
    hls::stream<axis_pkt>& rx83,
    hls::stream<axis_pkt>& rx84,
    hls::stream<axis_pkt>& rx85,
    hls::stream<axis_pkt>& rx86,
    hls::stream<axis_pkt>& rx87,
    hls::stream<axis_pkt>& rx88,
    hls::stream<axis_pkt>& rx89,
    hls::stream<axis_pkt>& rx90,
    hls::stream<axis_pkt>& rx91,
    hls::stream<axis_pkt>& rx92,
    hls::stream<axis_pkt>& rx93,
    hls::stream<axis_pkt>& rx94,
    hls::stream<axis_pkt>& rx95,
    hls::stream<axis_pkt>& rx96,
    hls::stream<axis_pkt>& rx97,
    hls::stream<axis_pkt>& rx98,
		     hls::stream<axis_pkt>& rx99,
		     hls::stream<axis_pkt>& tx0,
		     hls::stream<axis_pkt>& tx1,
		     hls::stream<axis_pkt>& tx2,
		     hls::stream<axis_pkt>& tx3
		     );
    
#endif // _PACKET_RECEIVER_H_


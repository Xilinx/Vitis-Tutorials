/*Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
  
#ifndef _PACKET_SENDER_H_
#define _PACKET_SENDER_H_
    
#include <cstring>
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include "../../Module_02_aie/build/Work_x4_x100/temp/packet_ids_c.h"

#define NUM_CU 100     
#define PACKET_NUM 4 
#define PKTTYPE 0 
#define PACKET_LEN 224
#define BURST_SIZE PACKET_LEN * PACKET_NUM * NUM_CU //4 packets 
    
typedef ap_uint<32> data_t;
typedef ap_axiu<32, 0, 0, 0> axis_pkt;
    
static const unsigned int packet_ids[PACKET_NUM]={in_i0_0, in_i0_1, in_i0_2, in_i0_3}; //macro values are generated in packet_ids_c.h
    
    
data_t generateHeader(unsigned int pktType, unsigned int ID);
void packet_sender(hls::stream<axis_pkt>& rx,

                   hls::stream<axis_pkt>& tx0,
                   hls::stream<axis_pkt>& tx1,
                   hls::stream<axis_pkt>& tx2,
                   hls::stream<axis_pkt>& tx3,
                   hls::stream<axis_pkt>& tx4,
                   hls::stream<axis_pkt>& tx5,
                   hls::stream<axis_pkt>& tx6,
                   hls::stream<axis_pkt>& tx7,
                   hls::stream<axis_pkt>& tx8,
                   hls::stream<axis_pkt>& tx9,
                   hls::stream<axis_pkt>& tx10,
                   hls::stream<axis_pkt>& tx11,
                   hls::stream<axis_pkt>& tx12,
                   hls::stream<axis_pkt>& tx13,
                   hls::stream<axis_pkt>& tx14,
                   hls::stream<axis_pkt>& tx15,
                   hls::stream<axis_pkt>& tx16,
                   hls::stream<axis_pkt>& tx17,
                   hls::stream<axis_pkt>& tx18,
                   hls::stream<axis_pkt>& tx19,
                   hls::stream<axis_pkt>& tx20,
                   hls::stream<axis_pkt>& tx21,
                   hls::stream<axis_pkt>& tx22,
                   hls::stream<axis_pkt>& tx23,
                   hls::stream<axis_pkt>& tx24,
                   hls::stream<axis_pkt>& tx25,
                   hls::stream<axis_pkt>& tx26,
                   hls::stream<axis_pkt>& tx27,
                   hls::stream<axis_pkt>& tx28,
                   hls::stream<axis_pkt>& tx29,
                   hls::stream<axis_pkt>& tx30,
                   hls::stream<axis_pkt>& tx31,
                   hls::stream<axis_pkt>& tx32,
                   hls::stream<axis_pkt>& tx33,
                   hls::stream<axis_pkt>& tx34,
                   hls::stream<axis_pkt>& tx35,
                   hls::stream<axis_pkt>& tx36,
                   hls::stream<axis_pkt>& tx37,
                   hls::stream<axis_pkt>& tx38,
                   hls::stream<axis_pkt>& tx39,
                   hls::stream<axis_pkt>& tx40,
                   hls::stream<axis_pkt>& tx41,
                   hls::stream<axis_pkt>& tx42,
                   hls::stream<axis_pkt>& tx43,
                   hls::stream<axis_pkt>& tx44,
                   hls::stream<axis_pkt>& tx45,
                   hls::stream<axis_pkt>& tx46,
                   hls::stream<axis_pkt>& tx47,
                   hls::stream<axis_pkt>& tx48,
                   hls::stream<axis_pkt>& tx49,
                   hls::stream<axis_pkt>& tx50,
                   hls::stream<axis_pkt>& tx51,
                   hls::stream<axis_pkt>& tx52,
                   hls::stream<axis_pkt>& tx53,
                   hls::stream<axis_pkt>& tx54,
                   hls::stream<axis_pkt>& tx55,
                   hls::stream<axis_pkt>& tx56,
                   hls::stream<axis_pkt>& tx57,
                   hls::stream<axis_pkt>& tx58,
                   hls::stream<axis_pkt>& tx59,
                   hls::stream<axis_pkt>& tx60,
                   hls::stream<axis_pkt>& tx61,
                   hls::stream<axis_pkt>& tx62,
                   hls::stream<axis_pkt>& tx63,
                   hls::stream<axis_pkt>& tx64,
                   hls::stream<axis_pkt>& tx65,
                   hls::stream<axis_pkt>& tx66,
                   hls::stream<axis_pkt>& tx67,
                   hls::stream<axis_pkt>& tx68,
                   hls::stream<axis_pkt>& tx69,
                   hls::stream<axis_pkt>& tx70,
                   hls::stream<axis_pkt>& tx71,
                   hls::stream<axis_pkt>& tx72,
                   hls::stream<axis_pkt>& tx73,
                   hls::stream<axis_pkt>& tx74,
                   hls::stream<axis_pkt>& tx75,
                   hls::stream<axis_pkt>& tx76,
                   hls::stream<axis_pkt>& tx77,
                   hls::stream<axis_pkt>& tx78,
                   hls::stream<axis_pkt>& tx79,
                   hls::stream<axis_pkt>& tx80,
                   hls::stream<axis_pkt>& tx81,
                   hls::stream<axis_pkt>& tx82,
                   hls::stream<axis_pkt>& tx83,
                   hls::stream<axis_pkt>& tx84,
                   hls::stream<axis_pkt>& tx85,
                   hls::stream<axis_pkt>& tx86,
                   hls::stream<axis_pkt>& tx87,
                   hls::stream<axis_pkt>& tx88,
                   hls::stream<axis_pkt>& tx89,
                   hls::stream<axis_pkt>& tx90,
                   hls::stream<axis_pkt>& tx91,
                   hls::stream<axis_pkt>& tx92,
                   hls::stream<axis_pkt>& tx93,
                   hls::stream<axis_pkt>& tx94,
                   hls::stream<axis_pkt>& tx95,
                   hls::stream<axis_pkt>& tx96,
                   hls::stream<axis_pkt>& tx97,
                   hls::stream<axis_pkt>& tx98,
                   hls::stream<axis_pkt>& tx99
                   );
#endif // _PACKET_SENDER_H_

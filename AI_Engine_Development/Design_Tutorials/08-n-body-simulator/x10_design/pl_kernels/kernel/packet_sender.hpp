/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
  
#ifndef _PACKET_SENDER_H_
#define _PACKET_SENDER_H_
    
#include <cstring>
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include "../../aie/build/Work_x4_x10/temp/packet_ids_c.h"

#define NUM_CU 10    
#define PACKET_NUM 4 
#define PKTTYPE 0 
#define PACKET_LEN 224
#define BURST_SIZE PACKET_LEN * PACKET_NUM //4 packets 
    
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
                   hls::stream<axis_pkt>& tx9
                   );
#endif // _PACKET_SENDER_H_

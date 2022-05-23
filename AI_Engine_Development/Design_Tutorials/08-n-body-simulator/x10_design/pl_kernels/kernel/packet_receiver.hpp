/* (c) Copyright 2021 Xilinx, Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 * 		http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */
  
#ifndef _PACKET_RECEIVER_H_
#define _PACKET_RECEIVER_H_

#include <cstring>
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include "../../aie/build/Work_x4_x10/temp/packet_ids_c.h"

#define PACKET_NUM 4 
#define NUM_CU 10
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
		     hls::stream<axis_pkt>& tx0,
		     hls::stream<axis_pkt>& tx1,
		     hls::stream<axis_pkt>& tx2,
		     hls::stream<axis_pkt>& tx3
		     );
    
#endif // _PACKET_RECEIVER_H_


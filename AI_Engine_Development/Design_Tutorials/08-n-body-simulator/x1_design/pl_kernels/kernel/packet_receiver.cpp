/*Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "packet_receiver.hpp"

unsigned int getPacketId(ap_uint<32> header){
#pragma HLS inline
	ap_uint<32> ID=0;
	ID(4,0)=header(4,0);
	return ID;
}

void packet_receiver(hls::stream<axis_pkt>& rx, hls::stream<axis_pkt>& tx0,
		     hls::stream<axis_pkt>& tx1,
		     hls::stream<axis_pkt>& tx2,
		     hls::stream<axis_pkt>& tx3)
{
#pragma HLS INTERFACE axis port=rx
#pragma HLS INTERFACE axis port=tx0
#pragma HLS INTERFACE axis port=tx1
#pragma HLS INTERFACE axis port=tx2
#pragma HLS INTERFACE axis port=tx3

#pragma HLS interface s_axilite port=return bundle=control  
  
  axis_pkt x;
  
  for (int h = 0; h < PACKET_NUM; h++) 
    { 
      ap_axiu<32,0,0,0> tmp=rx.read();//first word is packet header
      unsigned int ID=getPacketId(tmp.data);
      unsigned int channel=packet_ids[ID];

      for(int i = 0; i < PACKET_LEN; i++)
	{
#pragma HLS PIPELINE II=1
	  x=rx.read();
	  switch(channel){
	  case 0:tx0.write(x);break;
	  case 1:tx1.write(x);break;
	  case 2:tx2.write(x);break;
	  case 3:tx3.write(x);break;
	  }
	}
    }
}

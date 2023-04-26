/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
  
#include "packet_receiver.hpp"

unsigned int getPacketId(ap_uint<32> header){
#pragma HLS inline
	ap_uint<32> ID=0;
	ID(4,0)=header(4,0);
	return ID;
}

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
		     hls::stream<axis_pkt>& tx3)
{

#pragma HLS INTERFACE axis port=rx0
#pragma HLS INTERFACE axis port=rx1
#pragma HLS INTERFACE axis port=rx2
#pragma HLS INTERFACE axis port=rx3
#pragma HLS INTERFACE axis port=rx4
#pragma HLS INTERFACE axis port=rx5
#pragma HLS INTERFACE axis port=rx6
#pragma HLS INTERFACE axis port=rx7
#pragma HLS INTERFACE axis port=rx8
#pragma HLS INTERFACE axis port=rx9

#pragma HLS INTERFACE axis port=tx0
#pragma HLS INTERFACE axis port=tx1
#pragma HLS INTERFACE axis port=tx2
#pragma HLS INTERFACE axis port=tx3

#pragma HLS interface s_axilite port=return bundle=control  
  
  axis_pkt x;
  ap_axiu<32,0,0,0> tmp;
  
  for (int cu =0; cu < NUM_CU; cu++){
    for (int h = 0; h < PACKET_NUM; h++) 
      { 
	//first word is packet header
	switch(cu){	
        case 0:tmp=rx0.read();break;	
        case 1:tmp=rx1.read();break;	
        case 2:tmp=rx2.read();break;	
        case 3:tmp=rx3.read();break;	
        case 4:tmp=rx4.read();break;	
        case 5:tmp=rx5.read();break;	
        case 6:tmp=rx6.read();break;	
        case 7:tmp=rx7.read();break;	
        case 8:tmp=rx8.read();break;	
        case 9:tmp=rx9.read();break;	
        }
	
	unsigned int ID=getPacketId(tmp.data);
	unsigned int channel=packet_ids[ID];
	
	for(int i = 0; i < PACKET_LEN; i++)
	  {
#pragma HLS PIPELINE II=1
	    switch(cu){
	    case 0:x=rx0.read();break;
	    case 1:x=rx1.read();break;
	    case 2:x=rx2.read();break;
	    case 3:x=rx3.read();break;
	    case 4:x=rx4.read();break;
	    case 5:x=rx5.read();break;
	    case 6:x=rx6.read();break;
	    case 7:x=rx7.read();break;
	    case 8:x=rx8.read();break;
	    case 9:x=rx9.read();break;
	    }
	    
	    switch(channel){
	    case 0:tx0.write(x);break;
	    case 1:tx1.write(x);break;
	    case 2:tx2.write(x);break;
	    case 3:tx3.write(x);break;
	    }
	  }
      }
  }
}

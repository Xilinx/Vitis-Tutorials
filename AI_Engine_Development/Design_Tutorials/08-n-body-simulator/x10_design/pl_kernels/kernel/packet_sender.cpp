/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
  
#include "packet_sender.hpp"
ap_uint<32> generateHeader(unsigned int pktType, unsigned int ID){

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
		   )
{
#pragma HLS interface axis port=rx

#pragma HLS interface axis port=tx0
#pragma HLS interface axis port=tx1
#pragma HLS interface axis port=tx2
#pragma HLS interface axis port=tx3
#pragma HLS interface axis port=tx4
#pragma HLS interface axis port=tx5
#pragma HLS interface axis port=tx6
#pragma HLS interface axis port=tx7
#pragma HLS interface axis port=tx8
#pragma HLS interface axis port=tx9

#pragma HLS interface s_axilite port=return bundle=control  
  axis_pkt x;
  ap_axiu<32,0,0,0> tmp;
  
  for (int cu=0; cu<NUM_CU; cu++){
    for (int h = 0; h < PACKET_NUM; h++) 
      { 
	unsigned int ID=packet_ids[h];
	ap_uint<32> header=generateHeader(PKTTYPE,ID); //packet header
	tmp.data=header;
	tmp.keep=-1;
	tmp.last=0;
	switch(cu){	
        case 0:tx0.write(tmp);break;	
        case 1:tx1.write(tmp);break;	
        case 2:tx2.write(tmp);break;	
        case 3:tx3.write(tmp);break;	
        case 4:tx4.write(tmp);break;	
        case 5:tx5.write(tmp);break;	
        case 6:tx6.write(tmp);break;	
        case 7:tx7.write(tmp);break;	
        case 8:tx8.write(tmp);break;	
        case 9:tx9.write(tmp);break;	
        }

	for(int i = 0; i < PACKET_LEN; i++)
	  {
#pragma HLS PIPELINE II=1
	    x=rx.read();
	    x.last = (i==PACKET_LEN-1);
	   
	    switch(cu){	
	    case 0:tx0.write(x);break;	
	    case 1:tx1.write(x);break;	
	    case 2:tx2.write(x);break;	
	    case 3:tx3.write(x);break;	
	    case 4:tx4.write(x);break;	
	    case 5:tx5.write(x);break;	
	    case 6:tx6.write(x);break;	
	    case 7:tx7.write(x);break;	
	    case 8:tx8.write(x);break;	
	    case 9:tx9.write(x);break;	
	    }
	  }
      }
  }
}


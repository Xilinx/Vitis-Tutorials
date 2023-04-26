/*Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "packet_sender.hpp"

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

void packet_sender(hls::stream<axis_pkt>& rx, hls::stream<axis_pkt>& tx)
{
#pragma HLS interface axis port=tx
#pragma HLS interface axis port=rx

#pragma HLS interface s_axilite port=return bundle=control  
   
  axis_pkt x;
  
  for (int h = 0; h < PACKET_NUM; h++) 
    { 
      unsigned int ID=packet_ids[h];
      ap_uint<32> header=generateHeader(PKTTYPE,ID); //packet header
      ap_axiu<32,0,0,0> tmp;
      tmp.data=header;
      tmp.keep=-1;
      tmp.last=0;
      tx.write(tmp);
      for(int i = 0; i < PACKET_LEN; i++)
	{
#pragma HLS PIPELINE II=1
	  x=rx.read();
	  x.last = (i==PACKET_LEN-1);
	  tx.write(x);
	}
    }
}

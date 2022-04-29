/**********
© Copyright 2020-2022 Xilinx, Inc.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/
#include "hls_stream.h"
#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "packet_ids_c.h"

static const unsigned int pktType=0;
static const int PACKET_NUM=4; //How many kernels do packet switching
static const int PACKET_LEN=8; //Length for a packet

static const unsigned int packet_ids[PACKET_NUM]={Datain0_0, Datain0_1, Datain0_2, Datain0_3}; //macro values are generated in packet_ids_c.h

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

void hls_packet_sender(hls::stream<ap_axiu<32,0,0,0>> &s0,hls::stream<ap_axiu<32,0,0,0>> &s1,hls::stream<ap_axiu<32,0,0,0>> &s2,hls::stream<ap_axiu<32,0,0,0>> &s3,
		hls::stream<ap_axiu<32,0,0,0>> &out, const unsigned int num){
	for(unsigned int iter=0;iter<num;iter++){
		for(int i=0;i<PACKET_NUM;i++){//Iterate on PL kernels that do packet switching
			unsigned int ID=packet_ids[i];
			ap_uint<32> header=generateHeader(pktType,ID); //packet header
			ap_axiu<32,0,0,0> tmp;
			tmp.data=header;
			tmp.keep=-1;
			tmp.last=0;
			out.write(tmp);
			for(int j=0;j<PACKET_LEN;j++){ //packet data
				switch(i){//based on which kernel is sending packet, read the corresponding stream
				case 0:tmp=s0.read();break;
				case 1:tmp=s1.read();break;
				case 2:tmp=s2.read();break;
				case 3:tmp=s3.read();break;
				}
				if(j==PACKET_LEN-1){
					tmp.last=1; //last word in a packet has TLAST=1
				}else{
					tmp.last=0;
				}
				out.write(tmp);
			}
		}
	}
}

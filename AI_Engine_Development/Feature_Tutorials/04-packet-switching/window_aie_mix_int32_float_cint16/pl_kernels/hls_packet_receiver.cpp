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

static const int PACKET_NUM=4;
static const int PACKET_LEN=8;

static const unsigned int packet_ids[PACKET_NUM]={Dataout0_0, Dataout0_1, Dataout0_2, Dataout0_3};

unsigned int getPacketId(ap_uint<32> header){
#pragma HLS inline
	ap_uint<32> ID=0;
	ID(4,0)=header(4,0);
	return ID;
}

void hls_packet_receiver(hls::stream<ap_axiu<32,0,0,0>> &in, hls::stream<ap_axiu<32,0,0,0>> &out0,hls::stream<ap_axiu<32,0,0,0>> &out1,hls::stream<ap_axiu<32,0,0,0>> &out2,hls::stream<ap_axiu<32,0,0,0>> &out3,
		const unsigned int total_num_packet){
	for(unsigned int iter=0;iter<total_num_packet;iter++){
		ap_axiu<32,0,0,0> tmp=in.read();//first word is packet header
		unsigned int ID=getPacketId(tmp.data);
		unsigned int channel=packet_ids[ID];
		for(int j=0;j<PACKET_LEN;j++){
			tmp=in.read();
			switch(channel){
			case 0:out0.write(tmp);break;
			case 1:out1.write(tmp);break;
			case 2:out2.write(tmp);break;
			case 3:out3.write(tmp);break;
			}
		}
	}
}

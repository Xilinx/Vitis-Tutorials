/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <stdlib.h>
#include <iostream>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

extern "C" void datagen(hls::stream<ap_axis<128,0,0,0>> & out, int size) {
#pragma HLS INTERFACE axis port=out
#pragma HLS INTERFACE s_axilite port=return bundle=control
#pragma HLS INTERFACE ap_ctrl_hs port=return
#pragma HLS interface s_axilite port=size bundle=control
#pragma HLS interface ap_none port=size
	for(int i=0;i<size;i++){
		ap_axis<128,0,0,0> tmp;
		tmp.data(15,0)=0x0; //bfloat16=0
		tmp.data(31,16)=0x3f80; //bfloat16=1 
		tmp.data(47,32)=0x4000; //bfloat16=2
		tmp.data(63,48)=0x4040; //bfloat16=3
		tmp.data(79,64)=0x4080; //bfloat16=4
		tmp.data(95,80)=0x40a0; //bfloat16=5
		tmp.data(111,96)=0x40c0; //bfloat16=6
		tmp.data(127,112)=0x40e0; //bfloat16=7
		tmp.keep=-1;
		tmp.last=0;
		out.write(tmp);
	}
}


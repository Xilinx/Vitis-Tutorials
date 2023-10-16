/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <stdlib.h>
#include <iostream>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

extern "C" void datagen(hls::stream<ap_axis<32,0,0,0>> & out, int size, int init_val=0) {
#pragma HLS INTERFACE axis port=out
#pragma HLS INTERFACE s_axilite port=return bundle=control
#pragma HLS INTERFACE ap_ctrl_hs port=return
#pragma HLS interface s_axilite port=size bundle=control
#pragma HLS interface ap_none port=size
#pragma HLS interface s_axilite port=init_val bundle=control
#pragma HLS interface ap_none port=init_val
	for(int i=0;i<size;i++){
		ap_axis<32,0,0,0> tmp;
		tmp.data=i+init_val;
		tmp.keep=-1;
		tmp.last=0;
		out.write(tmp);
	}
}

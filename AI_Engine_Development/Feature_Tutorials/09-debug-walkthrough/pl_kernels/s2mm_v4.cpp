/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

const int STREAM_NUM=4;
const int STREAM_WIDTH=64;

extern "C" {

void s2mm_v4(ap_int<STREAM_WIDTH*STREAM_NUM>* mem, hls::stream<ap_axis<STREAM_WIDTH, 0, 0, 0>  > &s0, hls::stream<ap_axis<STREAM_WIDTH, 0, 0, 0>  > &s1, hls::stream<ap_axis<STREAM_WIDTH, 0, 0, 0>  > &s2, hls::stream<ap_axis<STREAM_WIDTH, 0, 0, 0>  > &s3, int size) {
#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem

#pragma HLS INTERFACE s_axilite port=mem bundle=control
#pragma HLS INTERFACE s_axilite port=size bundle=control
#pragma HLS interface s_axilite port=return bundle=control

	for(int i = 0; i < size; i++) {
#pragma HLS PIPELINE II=1
		ap_int<STREAM_WIDTH*STREAM_NUM> data;
		data(STREAM_WIDTH-1,0)=s0.read().data;
		data(STREAM_WIDTH*2-1,STREAM_WIDTH)=s1.read().data;
		data(STREAM_WIDTH*3-1,STREAM_WIDTH*2)=s2.read().data;
		data(STREAM_WIDTH*4-1,STREAM_WIDTH*3)=s3.read().data;
		mem[i]=data;
	}

}

}


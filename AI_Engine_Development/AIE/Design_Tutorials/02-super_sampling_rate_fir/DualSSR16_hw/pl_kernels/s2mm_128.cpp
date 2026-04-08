/*
Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>


extern "C" {

void s2mm_128(ap_int<128>* mem, hls::stream<qdma_axis<128, 0, 0, 0>  >& s, int size) {
static ap_int<128> v[512];

#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem

#pragma HLS interface axis port=s

#pragma HLS INTERFACE s_axilite port=mem bundle=control
#pragma HLS INTERFACE s_axilite port=size bundle=control
#pragma HLS interface s_axilite port=return bundle=control

	for(int i = 0; i < size; i++) {
#pragma HLS PIPELINE II=1
		qdma_axis<128, 0, 0, 0> x = s.read();
		v[i%512]=x.data;
	}

	for(int i = 0; i < 512; i++) {
#pragma HLS PIPELINE II=1
		mem[i] = v[i];
	}

}

}

/*
Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>


extern "C" {

void s2mm_dual_128(ap_int<128>* mem1, ap_int<128>* mem2, hls::stream<qdma_axis<128, 0, 0, 0>  >& s1, hls::stream<qdma_axis<128, 0, 0, 0>  >& s2, int size) {
static ap_int<128> v1[512];
static ap_int<128> v2[512];

#pragma HLS INTERFACE m_axi port=mem1 offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=mem2 offset=slave bundle=gmem

#pragma HLS interface axis port=s1
#pragma HLS interface axis port=s2

#pragma HLS INTERFACE s_axilite port=mem1 bundle=control
#pragma HLS INTERFACE s_axilite port=mem2 bundle=control
#pragma HLS INTERFACE s_axilite port=size bundle=control
#pragma HLS interface s_axilite port=return bundle=control

	for(int i = 0; i < size; i++) {
#pragma HLS PIPELINE II=1
		qdma_axis<128, 0, 0, 0> x1 = s1.read();
		v1[i%512]=x1.data;
		qdma_axis<128, 0, 0, 0> x2 = s2.read();
		v2[i%512]=x2.data;
	}

	for(int i = 0; i < 512; i++) {
#pragma HLS PIPELINE II=1
		mem1[i] = v1[i];
		mem2[i] = v2[i];
	}

}

}

/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>


extern "C" {

void s2ss(hls::stream<ap_axis<128, 0, 0, 0>  >& s, int size) {
#pragma HLS interface axis port=s

#pragma HLS INTERFACE s_axilite port=size bundle=control
#pragma HLS interface s_axilite port=return bundle=control
	for(int i = 0; i < size; i++) {
#pragma HLS PIPELINE II=1
		ap_axis<128, 0, 0, 0> x = s.read();
	}
}

}

/*
 Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
 SPDX-License-Identifier: MIT

 Author: Peifang Zhou, Bachir Berkane, Mark Rollins

Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
*/


#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>


extern "C" {

void mm2s(ap_int<128>* mem, hls::stream<ap_int<128>  >& s, int size) {
#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem

#pragma HLS interface axis port=s

#pragma HLS INTERFACE s_axilite port=mem bundle=control
#pragma HLS INTERFACE s_axilite port=size bundle=control
#pragma HLS interface s_axilite port=return bundle=control

	for(int i = 0; i < size; i++) {
#pragma HLS PIPELINE II=1
		ap_int<128> x = mem[i];
		s.write(x);
	}

}

}

/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>


extern "C" {

void mm2s_1(ap_int<32>* mem, hls::stream<qdma_axis<32, 0, 0, 0>  >& s, int size, int* written) {
#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem

#pragma HLS interface axis port=s

#pragma HLS INTERFACE s_axilite port=mem bundle=control
#pragma HLS INTERFACE s_axilite port=size bundle=control
#pragma HLS interface s_axilite port=return bundle=control

#pragma HLS INTERFACE s_axilite port=written bundle=control offset=0x38
#pragma HLS INTERFACE ap_vld port=written

    int written_local = 0;

	for(int i = 0; i < size; i++) {
#pragma HLS PIPELINE II=1
		qdma_axis<32, 0, 0, 0> x;
		x.data = mem[i];
		x.keep_all();
		s.write(x);
		(*written) = (++written_local);		
	}

}

}

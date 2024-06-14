/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>


extern "C" {

	#define BUS_SIZE 
	#define DATA_SIZE 
	#define ARRAY_SIZE 
	#define FRAME_SIZE 

	void FUNCTION_NAME (ap_int<BUS_SIZE>* mem, hls::stream<qdma_axis<BUS_SIZE, 0, 0, 0>  >& s, int size) {
		#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem

		#pragma HLS interface axis port=s

		#pragma HLS INTERFACE s_axilite port=mem bundle=control
		#pragma HLS INTERFACE s_axilite port=size bundle=control
		#pragma HLS interface s_axilite port=return bundle=control
		ap_int<BUS_SIZE> v[ARRAY_SIZE];

		for(int i=0;i<ARRAY_SIZE;i++)
		{
			#pragma HLS PIPELINE II=1
			v[i] = mem[i];
		}

		for(int i = 0; i < size; i++) {
			#pragma HLS PIPELINE II=1
			qdma_axis<BUS_SIZE, 0, 0, 0> x;
			x.data = v[i%ARRAY_SIZE];
			x.last = ((i%FRAME_SIZE)==127);
			x.keep_all();
			s.write(x);
		}

	}

}

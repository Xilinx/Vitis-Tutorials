/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/


#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

// The following header file deines the inner size of array v
#include "system_settings_8.h"

extern "C" {
	#define BUS_SIZE 128
	#define RATIO NBitsData/BUS_SIZE
	#define ArraySize (InputArraySize*RATIO)

	void mm2s_8_128(ap_int<BUS_SIZE>* mem, hls::stream<ap_axis<BUS_SIZE, 0, 0, 0>  >& s, int size)
	{
		#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem

		#pragma HLS interface axis port=s

		#pragma HLS INTERFACE s_axilite port=mem bundle=control
		#pragma HLS INTERFACE s_axilite port=size bundle=control
		#pragma HLS interface s_axilite port=return bundle=control
		ap_int<BUS_SIZE> v[ArraySize];

		for(int i=0;i<ArraySize;i++)
		{
			#pragma HLS PIPELINE II=1
			v[i] = mem[i];
		}

		for(int i = 0; i < size; i++) {
			#pragma HLS PIPELINE II=1
			ap_axis<BUS_SIZE, 0, 0, 0> x;
			x.data = v[i%ArraySize];
			s.write(x);
		}
	}
}
